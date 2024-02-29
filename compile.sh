#!/bin/bash

full_board_name=arduino:sam:arduino_due_x_dbg
ino=arduino_controller
path=/dev/ttyACM0


arduino-cli compile --fqbn $full_board_name $ino
arduino-cli upload -t -p $path --fqbn $full_board_name $ino


