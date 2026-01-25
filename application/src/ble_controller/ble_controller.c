#include <stddef.h>
#include <string.h>
#include <errno.h>

#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/logging/log.h>

#include "ble_controller.h"

LOG_MODULE_REGISTER(ble_controller);

// singleton object because the BLE API doesnt allow forwarding of params to the service callbacks
static ble_controller_t ble_ctlr;

// Define the BLE stack init params
// ---
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

static const struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(
	(BT_LE_ADV_OPT_CONN |
	 BT_LE_ADV_OPT_USE_IDENTITY), /* Connectable advertising and use identity address */
	// 800, /* Min Advertising Interval 500ms (800*0.625ms) */
	// 801, /* Max Advertising Interval 500.625ms (801*0.625ms) */
    BT_GAP_ADV_FAST_INT_MIN_1,
    BT_GAP_ADV_FAST_INT_MAX_1,
	NULL); /* Set to NULL for undirected advertising */

static struct k_work advertising_work;
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BLE_CTRLR_BT_UUID_SVC_IMU_VAL),
};

static void advertising_work_handler(struct k_work *work)
{
	int err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));

	if (err) {
		LOG_ERR("advertising failed to start (err %d)", err);
		return;
	}

	LOG_INF("advertising successfully started");
}

static void advertising_start(void)
{
	k_work_submit(&advertising_work);
}

static void on_connection_recycled(void)
{
    LOG_INF("connection object available from previous connection (now disconnected)");
	advertising_start();
}

static void on_connection_connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		LOG_ERR("connection failed (err %u, conn %p)\n", err, conn);
		return;
	}

	LOG_INF("connected (conn %p)", conn);
}

static void on_connection_disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("disconnected (conn %p) with reason %u", conn, reason);
}

struct bt_conn_cb connection_callbacks = {
	.connected = on_connection_connected,
	.disconnected = on_connection_disconnected,
	.recycled = on_connection_recycled,
};

// Instantiate the services and characteristics
// ---
#define BLE_CTRLR_BT_UUID_SVC_IMU                 BT_UUID_DECLARE_128(BLE_CTRLR_BT_UUID_SVC_IMU_VAL)
#define BLE_CTRLR_BT_UUID_SVC_IMU_CHARAC_FW_VERSION BT_UUID_DECLARE_128(BLE_CTRLR_BT_UUID_SVC_IMU_CHARAC_FW_VERSION_VAL)

// Define the characterisitic operations' callbacks
// ---
static ssize_t ble_stack_read_cb_svc_imu_charac_fw_version(
    struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset
)
{
	LOG_DBG("svc: imu, charac: fw_version, read, handle: %u, conn: %p", attr->handle, (void *)conn);

    if(ble_ctlr.fw_version_read_cb)
    {
        uint8_t value;
        bool is_read_good = ble_ctlr.fw_version_read_cb(&value);
        if(is_read_good)
        {
            return bt_gatt_attr_read(conn, attr, buf, len, offset, &value, sizeof(value));
        }
        else
        {
            return BT_GATT_ERR(BT_ATT_ERR_UNLIKELY);
        }
    }

	return 0;
}

// Define the services with their characteristics
// ---
BT_GATT_SERVICE_DEFINE(imu_svc, BT_GATT_PRIMARY_SERVICE(BLE_CTRLR_BT_UUID_SVC_IMU),
    BT_GATT_CHARACTERISTIC(
        BLE_CTRLR_BT_UUID_SVC_IMU_CHARAC_FW_VERSION, 
        BT_GATT_CHRC_READ,
        BT_GATT_PERM_READ, 
        ble_stack_read_cb_svc_imu_charac_fw_version,
        NULL,
        NULL
    )
);

int ble_controller_singleton_init(ble_controller_t *this)
{
    // handle the user cb registration
    if(this) {
        memcpy(&ble_ctlr, this, sizeof(ble_controller_t));
    }

    // start the BLE stack
	int err = bt_enable(NULL);
	if (err) {
		LOG_ERR("bluetooth init failed (err %d)", err);
		return -1;
	}
	bt_conn_cb_register(&connection_callbacks);
 	k_work_init(&advertising_work, advertising_work_handler);
    advertising_start();

    LOG_INF("bluetooth initialized and advertising");

    return 0;
}