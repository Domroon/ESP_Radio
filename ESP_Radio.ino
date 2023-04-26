#include <WiFi.h>
#include <Audio.h>

#define I2S_DOUT      26  // connect to DAC pin DIN
#define I2S_BCLK      27  // connect to DAC pin BCK
#define I2S_LRC       25  // connect to DAC pin LCK

Audio audio;

char ssid[] = "AlphaCentauri";
char password[] = "6ER6bXskskZ";
unsigned long menuMillis;

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

void menu_loop() {
    if (millis() - menuMillis >= 1000) {
        Serial.println("HALLO WELT :)");
        menuMillis = millis();
    }
}

void setup() {
    menuMillis = millis();
    initWiFi();
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(2);
    audio.connecttohost("http://wdr-1live-live.icecast.wdr.de/wdr/1live/live/mp3/128/stream.mp3");
}

void loop() {
    audio.loop();
    // menu_loop();
}
