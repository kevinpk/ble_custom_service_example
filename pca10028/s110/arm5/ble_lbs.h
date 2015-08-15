/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup ble_sdk_srv_bas Battery Service
 * @{
 * @ingroup ble_sdk_srv
 * @brief Battery Service module.
 *
 * @details This module implements the Battery Service with the Battery Level characteristic.
 *          During initialization it adds the Battery Service and Battery Level characteristic
 *          to the BLE stack database. Optionally it can also add a Report Reference descriptor
 *          to the Battery Level characteristic (used when including the Battery Service in
 *          the HID service).
 *
 *          If specified, the module will support notification of the Battery Level characteristic
 *          through the ble_lbs_battery_level_update() function.
 *          If an event handler is supplied by the application, the Battery Service will
 *          generate Battery Service events to the application.
 *
 * @note The application must propagate BLE stack events to the Battery Service module by calling
 *       ble_lbs_on_ble_evt() from the @ref softdevice_handler callback.
 *
 * @note Attention! 
 *  To maintain compliance with Nordic Semiconductor ASA Bluetooth profile 
 *  qualification listings, this section of source code must not be modified.
 */

#ifndef ble_lbs_H__
#define ble_lbs_H__
#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

//#define LBS_UUID_BASE {0x21, 0xD9, 0x00, 0x00, 0x23, 0x56, 0x34, 0xF8, 0x0D, 0x59, 0xE6, 0x71, 0x6A, 0xC5, 0x13, 0x2B}  // using nRFgo Studio
#define LBS_UUID_BASE {0xD2, 0xAD, 0xF8, 0x6D, 0xC4, 0x3C, 0x42, 0xFF, 0x9D, 0x44, 0x8A, 0x3E, 0xFD, 0x29, 0xB2, 0xFF}  // using uuidgen
//#define LBS_UUID_BASE {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
#define LBS_UUID_SERVICE 0x1523
#define LBS_UUID_LED_CHAR 0x1525
#define LBS_UUID_BUTTON_CHAR 0x1524


/**@brief Battery Service event type. */
typedef enum
{
    ble_lbs_EVT_NOTIFICATION_ENABLED,                             /**< Battery value notification enabled event. */
    ble_lbs_EVT_NOTIFICATION_DISABLED                             /**< Battery value notification disabled event. */
} ble_lbs_evt_type_t;

/**@brief Battery Service event. */
typedef struct
{
    ble_lbs_evt_type_t evt_type;                                  /**< Type of event. */
} ble_lbs_evt_t;

// Forward declaration of the ble_lbs_t type. 
typedef struct ble_lbs_s ble_lbs_t;

/**@brief Battery Service event handler type. */
typedef void (*ble_lbs_evt_handler_t) (ble_lbs_t * p_bas, ble_lbs_evt_t * p_evt);

typedef void (*ble_lbs_led_write_handler_t) (ble_lbs_t * p_lbs, uint8_t new_state);

/**@brief Battery Service init structure. This contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
	ble_lbs_led_write_handler_t led_write_handler;
} ble_lbs_init_t;

/**@brief Battery Service structure. This contains various status information for the service. */
typedef struct ble_lbs_s
{
	uint16_t										service_handle;
	ble_gatts_char_handles_t		led_char_handles;
	ble_gatts_char_handles_t		button_char_handles;
	uint8_t											uuid_type;
	uint8_t											current_led_state;
	uint16_t										conn_handle;
	ble_lbs_led_write_handler_t led_write_handler;
} ble_lbs_t;

/**@brief Function for initializing the Battery Service.
 *
 * @param[out]  p_bas       Battery Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_bas_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_lbs_init(ble_lbs_t * p_bas, const ble_lbs_init_t * p_bas_init);

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Battery Service.
 *
 * @note For the requirements in the BAS specification to be fulfilled,
 *       ble_lbs_battery_level_update() must be called upon reconnection if the
 *       battery level has changed while the service has been disconnected from a bonded
 *       client.
 *
 * @param[in]   p_bas      Battery Service structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_lbs_on_ble_evt(ble_lbs_t * p_bas, ble_evt_t * p_ble_evt);

/**@brief Function for updating the battery level.
 *
 * @details The application calls this function after having performed a battery measurement. If
 *          notification has been enabled, the battery level characteristic is sent to the client.
 *
 * @note For the requirements in the BAS specification to be fulfilled,
 *       this function must be called upon reconnection if the battery level has changed
 *       while the service has been disconnected from a bonded client.
 *
 * @param[in]   p_bas          Battery Service structure.
 * @param[in]   battery_level  New battery measurement value (in percent of full capacity).
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_lbs_battery_level_update(ble_lbs_t * p_bas, uint8_t battery_level);

uint32_t ble_lbs_init(ble_lbs_t * p_lbs, const ble_lbs_init_t * p_lbs_init);
void ble_lbs_on_ble_evt(ble_lbs_t *p_lbs, ble_evt_t * p_ble_evt);
uint32_t ble_lbs_on_button_change(ble_lbs_t * p_lbs, uint8_t button_state);

#endif // ble_lbs_H__

/** @} */
