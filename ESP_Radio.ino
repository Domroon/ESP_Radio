#include <WiFi.h>
#include <Audio.h>
#include <LiquidCrystal_I2C.h>

#define I2S_DOUT      26  // connect to DAC pin DIN
#define I2S_BCLK      27  // connect to DAC pin BCK
#define I2S_LRC       25  // connect to DAC pin LCK

Audio audio;
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

char ssid[] = "AlphaCentauri";
char password[] = "6ER6bXskskZ";
unsigned long menuMillis;
unsigned long displayMillis;

String Text;
int infotextLen;
String infotext_change;
int scrolls;

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

void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}

void audio_showstreamtitle(const char *info){
    Text = info;
    infotext_change = info;
    lcd.setCursor(0,1);
    lcd.print(Text.substring(0,16));
    infotextLen = Text.length() + 1;
}

void audio_showstation(const char *info){
    String Text = info;
    lcd.setCursor(0,0);
    lcd.print(Text.substring(0,16));
}

void menu_loop() {
    if (millis() - menuMillis >= 10000) {
        audio.connecttohost("http://st01.dlf.de/dlf/01/128/mp3/stream.mp3");
        // Serial.println("HALLO WELT :)");
        menuMillis = millis();
    }
}

void display_scroll(int row) {
    if(millis() - displayMillis >= 500) {
        infotext_change.remove(0, 1);
        lcd.setCursor(0, row);
        lcd.print("                ");
        lcd.setCursor(0, row);
        lcd.print(infotext_change);
        Serial.println(infotext_change);

        displayMillis = millis();
        scrolls = scrolls + 1;
    }

    if (scrolls == infotextLen) {
        infotext_change = Text;
        lcd.setCursor(0, row);
        lcd.print("                ");
        lcd.setCursor(0, row);
        lcd.print(infotext_change);
        scrolls = 0;
    }
}

void setup() {
    lcd.init();                      
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("ESP Radio");
    lcd.setCursor(0,1);
    lcd.print("Connect to WLAN");

    menuMillis = millis();
    displayMillis = millis();
    initWiFi();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ESP Radio");
    lcd.setCursor(0,1);
    lcd.print("Connected");
    
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(3);
    audio.connecttohost("http://wdr-1live-live.icecast.wdr.de/wdr/1live/live/mp3/128/stream.mp3");
}

void loop() {
    audio.loop();
    // menu_loop();
    display_scroll(1);
}
