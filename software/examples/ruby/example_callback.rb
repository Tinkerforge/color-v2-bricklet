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

# Register color callback
c.register_callback(BrickletColorV2::CALLBACK_COLOR) do |r, g, b, c_|
  puts "Color [R]: #{r}"
  puts "Color [G]: #{g}"
  puts "Color [B]: #{b}"
  puts "Color [C]: #{c_}"
  puts ''
end

# Set period for color callback to 0.1s (100ms)
c.set_color_callback_configuration 100, false

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
