avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -o temp $1
avr-objcopy -O ihex -R .eeprom temp temp.hex
sudo avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:temp.hex
