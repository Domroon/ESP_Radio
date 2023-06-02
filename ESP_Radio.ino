#include <stdlib.h>
#include <WiFi.h>
#include <Audio.h>
#include <LiquidCrystal_I2C.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"

extern "C" {
  #include "dict.h"
}


#define I2S_DOUT      26  // connect to DAC pin DIN
#define I2S_BCLK      27  // connect to DAC pin BCK
#define I2S_LRC       25  // connect to DAC pin LCK

#define STATIONS 17

Audio audio;
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

char ssid[] = "AlphaCentauri";
char password[] = "6ER6bXskskZ";
unsigned long displayMillis;

String streamtitle;
int infotextLen;
int scrolls;
int title_from = 0;
int title_to = 16;

Dict* stations = NULL;
Item* currentStation = NULL;
Item* firstStation = NULL;

int button_up = 35;
int button_down = 34;
int button_enter = 39;
int button_res_1 = 32;
int button_res_2 = 33;

bool button_up_is_pressed = false;
bool button_down_is_pressed = false;
bool button_enter_is_pressed = false;
bool button_res_1_is_pressed = false;
bool button_res_2_is_pressed = false;

// Display Menu
int streamTitleLenght;

bool inMenu = false;
unsigned long menuTime = 2001;
Item* topDisplay = NULL;
Item* bottomDisplay = NULL;

// Task for Audio Loop
TaskHandle_t audioLoopTask;
TaskHandle_t showStreamTitle;

void runAudioLoop(void* parameter) {
    while(1){
        audio.loop();
    }
}

// audio functions overwrite

void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}

void audio_showstreamtitle(const char *info){
    streamtitle = info;
    streamTitleLenght = streamtitle.length();
    title_from = 0;
    title_to = 16;
    scrolls = 0;
}

// init functions

void initWiFi() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi: ");
    Serial.print(ssid);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println();
    Serial.println("Connected");
    Serial.print("IP-Address: ");
    Serial.println(WiFi.localIP());
}

