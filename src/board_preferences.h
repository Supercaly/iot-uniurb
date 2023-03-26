#ifndef BOARD_PREFERENCES_H
#define BOARD_PREFERENCES_H

#include "sensor/sensor_type.h"

#include <Arduino.h>

// Header magic number for the preferences.
#define PREFERENCES_HEADER_MAGIC 0x494F5420

// Default values for preferences.
#define DEFAULT_BOARD_HOST_NAME "default_host"
#define DEFAULT_BOARD_LOCATION "default_location"
#define DEFAULT_BOARD_ROOM "default_room"

/*
 * Init the global preferences.
 */
bool preferences_init();

/*
 * Clear all the preferences stored inthe board
 * restoring them to their default values.
 *
 * Returns flase if the change cannot be applied
 * and leaves the values unchanged.
 */
bool preferences_clear();

/*
 * Returns the available sensors in a
 * human-readable format for printing.
 */
String available_sensors_to_String();

/*
 * Returns true if the given sensor is
 * available; false otherwise.
 */
bool has_sensor(SensorType s);

/*
 * Mark the given sensor as available.
 *
 * Returns flase if the change cannot be applied
 * and leaves the values unchanged.
 */
bool add_available_sensor(SensorType s);

/*
 * Mark the given sensor as no longer available.
 *
 * Returns flase if the change cannot be applied
 * and leaves the values unchanged.
 */
bool remove_available_sensor(SensorType s);

/*
 * Returns the board's host name.
 */
String get_board_host_name();

/*
 * Set the board's host name.
 *
 * Returns flase if the change cannot be applied
 * and leaves the values unchanged.
 */
bool set_board_host_name(String name);

/*
 * Returns the board's location.
 */
String get_board_location();

/*
 * Set the board's location.
 *
 * Returns flase if the change cannot be applied
 * and leaves the values unchanged.
 */
bool set_board_location(String location);

/*
 * Returns the bord's room.
 */
String get_board_room();

/*
 * Set the board's room.
 *
 * Returns flase if the change cannot be applied
 * and leaves the values unchanged.
 */
bool set_board_room(String room);

#endif  // BOARD_PREFERENCES_H
