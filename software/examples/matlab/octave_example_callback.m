function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Color Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    c = javaObject("com.tinkerforge.BrickletColorV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register color callback to function cb_color
    c.addColorCallback(@cb_color);

    % Set period for color callback to 0.1s (100ms)
    c.setColorCallbackConfiguration(100, false);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for color callback
function cb_color(e)
    fprintf("Color [R]: %d\n", e.r);
    fprintf("Color [G]: %d\n", e.g);
    fprintf("Color [B]: %d\n", e.b);
    fprintf("Color [C]: %d\n", e.c);
    fprintf("\n");
end
