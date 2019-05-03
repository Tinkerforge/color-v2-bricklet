#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletColorV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Color Bricklet 2.0

# Callback subroutine for color callback
sub cb_color
{
    my ($r, $g, $b, $c) = @_;

    print "Color [R]: $r\n";
    print "Color [G]: $g\n";
    print "Color [B]: $b\n";
    print "Color [C]: $c\n";
    print "\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $c = Tinkerforge::BrickletColorV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register color callback to subroutine cb_color
$c->register_callback($c->CALLBACK_COLOR, 'cb_color');

# Set period for color callback to 0.1s (100ms)
$c->set_color_callback_configuration(100, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
