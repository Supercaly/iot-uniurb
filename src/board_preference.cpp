#include "board_preference.h"

#include <EEPROM.h>
#include <WString.h>
#include <stdint.h>
#include <string.h>

#include "log.h"
#include "sensor/sensor_type.h"

#define _PREF_HAS_SENSOR_BIT(bytes, idx)   (((bytes >> (uint16_t)idx)) & (uint16_t)0x01)
#define _PREF_SET_SENSOR_BIT(bytes, idx)   bytes |= ((uint16_t)0x01 << (uint16_t)idx)
#define _PREF_UNSET_SENSOR_BIT(bytes, idx) bytes &= ~((uint16_t)0x01 << (uint16_t)idx)

BoardPreference Preference;

bool BoardPreference::init() {
  app_traceln("BoardPreference::init: ");
  if (!EEPROM.begin(1000)) {
    app_errorln("BoardPreference::init: cannot initialize EEPROM");
    return false;
  }
  if (!read_preferences()) {
    app_debugln("BoardPreference::init: EEPROM memory is malformed and will be "
                "restored to default state");
    return clear();
  }
  app_infoln("Available Sensors:   " + available_sensors_to_String());
  app_infoln("Device Host Name:   '" + get_board_host_name() + "'");
  app_infoln("Device Location:    '" + get_board_location() + "'");
  app_infoln("Device Room:        '" + get_board_room() + "'");
  app_infoln("Spoofed MAC:        '" + get_spoofed_mac() + "'");
  app_infoln("Temp Offset:        '" + String(get_temperature_offset()) + "'");
  app_infoln("Reboot Count:       '" + String(get_reboot_count()) + "'");

  return true;
}

bool BoardPreference::clear() {
  app_traceln("BoardPreference::clear: restoring preferences to default state");
  _available_sensors_bytes = 0x0;
  _board_host_name         = "";
  _board_location          = "";
  _board_room              = "";
  _spoofed_mac_addr        = "";
  _temperature_offset      = 0;
  _reboot_count            = 0;

  return write_preferences();
}

bool BoardPreference::has_sensor(SensorType s) {
  if (s >= SensorType::COUNT_SENSORS) {
    return false;
  }
  return (bool)_PREF_HAS_SENSOR_BIT(_available_sensors_bytes, s);
}

bool BoardPreference::add_sensor(SensorType s) {
  if (s >= SensorType::COUNT_SENSORS) {
    return false;
  }
  app_traceln("BoardPreference::add_sensor: " + SensorType_to_String(s));
  _PREF_SET_SENSOR_BIT(_available_sensors_bytes, s);
  return write_preferences();
}

