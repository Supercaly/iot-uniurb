#include "board_preferences.h"
#include "log.h"

#include <assert.h>
#include <EEPROM.h>

#define _PREF_HAS_SENSOR_BIT(idx) (available_sensors_bytes >> (uint16_t)idx) & (uint16_t)0x01
#define _PREF_SET_SENSOR_BIT(idx) available_sensors_bytes |= (uint16_t)0x01 << (uint16_t)idx
#define _PREF_UNSET_SENSOR_BIT(idx) available_sensors_bytes &= ~((uint16_t)0x01 << (uint16_t)idx)

/*
 * Available sensors are represented by 2 bytes (uint16_t)
 * so we can have a total of 16 different sensors.
 * Each bit of this two bytes is connected with the presence
 * of a particular sensor (the index depends on the SensorType enum order)
 * and if the bit is set it means that the sensor is present.
 */
uint16_t available_sensors_bytes = 0;
String board_host_name,
  board_location,
  board_room;

static bool read_board_preferences() {
  Log.traceln("read_board_preferences: reading board preferences form EEPROM");

  int addr = 0;
  // Read header magic number and perform sanity check
  uint32_t magic = EEPROM.readUInt(addr);
  addr += sizeof(uint32_t);
  Log.debugln("read_board_preferences: header magic number: 0x" + String(magic, HEX));
  if (magic != PREFERENCES_HEADER_MAGIC) {
    Log.errorln("read_board_preferences: cannot find matching preferences header "
                "expecting header magic: 0x"
                + String(PREFERENCES_HEADER_MAGIC, HEX)
                + " but got: 0x" + String(magic, HEX));
    return false;
  }
  // Read other preferences
  available_sensors_bytes = EEPROM.readUShort(addr);
  addr += sizeof(uint16_t);
  Log.traceln("read_board_preferences: available_sensors_bytes: 0x" + String(available_sensors_bytes, HEX));
  board_host_name = EEPROM.readString(addr);
  addr += board_host_name.length() + 1;
  Log.traceln("read_board_preferences: board_host_name: '" + board_host_name + "'");
  board_location = EEPROM.readString(addr);
  addr += board_location.length() + 1;
  Log.traceln("read_board_preferences: board_location: '" + board_location + "'");
  board_room = EEPROM.readString(addr);
  Log.traceln("read_board_preferences: board_room: '" + board_room + "'");

  return true;
}

static bool write_board_preferences() {
  Log.traceln("write_board_preferences: writing board preferences to EEPROM");
  Log.traceln("write_board_preferences: available_sensors_bytes: '0x" + String(available_sensors_bytes, HEX) + "', "
              + "board_host_name: '" + board_host_name + "', "
              + "board_location: '" + board_location + "', "
              + "board_room: '" + board_room + "'");

  int addr = 0;
  // Write header magic number
  EEPROM.writeUInt(addr, PREFERENCES_HEADER_MAGIC);
  addr += sizeof(uint32_t);
  // Write other preferences
  EEPROM.writeUShort(addr, available_sensors_bytes);
  addr += sizeof(uint16_t);
  EEPROM.writeString(addr, board_host_name);
  addr += board_host_name.length() + 1;
  EEPROM.writeString(addr, board_location);
  addr += board_location.length() + 1;
  EEPROM.writeString(addr, board_room);

  if (!EEPROM.commit()) {
    Log.errorln("write_board_preferences: error writing preferences to EEPROM");
    return false;
  }
  return true;
}

bool preferences_init() {
  Log.traceln("preferences_init: ");
  if (!EEPROM.begin(1000)) {
    Log.errorln("preferences_init: cannot initialize EEPROM");
    return false;
  }
  if (!read_board_preferences()) {
    Log.debugln("preferences_init: EEPROM memory is malformed and will be restored to default state");
    return preferences_clear();
  }
  return true;
}

bool preferences_clear() {
  Log.traceln("preferences_clear: restoring preferences to default state");
  available_sensors_bytes = 0x0;
  board_host_name = "";
  board_location = "";
  board_room = "";
  return write_board_preferences();
}

bool has_sensor(SensorType s) {
  assert(s < SensorType::COUNT_SENSORS);
  return (bool)_PREF_HAS_SENSOR_BIT(s);
}

bool add_available_sensor(SensorType s) {
  assert(s < SensorType::COUNT_SENSORS);
  Log.traceln("add_available_sensor: add sensor " + SensorType_to_String(s));
  _PREF_SET_SENSOR_BIT(s);
  return write_board_preferences();
}

bool remove_available_sensor(SensorType s) {
  assert(s < SensorType::COUNT_SENSORS);
  Log.traceln("remove_available_sensor: remove sensor " + SensorType_to_String(s));
  _PREF_UNSET_SENSOR_BIT(s);
  return write_board_preferences();
}

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

String get_board_host_name() {
  return board_host_name.isEmpty() ? DEFAULT_BOARD_HOST_NAME : board_host_name;
}

bool set_board_host_name(String name) {
  board_host_name = name;
  return write_board_preferences();
}

String get_board_location() {
  return board_location.isEmpty() ? DEFAULT_BOARD_LOCATION : board_location;
}

bool set_board_location(String location) {
  board_location = location;
  return write_board_preferences();
}

String get_board_room() {
  return board_room.isEmpty() ? DEFAULT_BOARD_ROOM : board_room;
}

bool set_board_room(String room) {
  board_room = room;
  return write_board_preferences();
}
