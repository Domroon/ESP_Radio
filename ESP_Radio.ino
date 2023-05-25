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
unsigned long menuMillis;
unsigned long displayMillis;
unsigned long waitMillis;

String streamtitle;
int infotextLen;
int scrolls;
int title_from = 0;
int title_to = 16;

Item* currentStation = NULL;

int button_up = 35;
int button_down = 34;
int button_enter = 39;

bool button_up_is_pressed = false;
bool button_down_is_pressed = false;
bool button_enter_is_pressed = false;

// Display Menu
String displayView[2];
int top_station = 0;
int bottom_station = 1;
int cursor = 0;
int scrollWaitMillis = 0;

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
    // read line by line and add each line into dict
    // but first test reading from textfile on SD card
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
        // mache einen substring von begin bis zu diesem index (key)
        for(int i=0; i<index; i++){
            key[i] = line[i];
        }
        key[index] = '\0';
        // mache einen substring von diesem index bis ende
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

// void menu_loop() {
//     // if (millis() - menuMillis >= 10000) {
//     //     audio.connecttohost("http://st01.dlf.de/dlf/01/128/mp3/stream.mp3");
//     //     menuMillis = millis();
//     // }
//     show_text(0, 0, ">");
//     show_text(1, 0, currentStation->key);
//     Item* nextStation = (Item*) currentStation->next;
//     show_text(1, 1, nextStation->key);
//     if (bottom_station > STATIONS){
//         top_station = 0;
//         bottom_station = 1;
//     }
//     if (top_station < 0){
//         top_station = STATIONS - 1;
//         bottom_station = STATIONS;
//     }

//     if (button_down_is_pressed
//         && millis() - scrollWaitMillis >= 200) {
//         lcd.clear();
//         top_station++;
//         bottom_station++;
//         scrollWaitMillis = millis();
//     }
//     if (button_up_is_pressed
//         && millis() - scrollWaitMillis >= 200) {
//         lcd.clear();
//         top_station--;
//         bottom_station--;
//         scrollWaitMillis = millis();
//     }
//     if (button_enter_is_pressed
//         && millis() - scrollWaitMillis >= 1000){
//         lcd.clear();
//         audio.connecttohost(stationurl[top_station]);
//         scrollWaitMillis = millis();
//         delay(1000);
//     }
// }


void check_buttons_loop() {
    // Button UP
    if(digitalRead(button_up) && !button_up_is_pressed){
        Serial.println("UP button is pressed");
        button_up_is_pressed = true;
        delay(200);
    }
    if(!digitalRead(button_up) && button_up_is_pressed){
        Serial.println("UP button has been released");
        button_up_is_pressed = false;
        menuMillis = millis();
        delay(200);
    }

    // Button DOWN
    if(digitalRead(button_down) && !button_down_is_pressed){
        Serial.println("DOWN button is pressed");
        button_down_is_pressed = true;
        delay(200);
    }
    if(!digitalRead(button_down) && button_down_is_pressed){
        Serial.println("DOWN button has been released");
        button_down_is_pressed = false;
        menuMillis = millis();
        delay(200);
    }
    
    // Button ENTER
    if(digitalRead(button_enter) && !button_enter_is_pressed){
        Serial.println("ENTER button is pressed");
        button_enter_is_pressed = true;
        delay(200);
    }
    if(!digitalRead(button_enter) && button_enter_is_pressed){
        Serial.println("ENTER button has been released");
        button_enter_is_pressed = false;
        menuMillis = 0;
    }
}


void show_station_loop(Item* station) {
    infotextLen = streamtitle.length();
    if(millis() - displayMillis >= 500) {
        show_text(0, 0, station->key);
        show_text(0, 1, "                ");
        show_text(0, 1, streamtitle.substring(title_from, title_to));

        displayMillis = millis();
        scrolls = scrolls + 1;
        title_from = title_from + 1;
        title_to = title_to + 1;
    }

    if (scrolls == infotextLen + 1) {
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
    Dict* stations = NULL;
    Item* firstStation = NULL;
    if(sdCardMounted){
        stations = loadRadioStations();
        firstStation = (Item*) stations->firstItem;
        currentStation = firstStation;
    }

    Serial.begin(115200);
    
    // init buttons
    pinMode(button_up, INPUT);
    pinMode(button_down, INPUT);
    pinMode(button_enter, INPUT);

    lcd.init();                      
    lcd.backlight();
    
    show_text(0, 0, "ESP Radio");
    show_text(0, 1, "Connect to WLAN");

    menuMillis = -2000;
    displayMillis = millis();
    initWiFi();
    
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.connecttohost(firstStation->value);

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
    if (button_up_is_pressed 
        || button_down_is_pressed 
        || button_enter_is_pressed 
        || millis() - menuMillis >= 0 && millis() - menuMillis <= 2000){
        // menuLoop;
    } else {
        if(currentStation != NULL){
            show_station_loop(currentStation);
        }
    }
    check_buttons_loop();
}
