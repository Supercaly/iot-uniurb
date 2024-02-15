#include "board_preference.h"

#include <Arduino.h>
#include <Preferences.h>

#include "log.h"
#include "sensor/sensor.h"

#define PREF_SENSORS_KEY      "sensors"
#define PREF_HOST_NAME_KEY    "board-host"
#define PREF_ROOM_KEY         "board-room"
#define PREF_LOCATION_KEY     "board-loc"
#define PREF_OTA_KEY          "ota-enabled"
#define PREF_TEMP_OFFSET_KEY  "offs-temp"
#define PREF_HUM_OFFSET_KEY   "offs-hum"
#define PREF_CO2_OFFSET_KEY   "offs-co2"
#define PREF_ECO2_OFFSET_KEY  "offs-eco2"
#define PREF_TVOC_OFFSET_KEY  "offs-tvco"
#define PREF_PM10_OFFSET_KEY  "offs-pm10"
#define PREF_REBOOT_COUNT_KEY "reboot-cnt"

#define _PREF_HAS_SENSOR_BIT(bytes, idx)   (((bytes >> (uint16_t)idx)) & (uint16_t)0x01)
#define _PREF_SET_SENSOR_BIT(bytes, idx)   bytes |= ((uint16_t)0x01 << (uint16_t)idx)
#define _PREF_UNSET_SENSOR_BIT(bytes, idx) bytes &= ~((uint16_t)0x01 << (uint16_t)idx)

BoardPreference Preference;

BoardPreference::BoardPreference() {
  _board_info     = new BoardInfo();
  _sensor_offsets = new SensorOffsets();
}

BoardPreference::~BoardPreference() {
  delete _board_info;
  delete _sensor_offsets;
}

bool BoardPreference::init() {
  app_traceln("BoardPreference::init: initializing preferences");

  if (!_prefs.begin("app-prefs")) {
    app_errorln("BoardPreference::init: cannot initialize preferences");
    return false;
  }

  // Read all preferences from memory and store it in variables for easy access
  _available_sensors_bytes     = _prefs.getUShort(PREF_SENSORS_KEY, 0);
  _board_info->host_name       = _prefs.getString(PREF_HOST_NAME_KEY, DEFAULT_BOARD_HOST_NAME);
  _board_info->room            = _prefs.getString(PREF_ROOM_KEY, DEFAULT_BOARD_ROOM);
  _board_info->location        = _prefs.getString(PREF_LOCATION_KEY, DEFAULT_BOARD_ROOM);
  _board_info->ota_enabled     = _prefs.getBool(PREF_OTA_KEY, false);
  _sensor_offsets->temperature = _prefs.getShort(PREF_TEMP_OFFSET_KEY, 0);
  _sensor_offsets->humidity    = _prefs.getShort(PREF_HUM_OFFSET_KEY, 0);
  _sensor_offsets->co2         = _prefs.getShort(PREF_CO2_OFFSET_KEY, 0);
  _sensor_offsets->eco2        = _prefs.getShort(PREF_ECO2_OFFSET_KEY, 0);
  _sensor_offsets->tvoc        = _prefs.getShort(PREF_TVOC_OFFSET_KEY, 0);
  _sensor_offsets->pm10        = _prefs.getShort(PREF_PM10_OFFSET_KEY, 0);
  _reboot_count                = _prefs.getUShort(PREF_REBOOT_COUNT_KEY, 0);

  app_infoln("Board Preferences:");
  app_infoln("  Available Sensors:   " + available_sensors_to_String());
  app_infoln("  Device Host Name:   '" + _board_info->host_name + "'");
  app_infoln("  Device Location:    '" + _board_info->location + "'");
  app_infoln("  Device Room:        '" + _board_info->room + "'");
  app_infoln("  OTA enabled:        '" + String(_board_info->ota_enabled) + "'");
  app_infoln("  Temp Offset:        '" + String(_sensor_offsets->temperature) + "'");
  app_infoln("  Hum Offset:         '" + String(_sensor_offsets->humidity) + "'");
  app_infoln("  CO2 Offset:         '" + String(_sensor_offsets->co2) + "'");
  app_infoln("  eCO2 Offset:        '" + String(_sensor_offsets->eco2) + "'");
  app_infoln("  TVOC Offset:        '" + String(_sensor_offsets->tvoc) + "'");
  app_infoln("  PM 10 Offset:       '" + String(_sensor_offsets->pm10) + "'");
  app_infoln("  Reboot Count:       '" + String(_reboot_count) + "'");

  app_debugln("BoardPreference::init: preferences initialized");
  return true;
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
  app_traceln("BoardPreference::add_sensor: " + sensor_type_to_string(s));
  _PREF_SET_SENSOR_BIT(_available_sensors_bytes, s);
  return _prefs.putUShort(PREF_SENSORS_KEY, _available_sensors_bytes) > 0;
}

