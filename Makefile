BOARD = esp32:esp32:esp32s2:CDCOnBoot=cdc
PORT := $(wildcard /dev/serial/by-id/usb-Espressif_ESP32-S2_0* /dev/serial/by-id/usb-Espressif_Systems_ESP32S2_DEV_0*)
PORT := $(strip $(PORT))

build: .dependencies
	arduino-cli compile --fqbn $(BOARD) src/4_lesson

clean_upload: build
	arduino-cli upload --fqbn $(BOARD) -p "/dev/serial/by-id/usb-Espressif_ESP32-S2_0-if00" src/4_lesson

upload:
	arduino-cli upload --fqbn $(BOARD) -p "/dev/serial/by-id/usb-Espressif_ESP32-S2_0-if00" src/4_lesson

monitor:
	arduino-cli monitor -p "$(PORT)" -c baudrate=115200

dependencies .dependencies:
	arduino-cli core update-index --config-file arduino-cli.yaml
	arduino-cli core install esp32:esp32 --config-file arduino-cli.yaml
	arduino-cli lib update-index
	arduino-cli lib install "Joystick_ESP32S2"
	touch .dependencies

clean:
	rm -rf .dependencies

.PHONY: build upload monitor dependencies clean
