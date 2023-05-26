# ESP_Radio

## Arduino ESP32 Documentation
https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html

## I2S 
https://www.youtube.com/watch?v=UXt27kOokh08

## MAX98357_I2S_Amplifier_Module
https://wiki.dfrobot.com/SKU_DFR0954_MAX98357_I2S_Amplifier_Module

https://www.youtube.com/watch?v=m-MPBjScNRk

https://forum.arduino.cc/t/line-level/101009

https://www.epanorama.net/circuits/speaker_to_line.html

https://www.az-delivery.de/fr/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/max98357-was-ist-eigentlich-i2s

## ESP Audio
https://www.youtube.com/watch?v=a936wNgtcRA

## Similar Project
https://github.com/coderholic/pyradio#features

## Install Arduino for ESP32

    yay arduino-cli

    arduino-cli config init --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

    arduino-cli core update-index

    arduino-cli board listall
    
    arduino-cli core install esp32:esp32

## Install Arduino Libraries

    sudo nano /home/domroon/.arduino15/arduino-cli.yaml
    set enable_unsafe_install to true

    arduino-cli lib install --git-url https://github.com/schreibfaul1/ESP32-audioI2S.git
    arduino-cli lib install --git-url https://github.com/johnrickman/LiquidCrystal_I2C.git

## arduino-cli doc

    https://arduino.github.io/arduino-cli/0.20/commands/arduino-cli_lib_install/

## Compile and Upload Sketch

    arduino-cli compile --fqbn esp32:esp32:nodemcu-32s MyFirstSketch.ino
    arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:nodemcu-32s MyFirstSketch.ino


## Serial Monitor

    arduino-cli monitor -p /dev/ttyUSB0 --fqbn esp32:esp32:nodemcu-32s -c baudrate=115200

## Useful Wifi Functions
https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/

## Stream Adressen
https://wiki.ubuntuusers.de/Internetradio/Streamadressen_ermitteln/

https://www.radio-browser.info/

https://copyprogramming.com/howto/python-code-for-internet-radio-streaming-a-comprehensive-guide?utm_content=cmp-true


## Display
https://github.com/johnrickman/LiquidCrystal_I2C

## SD Card Reader
https://randomnerdtutorials.com/esp32-microsd-card-arduino/

## Arduino String Operationen
https://arduinogetstarted.com/de/reference/arduino-string-remove

## Radio Station Collection
1LIVE -  http://wdr-1live-live.icecast.wdr.de/wdr/1live/live/mp3/128/stream.mp3

1LIVE diggi - http://wdr-1live-diggi.icecast.wdr.de/wdr/1live/diggi/mp3/128/stream.mp3

WDR Cosmo - http://wdr-cosmo-live.icecast.wdr.de/wdr/cosmo/live/mp3/128/stream.mp3

WDR2 - http://wdr-wdr2-rheinruhr.icecast.wdr.de/wdr/wdr2/rheinruhr/mp3/128/stream.mp3

Radio Bollerwagen - https://ffn-stream19.radiohost.de/radiobollerwagen_mp3-192?ref=radioplayer&listenerid=31363233323633323031-323030333a64653a343731363a3730313a623731343a346664383a313761363a39386165-3537373636-4d6f7a696c6c612f352e3020285831313b205562

AIDA Radio - https://aidaradio.de/AIDAradio.m3u

Deutschlandfunk - http://st01.dlf.de/dlf/01/128/mp3/stream.mp3

Deutschlandf Nova - http://st03.dlf.de/dlf/03/128/mp3/stream.mp3

Radio BOB! - http://streams.radiobob.de/bob-classicrock/mp3-192/streams.radiobob.de/

N-JOY - http://www.ndr.de/resources/metadaten/audio/m3u/n-joy.m3u

OLDIE Antenne - https://s1-webradio.oldie-antenne.de/oldie-antenne?aw_0_1st.playerid=OldieAntenneWebPlayer

sunshine live - http://stream.sunshine-live.de/live/mp3-192/stream.sunshine-live.de/

absolut relax - https://edge56.live-sm.absolutradio.de/absolut-relax/stream/mp3

YouFM - https://dispatcher.rndfnk.com/hr/youfm/live/mp3/high

Schwarzwaldradio - https://frontend.streamonkey.net/fho-schwarzwaldradiolive/mp3-stream.m3u

80s80s - http://streams.80s80s.de/web/mp3-192/streams.80s80s.de/

RockAntenne - http://mp3channels.webradio.rockantenne.de/rockantenne

HouseTime.FM - https://listen.housetime.fm/aac-hd.pls


## UKW FM Receiver
si4703

https://www.instructables.com/How-to-use-the-Si4703-FM-Radio-board-with-RDS-Ardu/

https://github.com/mkhuthir/Si4703