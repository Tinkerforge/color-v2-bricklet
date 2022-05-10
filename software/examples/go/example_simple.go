package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/color_v2_bricklet"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Color Bricklet 2.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	c, _ := color_v2_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Get current color.
	r, g, b, c_, _ := c.GetColor()

	fmt.Printf("Color [R]: %d\n", r)
	fmt.Printf("Color [G]: %d\n", g)
	fmt.Printf("Color [B]: %d\n", b)
	fmt.Printf("Color [C]: %d\n", c_)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
