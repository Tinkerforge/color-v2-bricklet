import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletColorV2;
import com.tinkerforge.BrickletColorV2.Color;

public class ExampleSimple {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Color Bricklet 2.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletColorV2 c = new BrickletColorV2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current color
		Color color = c.getColor(); // Can throw com.tinkerforge.TimeoutException

		System.out.println("Color [R]: " + color.r);
		System.out.println("Color [G]: " + color.g);
		System.out.println("Color [B]: " + color.b);
		System.out.println("Color [C]: " + color.c);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
