#include <WiFi.h>
#include <Audio.h>
#include <LiquidCrystal_I2C.h>

#define I2S_DOUT      26  // connect to DAC pin DIN
#define I2S_BCLK      27  // connect to DAC pin BCK
#define I2S_LRC       25  // connect to DAC pin LCK

#define STATIONS 25

Audio audio;
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

char ssid[] = "AlphaCentauri";
char password[] = "6ER6bXskskZ";
unsigned long menuMillis;
unsigned long displayMillis;

String streamtitle;
int infotextLen;
int scrolls;
int title_from = 0;
int title_to = 16;

uint8_t stationnumber = 0;
uint8_t actStation = stationnumber;
const char* stationurl[STATIONS];
String stationname[STATIONS];


// audio functions overwrite

void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}

void audio_showstreamtitle(const char *info){
    streamtitle = info;
    show_text(0, 1, streamtitle.substring(0,infotextLen));
    infotextLen = streamtitle.length();
}

void audio_showstation(const char *info){
    show_text(0, 0, stationname[stationnumber]);
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


// general purpose functions

void show_text(int col, int row, String text) {
    lcd.setCursor(col, row);
    lcd.print(text);
}


// loop functions

void menu_loop() {
    if (millis() - menuMillis >= 10000) {
        audio.connecttohost("http://st01.dlf.de/dlf/01/128/mp3/stream.mp3");
        menuMillis = millis();
    }
}

void display_scroll(int row) {
    if(millis() - displayMillis >= 500) {
        // infotext_change.remove(0, 1);
        show_text(0, row, "                ");
        show_text(0, row, streamtitle.substring(title_from, title_to));

        displayMillis = millis();
        scrolls = scrolls + 1;
        title_from = title_from + 1;
        title_to = title_to + 1;
    }

    if (scrolls == infotextLen + 1) {
        title_from = 0;
        title_to = 16;
        show_text(0, row, "                ");
        show_text(0, row, streamtitle.substring(title_from, title_to));
        scrolls = 0;
    }
}



void setup() {
    lcd.init();                      
    lcd.backlight();
    
    show_text(0, 0, "ESP Radio");
    show_text(0, 1, "Connect to WLAN");

    menuMillis = millis();
    displayMillis = millis();
    initWiFi();
    
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(4);
    // audio.connecttohost("http://wdr-1live-live.icecast.wdr.de/wdr/1live/live/mp3/128/stream.mp3");
    setup_senderList();
    audio.connecttohost(stationurl[stationnumber]);
}


void loop() {
    audio.loop();
    // menu_loop();
    display_scroll(1);
}