bool BoardPreference::remove_sensor(SensorType s) {
  if (s >= SensorType::COUNT_SENSORS) {
    return false;
  }
  app_traceln("BoardPreference::remove_sensor: " + sensor_type_to_string(s));
  _PREF_UNSET_SENSOR_BIT(_available_sensors_bytes, s);
  return _prefs.putUShort(PREF_SENSORS_KEY, _available_sensors_bytes) > 0;
}

String BoardPreference::available_sensors_to_String() {
  String ret = "[";

  bool is_first = true;
  for (int i = 0; i < SensorType::COUNT_SENSORS; ++i) {
    if (has_sensor((SensorType)i)) {
      if (!is_first) {
        ret += ", ";
      }
      ret += sensor_type_to_string((SensorType)i);
      is_first = false;
    }
  }
  ret += "]";
  return ret;
}

bool BoardPreference::get_board_info(BoardInfo *info) {
  info->host_name   = _board_info->host_name;
  info->room        = _board_info->room;
  info->location    = _board_info->location;
  info->ota_enabled = _board_info->ota_enabled;
  return true;
}

bool BoardPreference::set_board_info(BoardInfo info) {
  _board_info->host_name   = info.host_name;
  _board_info->room        = info.room;
  _board_info->location    = info.location;
  _board_info->ota_enabled = info.ota_enabled;
  if (_prefs.putString(PREF_HOST_NAME_KEY, _board_info->host_name) <= 0) {
    return false;
  }
  if (_prefs.putString(PREF_ROOM_KEY, _board_info->room) <= 0) {
    return false;
  }
  if (_prefs.putString(PREF_LOCATION_KEY, _board_info->location) <= 0) {
    return false;
  }
  if (_prefs.putBool(PREF_OTA_KEY, _board_info->ota_enabled) <= 0) {
    return false;
  }
  return true;
}

bool BoardPreference::get_sensor_offsets(SensorOffsets *offsets) {
  offsets->temperature = _sensor_offsets->temperature;
  offsets->humidity    = _sensor_offsets->humidity;
  offsets->co2         = _sensor_offsets->co2;
  offsets->eco2        = _sensor_offsets->eco2;
  offsets->tvoc        = _sensor_offsets->tvoc;
  offsets->pm10        = _sensor_offsets->pm10;
  return true;
}

bool BoardPreference::set_sensor_offsets(SensorOffsets offsets) {
  _sensor_offsets->temperature = offsets.temperature;
  _sensor_offsets->humidity    = offsets.humidity;
  _sensor_offsets->co2         = offsets.co2;
  _sensor_offsets->eco2        = offsets.eco2;
  _sensor_offsets->tvoc        = offsets.tvoc;
  _sensor_offsets->pm10        = offsets.pm10;
  if (_prefs.putShort(PREF_TEMP_OFFSET_KEY, _sensor_offsets->temperature) <= 0) {
    return false;
  }
  if (_prefs.putShort(PREF_HUM_OFFSET_KEY, _sensor_offsets->humidity) <= 0) {
    return false;
  }
  if (_prefs.putShort(PREF_CO2_OFFSET_KEY, _sensor_offsets->co2) <= 0) {
    return false;
  }
  if (_prefs.putShort(PREF_ECO2_OFFSET_KEY, _sensor_offsets->eco2) <= 0) {
    return false;
  }
  if (_prefs.putShort(PREF_TVOC_OFFSET_KEY, _sensor_offsets->tvoc) <= 0) {
    return false;
  }
  if (_prefs.putShort(PREF_PM10_OFFSET_KEY, _sensor_offsets->pm10) <= 0) {
    return false;
  }
  return true;
}

uint16_t BoardPreference::get_reboot_count() {
  return _reboot_count;
}

bool BoardPreference::increment_reboot_count() {
  _reboot_count++;
  return _prefs.putUShort(PREF_REBOOT_COUNT_KEY, _reboot_count) > 0;
}

bool BoardPreference::clear_reboot_count() {
  _reboot_count = 0;
  return _prefs.putUShort(PREF_REBOOT_COUNT_KEY, 0) > 0;
}
