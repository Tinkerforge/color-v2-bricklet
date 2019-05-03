function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletColorV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Color Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    c = handle(BrickletColorV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current color
    color = c.getColor();

    fprintf('Color [R]: %i\n', color.r);
    fprintf('Color [G]: %i\n', color.g);
    fprintf('Color [B]: %i\n', color.b);
    fprintf('Color [C]: %i\n', color.c);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
