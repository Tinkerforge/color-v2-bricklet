// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_color_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Color Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


static TF_ColorV2 c;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_color_v2_create(&c, UID, hal), "create device object");

	// Get current color
	uint16_t r, g, b, c_;
	check(tf_color_v2_get_color(&c, &r, &g, &b, &c_), "get color");

	tf_hal_printf("Color [R]: %I16u\n", r);
	tf_hal_printf("Color [G]: %I16u\n", g);
	tf_hal_printf("Color [B]: %I16u\n", b);
	tf_hal_printf("Color [C]: %I16u\n", c_);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
