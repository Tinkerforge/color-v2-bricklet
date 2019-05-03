function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Color Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    c = javaObject("com.tinkerforge.BrickletColorV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current color
    color = c.getColor();

    fprintf("Color [R]: %d\n", color.r);
    fprintf("Color [G]: %d\n", color.g);
    fprintf("Color [B]: %d\n", color.b);
    fprintf("Color [C]: %d\n", color.c);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
