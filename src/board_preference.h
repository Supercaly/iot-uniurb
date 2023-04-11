#ifndef BOARD_PREFERENCE_H
#define BOARD_PREFERENCE_H

#include "sensor/sensor_type.h"

#include <Arduino.h>

// Header magic number for the preferences.
#define PREFERENCES_HEADER_MAGIC 0x494F5420

// Default values for preferences.
#define DEFAULT_BOARD_HOST_NAME "default_host"
#define DEFAULT_BOARD_LOCATION "default_location"
#define DEFAULT_BOARD_ROOM "default_room"

/*
 * Class representing board's global preferences.
 *
 * Those preferences are stored inside the board's
 * EEPROM and are read and write each time they are updated.
 */
class BoardPreference {
public:
  /*
   * Init the global preferences.
   */
  bool init();

  /*
   * Clear all the preferences stored inside the board
   * restoring them to their default values.
   *
   * Returns flase if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool clear();

  /*
   * Returns true if the given sensor is available;
   * false otherwise.
   */
  // TODO: Replace has_sensor with a method returning a list of sensors
  bool has_sensor(SensorType s);

  /*
   * Returns the available sensors in a human-readable
   * format for printing.
   */
  // TODO: When we replace has_sensor we will no longer need this
  String available_sensors_to_String();

  /*
   * Mark the given sensor as available.
   *
   * Returns flase if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool add_sensor(SensorType s);

  /*
   * Mark the given sensor as no longer available.
   *
   * Returns flase if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool remove_sensor(SensorType s);

  /*
   * Returns the board's host name.
   */
  String get_board_host_name() const {
    return _board_host_name.isEmpty()
             ? DEFAULT_BOARD_HOST_NAME
             : _board_host_name;
  }

  /*
   * Set the board's host name.
   *
   * Returns flase if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool set_board_host_name(String name) {
    _board_host_name = name;
    return write_preferences();
  }

  /*
   * Returns the board's location.
   */
  String get_board_location() const {
    return _board_location.isEmpty()
             ? DEFAULT_BOARD_LOCATION
             : _board_location;
  }

  /*
   * Set the board's location.
   *
   * Returns flase if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool set_board_location(String location) {
    _board_location = location;
    return write_preferences();
  }

  /*
   * Returns the bord's room.
   */
  String get_board_room() const {
    return _board_room.isEmpty()
             ? DEFAULT_BOARD_ROOM
             : _board_room;
  }

  /*
   * Set the board's room.
   *
   * Returns flase if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool set_board_room(String room) {
    _board_room = room;
    return write_preferences();
  }

private:
  /*
   * Available sensors are represented by 2 bytes (uint16_t)
   * so we can have a total of 16 different sensors.
   * Each bit of this two bytes is connected with the presence
   * of a particular sensor (the index depends on the SensorType enum order)
   * and if the bit is set it means that the sensor is present.
   */
  uint16_t _available_sensors_bytes = 0;
  String _board_host_name;
  String _board_location;
  String _board_room;

  bool read_preferences();
  bool write_preferences();
};

/*
 * Global instance of BoardPreference
 */
extern BoardPreference Preference;

#endif  // BOARD_PREFERENCE_H
