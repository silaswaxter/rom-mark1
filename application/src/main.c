#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define USFSMAX_MMC5983_NODE DT_NODELABEL(usfsmax_mmc5983)

#define SLEEP_TIME_MS 1000
#define RESET_INTERVAL_MS 10000  // reset every 10 seconds
#define RESET_PULSE_MS 10        // hold reset low for 10 ms

const uint8_t kUSFSMAXMMCFirmwareID = 0x7F;

int main(void)
{
	printk("Initializing Application...\n");

	static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(USFSMAX_MMC5983_NODE);
	static const struct gpio_dt_spec reset_gpio = GPIO_DT_SPEC_GET(USFSMAX_MMC5983_NODE, reset_gpios);

	if (!device_is_ready(dev_i2c.bus)) {
		printk("I2C bus %s is not ready!\n", dev_i2c.bus->name);
		return -1;
	}

	if (!device_is_ready(reset_gpio.port)) {
		printk("Reset GPIO device not ready!\n");
		return -1;
	}

	int ret = gpio_pin_configure_dt(&reset_gpio, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		printk("Failed to configure reset GPIO (err %d)\n", ret);
		return -1;
	}

	uint8_t fw_id = 0;
	int64_t last_reset_time = k_uptime_get();

	printk("Entering Application Main Loop...\n");

	while (1) {
		/* Check if it's time to reset */
		int64_t now = k_uptime_get();
		if (now - last_reset_time >= RESET_INTERVAL_MS) {
			printk("Resetting device...\n");

			gpio_pin_set_dt(&reset_gpio, 1);  // 1 means active. (active_low vs active_high taken care of in DT)
			k_msleep(RESET_PULSE_MS);
			gpio_pin_set_dt(&reset_gpio, 0);
			k_msleep(RESET_PULSE_MS);
			gpio_pin_set_dt(&reset_gpio, 1);

			last_reset_time = now;
			printk("Device reset complete.\n");
		}

		/* Regular I2C read */
		printk("Application Main Loop Heartbeat...\n");

		ret = i2c_write_read_dt(&dev_i2c, &kUSFSMAXMMCFirmwareID, 1, &fw_id, 1);
		if (ret != 0) {
			printk("Failed to read register %#x (err %d)\n", kUSFSMAXMMCFirmwareID, ret);
		} else {
			printk("Firmware ID is %#x\n", fw_id);
		}

		k_msleep(SLEEP_TIME_MS);
	}
}