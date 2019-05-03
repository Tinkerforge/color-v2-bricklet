#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_color_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Color Bricklet 2.0

ipcon = IPConnection.new # Create IP connection
c = BrickletColorV2.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get current color as [r, g, b, c]
color = c.get_color

puts "Color [R]: #{color[0]}"
puts "Color [G]: #{color[1]}"
puts "Color [B]: #{color[2]}"
puts "Color [C]: #{color[3]}"

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
