var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Color Bricklet 2.0

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var c = new Tinkerforge.BrickletColorV2(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Set period for color callback to 0.1s (100ms)
        c.setColorCallbackConfiguration(100, false);
    }
);

// Register color callback
c.on(Tinkerforge.BrickletColorV2.CALLBACK_COLOR,
    // Callback function for color callback
    function (r, g, b, c) {
        console.log('Color [R]: ' + r);
        console.log('Color [G]: ' + g);
        console.log('Color [B]: ' + b);
        console.log('Color [C]: ' + c);
        console.log();
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
