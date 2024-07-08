#include "RobotControlService.h"

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>


LOG_MODULE_DECLARE(BLE_comm);


static ssize_t write_speed(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf,
			 uint16_t len, uint16_t offset, uint8_t flags)
{
	LOG_DBG("Attribute write, handle: %u, conn: %p", attr->handle, (void *)conn);

	if (len != 1U) {
		LOG_DBG("Write led: Incorrect data length");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	if (offset != 0) {
		LOG_DBG("Write led: Incorrect data offset");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	uint8_t val = *((uint8_t *)buf);
    LOG_INF("write_speed: received value %d", val);

	return len;
}

static ssize_t write_steering(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf,
			 uint16_t len, uint16_t offset, uint8_t flags)
{
	LOG_DBG("Attribute write, handle: %u, conn: %p", attr->handle, (void *)conn);

	if (len != 1U) {
		LOG_DBG("Write led: Incorrect data length");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	if (offset != 0) {
		LOG_DBG("Write led: Incorrect data offset");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	uint8_t val = *((uint8_t *)buf);
    LOG_INF("write_steering: received value %d", val);

	return len;
}


BT_GATT_SERVICE_DEFINE(
	rcs_svc, BT_GATT_PRIMARY_SERVICE(BT_UUID_RCS),
	BT_GATT_CHARACTERISTIC(BT_UUID_RCS_SPEED, BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE, NULL,
			       write_speed, NULL),
	BT_GATT_CHARACTERISTIC(BT_UUID_RCS_STEERING, BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE, NULL,
			       write_steering, NULL),
);