bool initSD(){
    if(!SD.begin(5)) {
        Serial.println("Card Mount Failed");
        return false;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return false;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD) {
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC) {
        Serial.println("SDHC");
    } else {
        Serial.println("UNKOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n",cardSize);
    return true;
}

void initDict(Dict* dict){
    dict->length = 0;
    dict->firstItem = NULL;
}

// general purpose functions

void show_text(int col, int row, String text) {
    lcd.setCursor(col, row);
    lcd.print(text);
}

void showDict(Dict* dict){
    Item* currentItem = (Item*) dict->firstItem;
    if (dict->length == 0){
        Serial.println("Dict is empty");
        return;
    }
    while(currentItem != NULL){
        Serial.print(currentItem->key);
        Serial.print(" : ");
        Serial.println(currentItem->value);
        currentItem = (Item*) currentItem->next;
    }
}

Dict* loadRadioStations(){
    Dict* dict = (Dict*) malloc(sizeof(Dict));
    initDict(dict);

    char path[50] = "/stations.txt";
    Serial.printf("Reading file: %s\n", path);

    File file = SD.open(path);
    int bufferLen = 250;
    int keyLen= 20;
    int valueLen = 250;
    char line[bufferLen];
    char c = NULL;
    int index = 0;
    char key[20];
    char value[250];
    if(!file){
        Serial.println("Failed to open file for reading");
        return NULL;
    }
    Serial.println("Read Radio-Stations from file:");
    while(1){
        if(!file.available()) {
            break;
        }
        c = NULL;
        for(int i=0; i<bufferLen; i++){
            if(c == '\n'){
                line[i-1] = '\0';
                break;
            }
            c = file.read();
            line[i] = c;
        }

        // split the string into key and value

        // find sign '|'
        index = 0;
        for(int i=0; i<bufferLen; i++){
            if(line[i] == '|'){
                index = i;
                break;
            }
        }

        if (index == 0){
            Serial.println("A line dont't have a \"|\" seperator.");
            return NULL;
        }
        // make a substring from begin through this index (key)
        for(int i=0; i<index; i++){
            key[i] = line[i];
        }
        key[index] = '\0';
        // make a substring from this index through the end
        for(int i=0; i<bufferLen; i++){
            value[i] = line[i+index+1];
            if(line[i] == '\n'){
                line[i] = '\0';
                break;
            }
        }
        addItem(key, value, dict);
    }
    file.close();
    showDict(dict);
    return dict;
}

// loop functions

void show_station_loop(Item* station) {
    if(millis() - displayMillis >= 500) {
        show_text(0, 0, station->key);
        show_text(0, 1, "                ");
        show_text(0, 1, streamtitle.substring(title_from, title_to));

        displayMillis = millis();
        scrolls = scrolls + 1;
        title_from = title_from + 1;
        title_to = title_to + 1;
    }

    if (scrolls == streamTitleLenght + 1) {
        title_from = 0;
        title_to = 16;
        show_text(0, 1, "                ");
        show_text(0, 1, streamtitle.substring(title_from, title_to));
        scrolls = 0;
    }
}

void setup() {
    // Stations init
    bool sdCardMounted = initSD();
    if(sdCardMounted){
        stations = loadRadioStations();
        firstStation = (Item*) stations->firstItem;
        currentStation = firstStation;
        topDisplay = currentStation;
        bottomDisplay = (Item*) topDisplay->next;
    }

    Serial.begin(115200);
    
    // init buttons
    pinMode(button_up, INPUT);
    pinMode(button_down, INPUT);
    pinMode(button_enter, INPUT);
    pinMode(button_res_1, INPUT);
    pinMode(button_res_2, INPUT);

    lcd.init();                      
    lcd.backlight();
    
    show_text(0, 0, "ESP Radio");
    show_text(0, 1, "Connect to WLAN");

    initWiFi();
    
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.connecttohost(firstStation->value);

    displayMillis = 0;
    show_station_loop(firstStation);

    // start audio loop
    xTaskCreatePinnedToCore(
        runAudioLoop,
        "AudioLoop",
        10000,
        NULL,
        1,
        &audioLoopTask,
        1
    );
}

void loop() {
    // 1.If any Button is released: Menu View is active
    // 2.If any Button is released for 2s or Enter Button is pressed: Menu View will deactivated
    // 3.If Button Down is released while the Menu View is active: Scroll Down
    // 4.If Menu View is active and Enter Button is pressed: Connect to 
    //   Radio Station that is on Top
    // 5.If the Scroll List hits the End: Show first Station on Top

    // Button DOWN
    if(digitalRead(button_down) && !button_down_is_pressed){
        button_down_is_pressed = true;
        Serial.println("DOWN button is pressed");
        delay(200);
    }
    if(!digitalRead(button_down) && button_down_is_pressed){
        button_down_is_pressed = false;
        inMenu = true;
        menuTime = millis();
        topDisplay = (Item*) topDisplay->next;
        if (topDisplay == NULL) {
            topDisplay = firstStation;
        }
        bottomDisplay = (Item*) topDisplay->next;
        lcd.clear();
        Serial.println("DOWN button has been released");
        Serial.println("In Menu");
    }

    // Button UP
    if(digitalRead(button_up) && !button_up_is_pressed){
        button_up_is_pressed = true;
        Serial.println("UP button is pressed");
        delay(200);
    }
    if(!digitalRead(button_up) && button_up_is_pressed){
        button_up_is_pressed = false;
        Serial.println("UP button has been released");
    }
    
    // Button ENTER
    if(digitalRead(button_enter) && !button_enter_is_pressed){
        button_enter_is_pressed = true;
        Serial.println("ENTER button is pressed");
        delay(200);
    }
    if(!digitalRead(button_enter) && button_enter_is_pressed){
        button_enter_is_pressed = false;
        if (inMenu){
            currentStation = (Item*) getItem(topDisplay->key, stations);
            lcd.clear();
            show_text(0, 0, "Connecting to");
            show_text(0, 1, currentStation->key);
            audio.connecttohost(currentStation->value);
            delay(1000);
            Serial.print("streamtitle: ");
            Serial.println(streamtitle);
            lcd.clear();
            menuTime = 0;
            inMenu = false;
        }
        Serial.println("ENTER button has been released");
    }

    // Button RES1
    if(digitalRead(button_res_1) && !button_res_1_is_pressed){
        button_res_1_is_pressed = true;
        Serial.println("RES1 button is pressed");
        delay(200);
    }
    if(!digitalRead(button_res_1) && button_res_1_is_pressed){
        button_res_1_is_pressed = false;
        Serial.println("RES1 button has been released");
    }

    // Button RES2
    if(digitalRead(button_res_2) && !button_res_2_is_pressed){
        button_res_2_is_pressed = true;
        Serial.println("RES2 button is pressed");
        delay(200);
    }
    if(!digitalRead(button_res_2) && button_res_2_is_pressed){
        button_res_2_is_pressed = false;
        Serial.println("RES2 button has been released");
    }


    if(millis() - menuTime > 2000 && millis() - menuTime <= 2100){
        lcd.clear();
        Serial.println("Menu Time is up");
        Serial.println(menuTime);
        topDisplay = firstStation;
        bottomDisplay = (Item*) topDisplay->next;
        inMenu = false;
    }

    if (currentStation == NULL) {
        Serial.println("Current Station do not have a value!");
        delay(2000);
    }

    if(currentStation != NULL && !inMenu){
        show_station_loop(currentStation);
    }

    if(inMenu){
        show_text(0, 0, ">");
        show_text(1, 0, topDisplay->key);
        show_text(0, 1, bottomDisplay->key);
    }
}