bool BoardPreference::remove_sensor(SensorType s) {
  if (s >= SensorType::COUNT_SENSORS) {
    return false;
  }
  app_traceln("BoardPreference::remove_sensor: " + SensorType_to_String(s));
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

bool BoardPreference::set_board_host_name(String name) {
  _board_host_name = name;
  return write_preferences();
}

bool BoardPreference::set_board_location(String location) {
  _board_location = location;
  return write_preferences();
}

bool BoardPreference::set_board_room(String room) {
  _board_room = room;
  return write_preferences();
}

bool BoardPreference::set_spoofed_mac(String mac) {
  _spoofed_mac_addr = mac;
  return write_preferences();
}

bool BoardPreference::set_temperature_offset(int offset) {
  _temperature_offset = (uint8_t)offset;
  return write_preferences();
}

bool BoardPreference::increment_reboot_count() {
  _reboot_count++;
  return write_preferences();
}

bool BoardPreference::clear_reboot_count() {
  _reboot_count = 0;
  return write_preferences();
}

bool BoardPreference::read_preferences() {
  app_traceln("BoardPreference::read_preferences: reading board preferences "
              "form EEPROM");

  int addr = 0;
  // Read header magic number and perform sanity check
  uint32_t magic = EEPROM.readUInt(addr);
  addr += sizeof(uint32_t);
  app_traceln("BoardPreference::read_preferences: header magic number: 0x" + String(magic, HEX));
  if (magic != PREFERENCES_HEADER_MAGIC) {
    app_errorln("BoardPreference::read_preferences: error reading preferences; "
                "expecting header magic number: 0x"
                + String(PREFERENCES_HEADER_MAGIC, HEX) + " but got: 0x" + String(magic, HEX));
    return false;
  }

  // Read stored checksum bytes
  uint16_t old_checksum = EEPROM.readUShort(addr);
  addr += sizeof(uint16_t);

  // Read other preferences
  _available_sensors_bytes = EEPROM.readUShort(addr);
  addr += sizeof(uint16_t);
  app_debugln("BoardPreference::read_preferences: _available_sensors_bytes: 0x"
              + String(_available_sensors_bytes, HEX));
  _board_host_name = EEPROM.readString(addr);
  addr += _board_host_name.length() + 1;
  app_debugln("BoardPreference::read_preferences: _board_host_name: '" + _board_host_name + "'");
  _board_location = EEPROM.readString(addr);
  addr += _board_location.length() + 1;
  app_debugln("BoardPreference::read_preferences: _board_location: '" + _board_location + "'");
  _board_room = EEPROM.readString(addr);
  addr += _board_room.length() + 1;
  app_debugln("BoardPreference::read_preferences: _board_room: '" + _board_room + "'");
  _spoofed_mac_addr = EEPROM.readString(addr);
  addr += _spoofed_mac_addr.length() + 1;
  app_debugln("BoardPreference::read_preferences: _spoofed_mac_addr: '" + _spoofed_mac_addr + "'");
  _temperature_offset = EEPROM.readByte(addr);
  addr += sizeof(uint8_t);
  app_debugln("BoardPreference::read_preferences: _temperature_offset: "
              + String(_temperature_offset));
  _reboot_count = EEPROM.readUShort(addr);
  addr += sizeof(uint16_t);
  app_debugln("BoardPreference::read_preferences: _reboot_count: " + String(_reboot_count));

  // Compute checksum of the parameter just read and check
  // if it's equal to the one stored in memory
  uint16_t new_checksum = checksum();
  if (old_checksum != new_checksum) {
    app_errorln("BoardPreference::read_preferences: checksum is not equal to the one stored ("
                + String(old_checksum) + "!=" + String(new_checksum) + ")");
    return false;
  }
  return true;
}

bool BoardPreference::write_preferences() {
  app_debugln("BoardPreference::write_preferences: writing board preferences "
              "to EEPROM");
  app_trace("BoardPreference::write_preferences: ");
  app_trace("_available_sensors_bytes: '0x" + String(_available_sensors_bytes, HEX) + "', ");
  app_trace("_board_host_name: '" + _board_host_name + "', ");
  app_trace("_board_location: '" + _board_location + "', ");
  app_trace("_board_room: '" + _board_room + "'");
  app_trace("_spoofed_mac_addr: '" + _spoofed_mac_addr + "'");
  app_trace("_temperature_offset: '" + String(_temperature_offset) + "'");
  app_traceln("_reboot_count: '" + String(_reboot_count) + "'");

  int addr = 0;
  // Write header magic number
  EEPROM.writeUInt(addr, PREFERENCES_HEADER_MAGIC);
  addr += sizeof(uint32_t);
  // Write checksum
  EEPROM.writeUShort(addr, checksum());
  addr += sizeof(uint16_t);
  // Write other preferences
  EEPROM.writeUShort(addr, _available_sensors_bytes);
  addr += sizeof(uint16_t);
  EEPROM.writeString(addr, _board_host_name);
  addr += _board_host_name.length() + 1;
  EEPROM.writeString(addr, _board_location);
  addr += _board_location.length() + 1;
  EEPROM.writeString(addr, _board_room);
  addr += _board_room.length() + 1;
  EEPROM.writeString(addr, _spoofed_mac_addr);
  addr += _spoofed_mac_addr.length() + 1;
  EEPROM.writeByte(addr, _temperature_offset);
  addr += sizeof(uint8_t);
  EEPROM.writeUShort(addr, _reboot_count);
  addr += sizeof(uint16_t);

  if (!EEPROM.commit()) {
    app_errorln("BoardPreference::write_preferences: error writing preferences "
                "to EEPROM");
    return false;
  }
  return true;
}

void BoardPreference::create_checksum_prefs_buffer() {
  _checksum_buffer_sz = 0;
  _checksum_buffer_sz += sizeof(_available_sensors_bytes);
  _checksum_buffer_sz += _board_host_name.length();
  _checksum_buffer_sz += _board_location.length();
  _checksum_buffer_sz += _board_room.length();
  _checksum_buffer_sz += _spoofed_mac_addr.length();
  _checksum_buffer_sz += sizeof(_temperature_offset);
  _checksum_buffer_sz += sizeof(_reboot_count);

  if (_checksum_buffer != nullptr) {
    free(_checksum_buffer);
  }
  _checksum_buffer = (uint8_t *)malloc(_checksum_buffer_sz);
  app_traceln("BoardPreference::create_checksum_prefs_buffer: create checksum buffer with size "
              + String(_checksum_buffer_sz));

  size_t address = 0;
  memcpy((_checksum_buffer + address), &_available_sensors_bytes, sizeof(_available_sensors_bytes));
  address += sizeof(_available_sensors_bytes);
  memcpy((_checksum_buffer + address), _board_host_name.c_str(), _board_host_name.length());
  address += _board_host_name.length();
  memcpy((_checksum_buffer + address), _board_location.c_str(), _board_location.length());
  address += _board_location.length();
  memcpy((_checksum_buffer + address), _board_room.c_str(), _board_room.length());
  address += _board_room.length();
  memcpy((_checksum_buffer + address), _spoofed_mac_addr.c_str(), _spoofed_mac_addr.length());
  address += _spoofed_mac_addr.length();
  memcpy((_checksum_buffer + address), &_temperature_offset, sizeof(_temperature_offset));
  address += sizeof(_temperature_offset);
  memcpy((_checksum_buffer + address), &_reboot_count, sizeof(_reboot_count));
  address += sizeof(_reboot_count);
}

uint16_t BoardPreference::checksum() {
  uint8_t a = 1, b = 0;

  // Compute the checksum from the preferences as buffer
  create_checksum_prefs_buffer();
  for (int i = 0; i < _checksum_buffer_sz; i++) {
    a = (a + _checksum_buffer[i]) % UINT8_MAX;
    b = (a + b) % UINT8_MAX;
  }

  uint16_t cs = ((b << 8) | a);
  app_traceln("BoardPreference::checksum: computed checksum '" + String(cs) + "'");
  return cs;
}
