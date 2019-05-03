<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletColorV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletColorV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Color Bricklet 2.0

// Callback function for color callback
function cb_color($r, $g, $b, $c)
{
    echo "Color [R]: $r\n";
    echo "Color [G]: $g\n";
    echo "Color [B]: $b\n";
    echo "Color [C]: $c\n";
    echo "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$c = new BrickletColorV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register color callback to function cb_color
$c->registerCallback(BrickletColorV2::CALLBACK_COLOR, 'cb_color');

// Set period for color callback to 0.1s (100ms)
$c->setColorCallbackConfiguration(100, FALSE);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
