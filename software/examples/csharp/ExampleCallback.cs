using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Color Bricklet 2.0

	// Callback function for color callback
	static void ColorCB(BrickletColorV2 sender, int r, int g, int b, int c)
	{
		Console.WriteLine("Color [R]: " + r);
		Console.WriteLine("Color [G]: " + g);
		Console.WriteLine("Color [B]: " + b);
		Console.WriteLine("Color [C]: " + c);
		Console.WriteLine("");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletColorV2 c = new BrickletColorV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register color callback to function ColorCB
		c.ColorCallback += ColorCB;

		// Set period for color callback to 0.1s (100ms)
		c.SetColorCallbackConfiguration(100, false);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
