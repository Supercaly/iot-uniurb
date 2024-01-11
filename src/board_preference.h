#ifndef BOARD_PREFERENCE_H
#define BOARD_PREFERENCE_H

#include <WString.h>
#include <stdint.h>

#include "sensor/sensor_type.h"

// Header magic number for the preferences.
#define PREFERENCES_HEADER_MAGIC 0x494F5420

// Default values for preferences.
#define DEFAULT_BOARD_HOST_NAME "default_host"
#define DEFAULT_BOARD_LOCATION  "default_location"
#define DEFAULT_BOARD_ROOM      "default_room"

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
   * Returns false if the change cannot be applied
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
   * Returns false if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool add_sensor(SensorType s);

  /*
   * Mark the given sensor as no longer available.
   *
   * Returns false if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool remove_sensor(SensorType s);

  /*
   * Returns the board's host name.
   */
  String get_board_host_name() const {
    return _board_host_name.isEmpty() ? DEFAULT_BOARD_HOST_NAME : _board_host_name;
  }

  /*
   * Set the board's host name.
   *
   * Returns false if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool set_board_host_name(String name);

  /*
   * Returns the board's location.
   */
  String get_board_location() const {
    return _board_location.isEmpty() ? DEFAULT_BOARD_LOCATION : _board_location;
  }

  /*
   * Set the board's location.
   *
   * Returns false if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool set_board_location(String location);

  /*
   * Returns the board's room.
   */
  String get_board_room() const { return _board_room.isEmpty() ? DEFAULT_BOARD_ROOM : _board_room; }

  /*
   * Set the board's room.
   *
   * Returns false if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool set_board_room(String room);

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
   * this function returns an empty String.
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
  bool set_spoofed_mac(String mac);

  /*
   * Return the temperature offset.
   */
  int8_t get_temperature_offset() const { return _temperature_offset; }

  /*
   * Set the temperature offset.
   *
   * Returns false is the change cannot be applied
   * and leaves the values unchanged.
   */
  bool set_temperature_offset(int offset);

  /*
   * Return the number of reboots performed by the board.
   */
  uint16_t get_reboot_count() const { return _reboot_count; }

  /*
   * Increment the boards reboot count.
   */
  bool increment_reboot_count();

  /*
   * Clears the boards reboot count.
   */
  bool clear_reboot_count();

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
  uint16_t _reboot_count       = 0;

  // Checksum internal buffer.
  uint8_t *_checksum_buffer    = nullptr;
  size_t   _checksum_buffer_sz = 0;

  bool read_preferences();
  bool write_preferences();

  /*
   * Compute the checksum from all the preferences
   * stored inside the board's EEPROM.
   *
   * This checksum function implements a 16bit version
   * of the Adler-32 algorithm.
   *
   * Returns the computed checksum as a 16bit unsigned int.
   */
  uint16_t checksum();
  void     create_checksum_prefs_buffer();
};

/*
 * Global instance of BoardPreference
 */
extern BoardPreference Preference;

#endif // BOARD_PREFERENCE_H
