#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

#define USFSMAX_MMC_I2C_NODE DT_NODELABEL(usfsmax_mmc)

#define SLEEP_TIME_MS 1000

const uint8_t kUSFSMAXMMCFirmwareID = 0x7F;

int main(void)
{
	static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(USFSMAX_MMC_I2C_NODE);

	if(!device_is_ready(dev_i2c.bus)) {
		printk("I2C bus %s is not ready!\n\r", dev_i2c.bus->name);
		return -1;
	}

	uint8_t fw_id = 0;

	while (1) {
		int ret = i2c_write_read_dt(&dev_i2c, &kUSFSMAXMMCFirmwareID, 1, &fw_id, 1); if (ret != 0) {
			printk("Failed to read register %#x\n", kUSFSMAXMMCFirmwareID);
			return -1;
		}

		printk("Firmware ID is %#x\n", fw_id);

		k_msleep(SLEEP_TIME_MS);
	}
}
