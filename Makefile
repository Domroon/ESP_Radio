SKETCH=ESP_Radio.ino
PORT=/dev/ttyUSB0
BOARD=esp32:esp32:nodemcu-32s

all:
	arduino-cli compile --fqbn $(BOARD) $(SKETCH)
	sudo chmod a+rw $(PORT)
	arduino-cli upload -p $(PORT) --fqbn $(BOARD) $(SKETCH)
	arduino-cli monitor -p $(PORT) --fqbn $(BOARD) -c baudrate=115200

compile:
	arduino-cli compile --fqbn $(BOARD) $(SKETCH)

upload:
	sudo chmod a+rw $(PORT)
	arduino-cli upload -p $(PORT) --fqbn $(BOARD) $(SKETCH)

monitor:
	sudo chmod a+rw $(PORT)
	arduino-cli monitor -p $(PORT) --fqbn $(BOARD) -c baudrate=115200