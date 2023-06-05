# ESP_Radio

## Install Arduino for ESP32 on ArchLinux

    yay arduino-cli

    arduino-cli config init --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

    arduino-cli core update-index

    arduino-cli board listall
    
    arduino-cli core install esp32:esp32

## Install Arduino Libraries

    sudo nano /home/<your-username>/.arduino15/arduino-cli.yaml
    set enable_unsafe_install to true

    arduino-cli lib install --git-url https://github.com/schreibfaul1/ESP32-audioI2S.git
    arduino-cli lib install --git-url https://github.com/johnrickman/LiquidCrystal_I2C.git

## Compile and Upload Sketch

    arduino-cli compile --fqbn esp32:esp32:nodemcu-32s MyFirstSketch.ino
    arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:nodemcu-32s MyFirstSketch.ino


## Serial Monitor

    arduino-cli monitor -p /dev/ttyUSB0 --fqbn esp32:esp32:nodemcu-32s -c baudrate=115200

## Part List
- ESP32 Dev Kit C https://www.amazon.de/gp/product/B074RG86SR/ref=ppx_yo_dt_b_asin_title_o07_s00?ie=UTF8&psc=1
- LCD Display I2C https://www.amazon.de/gp/product/B07JH6GHPR/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1
- Amplifier I2S https://de.aliexpress.com/item/1005005393398013.html?spm=a2g0o.order_list.order_list_main.11.21ef5c5fIxpWSc&gatewayAdapt=glo2deu
- SPI Micro SD Card Adapter https://www.amazon.de/dp/B077MB17JB?psc=1&ref=ppx_yo2ov_dt_b_product_details
- Micro SD Card https://www.amazon.de/dp/B00CBAUIMW?psc=1&ref=ppx_yo2ov_dt_b_product_details
- 5 x Button https://www.amazon.de/gp/product/B09WYRHPDL/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1
- 5 x 10k Resistor