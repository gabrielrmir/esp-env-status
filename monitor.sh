# /bin/bash

PORT=/dev/ttyUSB0
RATE=115200

arduino-cli monitor -p $PORT --config $RATE
