import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletColorV2;

public class ExampleCallback {
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

		// Add color listener
		c.addColorListener(new BrickletColorV2.ColorListener() {
			public void color(int r, int g, int b, int c) {
				System.out.println("Color [R]: " + r);
				System.out.println("Color [G]: " + g);
				System.out.println("Color [B]: " + b);
				System.out.println("Color [C]: " + c);
				System.out.println("");
			}
		});

		// Set period for color callback to 0.1s (100ms)
		c.setColorCallbackConfiguration(100, false);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
