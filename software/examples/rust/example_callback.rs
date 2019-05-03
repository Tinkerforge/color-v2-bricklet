use std::thread;
use std::{error::Error, io};
use tinkerforge::{color_v2_bricklet::*, ip_connection::IpConnection};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Color Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let c = ColorV2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    let color_receiver = c.get_color_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `c` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for color in color_receiver {
            println!("Color [R]: {}", color.r);
            println!("Color [G]: {}", color.g);
            println!("Color [B]: {}", color.b);
            println!("Color [C]: {}", color.c);
            println!();
        }
    });

    // Set period for color callback to 0.1s (100ms).
    c.set_color_callback_configuration(100, false);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
