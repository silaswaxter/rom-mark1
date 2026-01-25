/** @file
 *  @brief BLE Controller for our application defines our BLE device's characteristics; this module is a singleton adaptor between
 *  the nordic BLE stack and our applcation.
 * 
 *  BLE GATT fundamentals (for reference):
 *
 *  - Attribute table: a flat list of nodes in the GATT database.
 *  - Service: a region in the attribute table grouping related characteristics.
 *  - Characteristic: a cluster of attributes inside a service, typically consisting of:
 *       - Characteristic Declaration attribute
 *       - Characteristic Value attribute
 *       - Optional Descriptor attributes (e.g., Client Characteristic Configuration Descriptor (CCCD) for notifications/indications)
 *
 *  This helps clarify terminology when defining services and characteristics
 *  and understanding how they map to low-level GATT attributes.
 */
#ifndef BLE_CONTROLLER_H
#define BLE_CONTROLLER_H

#include <zephyr/types.h>

// Define the service and characibute UUIDS:
// ---
#define BLE_CTRLR_BT_UUID_SVC_IMU_VAL \
	BT_UUID_128_ENCODE(0x562326d3, 0xa8e8, 0x43c4, 0xb783, 0xdaa2145291f1)

#define BLE_CTRLR_BT_UUID_SVC_IMU_CHARAC_FW_VERSION_VAL \
	BT_UUID_128_ENCODE(0x562326d4, 0xa8e8, 0x43c4, 0xb783, 0xdaa2145291f1)

// Define the callback function types used with service operations
// ---
typedef bool (*ble_controller_svc_imu_charac_fw_version_read_cb_t)(uint8_t *);

// Define this module's interface 
// ---
typedef struct {
    ble_controller_svc_imu_charac_fw_version_read_cb_t fw_version_read_cb;
} ble_controller_t;

/** @brief initialize the ble controller
 *
 * This function registers application callback functions with the BLE
 * controller
 * 
 * @param[in] this Struct containing pointers to callback functions
 *			used. If NULL, no callback functions will be used.
 *
 * @retval 0 If the operation was successful; negative number on error.
 */
int ble_controller_singleton_init(ble_controller_t *this);
	

#endif // BLE_CONTROLLER_H