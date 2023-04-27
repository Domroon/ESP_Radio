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


## Arduino String Operationen
https://arduinogetstarted.com/de/reference/arduino-string-remove


