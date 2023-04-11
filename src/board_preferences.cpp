#include "board_preferences.h"
#include "log.h"

#include <assert.h>
#include <EEPROM.h>

#define _PREF_HAS_SENSOR_BIT(bytes, idx) \
  (((bytes >> (uint16_t)idx)) & (uint16_t)0x01)
#define _PREF_SET_SENSOR_BIT(bytes, idx) \
  bytes |= ((uint16_t)0x01 << (uint16_t)idx)
#define _PREF_UNSET_SENSOR_BIT(bytes, idx) \
  bytes &= ~((uint16_t)0x01 << (uint16_t)idx)

BoardPreference Preference;

bool BoardPreference::init() {
  Log.traceln("BoardPreference::init: ");
  if (!EEPROM.begin(1000)) {
    Log.errorln("BoardPreference::init: cannot initialize EEPROM");
    return false;
  }
  if (!read_preferences()) {
    Log.debugln("BoardPreference::init: EEPROM memory is malformed and will be restored to default state");
    return clear();
  }
  return true;
}

bool BoardPreference::clear() {
  Log.traceln("BoardPreference::clear: restoring preferences to default state");
  _available_sensors_bytes = 0x0;
  _board_host_name = "";
  _board_location = "";
  _board_room = "";
  return write_preferences();
}

bool BoardPreference::has_sensor(SensorType s) {
  assert(s < SensorType::COUNT_SENSORS);
  return (bool)_PREF_HAS_SENSOR_BIT(_available_sensors_bytes, s);
}

bool BoardPreference::add_sensor(SensorType s) {
  assert(s < SensorType::COUNT_SENSORS);
  Log.traceln("BoardPreference::add_sensor: " + SensorType_to_String(s));
  _PREF_SET_SENSOR_BIT(_available_sensors_bytes, s);
  return write_preferences();
}

bool BoardPreference::remove_sensor(SensorType s) {
  assert(s < SensorType::COUNT_SENSORS);
  Log.traceln("BoardPreference::remove_sensor: " + SensorType_to_String(s));
  _PREF_UNSET_SENSOR_BIT(_available_sensors_bytes, s);
  return write_preferences();
}

String BoardPreference::available_sensors_to_String() {
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

bool BoardPreference::read_preferences() {
  Log.traceln("BoardPreference::read_preferences: reading board preferences form EEPROM");

  int addr = 0;
  // Read header magic number and perform sanity check
  uint32_t magic = EEPROM.readUInt(addr);
  addr += sizeof(uint32_t);
  Log.debugln("BoardPreference::read_preferences: header magic number: 0x" + String(magic, HEX));
  if (magic != PREFERENCES_HEADER_MAGIC) {
    Log.errorln("BoardPreference::read_preferences: error reading preferences; "
                "expecting header magic number: 0x"
                + String(PREFERENCES_HEADER_MAGIC, HEX)
                + " but got: 0x" + String(magic, HEX));
    return false;
  }
  // Read other preferences
  _available_sensors_bytes = EEPROM.readUShort(addr);
  addr += sizeof(uint16_t);
  Log.traceln("BoardPreference::read_preferences: _available_sensors_bytes: 0x" + String(_available_sensors_bytes, HEX));
  _board_host_name = EEPROM.readString(addr);
  addr += _board_host_name.length() + 1;
  Log.traceln("BoardPreference::read_preferences: _board_host_name: '" + _board_host_name + "'");
  _board_location = EEPROM.readString(addr);
  addr += _board_location.length() + 1;
  Log.traceln("BoardPreference::read_preferences: _board_location: '" + _board_location + "'");
  _board_room = EEPROM.readString(addr);
  Log.traceln("BoardPreference::read_preferences: _board_room: '" + _board_room + "'");

  return true;
}

bool BoardPreference::write_preferences() {
  Log.traceln("BoardPreference::write_preferences: writing board preferences to EEPROM");
  Log.traceln("BoardPreference::write_preferences: _available_sensors_bytes: '0x" + String(_available_sensors_bytes, HEX) + "', "
              + "_board_host_name: '" + _board_host_name + "', "
              + "_board_location: '" + _board_location + "', "
              + "_board_room: '" + _board_room + "'");

  int addr = 0;
  // Write header magic number
  EEPROM.writeUInt(addr, PREFERENCES_HEADER_MAGIC);
  addr += sizeof(uint32_t);
  // Write other preferences
  EEPROM.writeUShort(addr, _available_sensors_bytes);
  addr += sizeof(uint16_t);
  EEPROM.writeString(addr, _board_host_name);
  addr += _board_host_name.length() + 1;
  EEPROM.writeString(addr, _board_location);
  addr += _board_location.length() + 1;
  EEPROM.writeString(addr, _board_room);

  if (!EEPROM.commit()) {
    Log.errorln("BoardPreference::write_preferences: error writing preferences to EEPROM");
    return false;
  }
  return true;
}
