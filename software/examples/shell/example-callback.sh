#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Color Bricklet 2.0

# Handle incoming color callbacks
tinkerforge dispatch color-v2-bricklet $uid color &

# Set period for color callback to 0.1s (100ms)
tinkerforge call color-v2-bricklet $uid set-color-callback-configuration 100 false

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
