#ifndef BOARD_PREFERENCE_H
#define BOARD_PREFERENCE_H

#include <Arduino.h>
#include <Preferences.h>

#include "sensor/sensor_type.h"

// Default values for preferences.
#define DEFAULT_BOARD_HOST_NAME "default_host"
#define DEFAULT_BOARD_LOCATION  "default_location"
#define DEFAULT_BOARD_ROOM      "default_room"

/*
 * Class containing all the info
 * of the board
 */
struct BoardInfo {
  String host_name;
  String room;
  String location;
};

/*
 * Class containing the
 * offset of each sensor
 */
struct SensorOffsets {
  int16_t temperature;
  int16_t humidity;
  int16_t co2;
  int16_t eco2;
  int16_t tvoc;
  int16_t pm10;
};

/*
 * Class representing board's global preferences.
 */
class BoardPreference {
  public:
  BoardPreference();
  ~BoardPreference();

  /*
   * Init the global preferences.
   */
  bool init();

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
   * Get the current board infos.
   */
  bool get_board_info(BoardInfo *info);

  /*
   * Set the board info to the given ones.
   *
   * Returns false if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool set_board_info(BoardInfo info);

  /*
   * Get the current sensors offsets.
   */
  bool get_sensor_offsets(SensorOffsets *offsets);

  /*
   * Set the sensors offsets to the given ones.
   *
   * Returns false if the change cannot be applied
   * and leaves the values unchanged.
   */
  bool set_sensor_offsets(SensorOffsets offsets);

  /*
   * Get the number of reboots of the board.
   */
  uint16_t get_reboot_count();

  /*
   * Increment the counter of reboots of the board.
   */
  bool increment_reboot_count();

  /*
   * Reset to zero the counter of reboots of the board.
   */
  bool clear_reboot_count();

  private:
  Preferences _prefs;

  /*
   * Available sensors are represented by 2 bytes (uint16_t)
   * so we can have a total of 16 different sensors.
   * Each bit of this two bytes is connected with the presence
   * of a particular sensor (the index depends on the SensorType enum order)
   * and if the bit is set it means that the sensor is present.
   */
  uint16_t       _available_sensors_bytes = 0;
  BoardInfo     *_board_info;
  SensorOffsets *_sensor_offsets;
  uint16_t       _reboot_count = 0;
};

/*
 * Global instance of BoardPreference
 */
extern BoardPreference Preference;

#endif // BOARD_PREFERENCE_H
