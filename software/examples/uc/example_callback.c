// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_color_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Color Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);


// Callback function for color callback
static void color_handler(TF_ColorV2 *device, uint16_t r, uint16_t g, uint16_t b,
                          uint16_t c, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Color [R]: %I16u\n", r);
	tf_hal_printf("Color [G]: %I16u\n", g);
	tf_hal_printf("Color [B]: %I16u\n", b);
	tf_hal_printf("Color [C]: %I16u\n", c);
	tf_hal_printf("\n");
}

static TF_ColorV2 c;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_color_v2_create(&c, UID, hal), "create device object");

	// Register color callback to function color_handler
	tf_color_v2_register_color_callback(&c,
	                                    color_handler,
	                                    NULL);

	// Set period for color callback to 0.1s (100ms)
	tf_color_v2_set_color_callback_configuration(&c, 100, false);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
