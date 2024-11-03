PORT = /dev/ttyUSB0
FQBN = esp32:esp32:esp32doit-devkit-v1

.PHONY: build upload

build: 
	arduino-cli compile --fqbn $(FQBN)

upload: 
	arduino-cli upload --fqbn $(FQBN) -p $(PORT)

