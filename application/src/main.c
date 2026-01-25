#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>

#include "ble_controller.h"

#define USFSMAX_MMC5983_NODE DT_NODELABEL(usfsmax_mmc5983)

#define SLEEP_TIME_MS 1000
#define RESET_INTERVAL_MS 10000  // reset every 10 seconds
#define RESET_PULSE_MS 10        // hold reset low for 10 ms

LOG_MODULE_REGISTER(main);

const uint8_t kUSFSMAXMMCFirmwareID = 0x7F;

uint8_t last_read_fw_id = 0;
bool is_last_read_fw_id_valid = false;

bool ble_fw_version_read_cb(uint8_t* value)
{
	if(is_last_read_fw_id_valid)
	{
		*value = last_read_fw_id;
		return true;
	}

	return false;
}

int main(void)
{
	LOG_INF("initialization starting");

	static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(USFSMAX_MMC5983_NODE);
	static const struct gpio_dt_spec reset_gpio = GPIO_DT_SPEC_GET(USFSMAX_MMC5983_NODE, reset_gpios);

	if (!device_is_ready(dev_i2c.bus)) {
		LOG_ERR("I2C bus %s is not ready", dev_i2c.bus->name);
		return -1;
	}

	if (!device_is_ready(reset_gpio.port)) {
		LOG_ERR("Reset GPIO device not ready");
		return -1;
	}

	int ret = gpio_pin_configure_dt(&reset_gpio, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		LOG_ERR("Failed to configure reset GPIO (err %d)", ret);
		return -1;
	}

	uint8_t fw_id = 0;
	int64_t last_reset_time = k_uptime_get();

	ble_controller_t ble_controller;
	ble_controller.fw_version_read_cb = ble_fw_version_read_cb;
	ble_controller_singleton_init(&ble_controller);

	LOG_INF("initialization complete");
	LOG_INF("entering main loop");

	while (1) {
		LOG_DBG("main loop heartbeat");

		/* Check if it's time to reset */
		int64_t now = k_uptime_get();
		if (now - last_reset_time >= RESET_INTERVAL_MS) {
			LOG_INF("preparing to reset device");

			gpio_pin_set_dt(&reset_gpio, 1);  // 1 means active. (active_low vs active_high taken care of in DT)
			k_msleep(RESET_PULSE_MS);
			gpio_pin_set_dt(&reset_gpio, 0);
			k_msleep(RESET_PULSE_MS);
			gpio_pin_set_dt(&reset_gpio, 1);

			last_reset_time = now;
			LOG_INF("device reset complete");
		}

		/* Regular I2C read */
		ret = i2c_write_read_dt(&dev_i2c, &kUSFSMAXMMCFirmwareID, 1, &fw_id, 1);
		if (ret != 0) {
			LOG_ERR("failed to read register %#x (err %d)", kUSFSMAXMMCFirmwareID, ret);
		} else {
			LOG_INF("firmware ID is %#x", fw_id);
		}

		k_msleep(SLEEP_TIME_MS);
	}
}