<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletColorV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletColorV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Color Bricklet 2.0

$ipcon = new IPConnection(); // Create IP connection
$c = new BrickletColorV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current color
$color = $c->getColor();

echo "Color [R]: " . $color['r'] . "\n";
echo "Color [G]: " . $color['g'] . "\n";
echo "Color [B]: " . $color['b'] . "\n";
echo "Color [C]: " . $color['c'] . "\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
