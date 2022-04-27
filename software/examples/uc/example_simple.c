// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_color_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

static TF_ColorV2 c;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_color_v2_create(&c, NULL, hal), "create device object");

	// Get current color
	uint16_t r, g, b, c_;
	check(tf_color_v2_get_color(&c, &r, &g, &b, &c_), "get color");

	tf_hal_printf("Color [R]: %I16u\n", r);
	tf_hal_printf("Color [G]: %I16u\n", g);
	tf_hal_printf("Color [B]: %I16u\n", b);
	tf_hal_printf("Color [C]: %I16u\n", c_);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
