#ifndef BOARD_PREFERENCE_H
#define BOARD_PREFERENCE_H

#include <Arduino.h>

#include "sensor/sensor_type.h"

// Header magic number for the preferences.
#define PREFERENCES_HEADER_MAGIC 0x494F5420

// Default values for preferences.
#define DEFAULT_BOARD_HOST_NAME "default_host"
#define DEFAULT_BOARD_LOCATION  "default_location"
#define DEFAULT_BOARD_ROOM      "default_room"

#define _PREF_HAS_SENSOR_BIT(bytes, idx) \
  (((bytes >> (uint16_t)idx)) & (uint16_t)0x01)
#define _PREF_SET_SENSOR_BIT(bytes, idx) \
  bytes |= ((uint16_t)0x01 << (uint16_t)idx)
#define _PREF_UNSET_SENSOR_BIT(bytes, idx) \
  bytes &= ~((uint16_t)0x01 << (uint16_t)idx)

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
  bool has_sensor(SensorType s) {
    if (s >= SensorType::COUNT_SENSORS) {
      return false;
    }
    return (bool)_PREF_HAS_SENSOR_BIT(_available_sensors_bytes, s);
  }

  /*
   * Returns the available sensors in a human-readable
   * format for printing.
   */
  // TODO: When we replace has_sensor we will no longer need this
  String available_sensors_to_String() {
    String ret = "[";

    bool is_first = true;
    for (int i = 0; i < SensorType::COUNT_SENSORS; ++i) {
      if (has_sensor((SensorType)i)) {
        if (!is_first) {
          ret += ", ";
        }
        ret += SensorType_to_String((SensorType)i);
        is_first = false;
      }
    }
    ret += "]";
    return ret;
  }

  /*
   * Mark the given sensor as available.
   *
   * Returns flase if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool add_sensor(SensorType s) {
    if (s >= SensorType::COUNT_SENSORS) {
      return false;
    }
    Log.traceln("BoardPreference::add_sensor: " + SensorType_to_String(s));
    _PREF_SET_SENSOR_BIT(_available_sensors_bytes, s);
    return write_preferences();
  }

  /*
   * Mark the given sensor as no longer available.
   *
   * Returns flase if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool remove_sensor(SensorType s) {
    if (s >= SensorType::COUNT_SENSORS) {
      return false;
    }
    Log.traceln("BoardPreference::remove_sensor: " + SensorType_to_String(s));
    _PREF_UNSET_SENSOR_BIT(_available_sensors_bytes, s);
    return write_preferences();
  }

  /*
   * Returns the board's host name.
   */
  String get_board_host_name() const {
    return _board_host_name.isEmpty() ? DEFAULT_BOARD_HOST_NAME : _board_host_name;
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
    return _board_location.isEmpty() ? DEFAULT_BOARD_LOCATION : _board_location;
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
  String get_board_room() const { return _board_room.isEmpty() ? DEFAULT_BOARD_ROOM : _board_room; }

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

  /*
   * Returns true if the board has enabled
   * MAC address spoofing, false otherwise.
   */
  bool has_spoofed_mac() const { return !_spoofed_mac_addr.isEmpty(); }

  /*
   * Returns the MAC address used for spoofing as
   * a String of six hexidecimal values separated
   * by column.
   *
   * In case MAC address spoofing is not enabled
   * this function returns an empry String.
   */
  String get_spoofed_mac() const { return _spoofed_mac_addr; }

  /*
   * Set the MAC address used for spoofing.
   *
   * The address is passed as a String of six
   * hexidecimal values separated by column.
   *
   * If the given address is empty the MAC address
   * spoofing will be disabled and the connection
   * will use the default one.
   */
  bool set_spoofed_mac(String mac) {
    _spoofed_mac_addr = mac;
    return write_preferences();
  }

  /*
   * Returns the offset used for temperature calibration.
   */
  int8_t get_temperature_offset() const {
    return _temperature_offset;
  }

  /*
   * Set the offset used for temperature calibration.
   */
  bool set_temperatue_offset(int offset) {
    _temperature_offset = (uint8_t)offset;
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
  String   _board_host_name;
  String   _board_location;
  String   _board_room;
  String   _spoofed_mac_addr;
  uint8_t  _temperature_offset = 0;

  bool read_preferences();
  bool write_preferences();
};

/*
 * Global instance of BoardPreference
 */
extern BoardPreference Preference;

#endif // BOARD_PREFERENCE_H
