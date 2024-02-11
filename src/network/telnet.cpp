#include "telnet.h"

#include <Arduino.h>
#include <ESPTelnet.h>

#include "../board_preference.h"
#include "../log.h"
#include "../sensor_helper.h"
#include "../utils.h"
#include "../version.h"
#include "wifi.h"

static String string_divide_by(String *s, char delim) {
  size_t i = 0;
  while (i < s->length() && (*s)[i] != delim) {
    i++;
  }
  String result = s->substring(0, i);
  *s            = s->substring(i);
  return result;
}

typedef void (*cmd_on_run)(String cmd);

struct TelnetCommand {
  String     name;
  cmd_on_run run;
  String     help;
};

static ESPTelnet telnet;

static void cmd_sensor(String);
static void cmd_board(String);
static void cmd_offset(String);
static void cmd_reboot_count(String);
static void cmd_uptime(String);
static void cmd_ping(String);
static void cmd_reboot(String);
static void cmd_version(String);
static void cmd_quit(String);
static void cmd_help(String);
static void cmd_sensor_info(String);
static void cmd_sensor_list(String);
static void cmd_sensor_add(String);
static void cmd_sensor_rm(String);
static void cmd_board_host(String);
static void cmd_board_location(String);
static void cmd_board_room(String);
static void cmd_board_mac(String);
static void cmd_offset_temp(String);
static void cmd_offset_hum(String);
static void cmd_offset_co2(String);
static void cmd_offset_eco2(String);
static void cmd_offset_tvoc(String);
static void cmd_offset_pm10(String);
static void cmd_reboot_count_get(String);
static void cmd_reboot_count_clear(String);

static const TelnetCommand commands[] = {
    {"sensor",       cmd_sensor,       "manage the available sensors"               },
    {"board",        cmd_board,        "get or set board's info"                    },
    {"offset",       cmd_offset,       "get or set sensors offsets"                 },
    {"reboot-count", cmd_reboot_count, "get or clear number of reboots"             },
    {"uptime",       cmd_uptime,       "get the time elapsed since the board's boot"},
    {"ping",         cmd_ping,         "ping the board"                             },
    {"reboot",       cmd_reboot,       "reboot the board"                           },
    {"version",      cmd_version,      "show current firmware version"              },
    {"quit",         cmd_quit,         "exit"                                       },
    {"help",         cmd_help,         "show this help message"                     },
};

static const TelnetCommand sensor_commands[] = {
    {"info", cmd_sensor_info, "print the last values of all sensors"},
    {"list", cmd_sensor_list, "list the available sensors"          },
    {"add",  cmd_sensor_add,  "mark a sensor as available"          },
    {"rm",   cmd_sensor_rm,   "mark a sensor as no longer available"},
};

static const TelnetCommand board_commands[] = {
    {"host",     cmd_board_host,     "get or set board's host name"},
    {"location", cmd_board_location, "get or set board's location" },
    {"room",     cmd_board_room,     "get or set board's room"     },
    {"mac",      cmd_board_mac,      "get board's MAC address"     },
};
static const TelnetCommand offset_commands[] = {
    {"temp", cmd_offset_temp, "get or set temperature offset"},
    {"hum",  cmd_offset_hum,  "get or set humidity offset"   },
    {"co2",  cmd_offset_co2,  "get or set CO2 offset"        },
    {"eco2", cmd_offset_eco2, "get or set eCO2 offset"       },
    {"tvoc", cmd_offset_tvoc, "get or set TVOC offset"       },
    {"pm10", cmd_offset_pm10, "get or set PM 10 offset"      },
};

static const TelnetCommand reboot_count_commands[] = {
    {"get",   cmd_reboot_count_get,   "get number of reboots performed by the device"  },
    {"clear", cmd_reboot_count_clear, "reset number of reboots performed by the device"},
};

// Main commands.

static void cmd_sensor(String cmd) {
  cmd.trim();
  String sub_cmd = string_divide_by(&cmd, ' ');
  sub_cmd.trim();
  cmd.trim();
  for (int i = 0; i < size_of_array(sensor_commands); i++) {
    if (sub_cmd == sensor_commands[i].name) {
      sensor_commands[i].run(cmd);
      return;
    }
  }
  telnet.println("sub-command not found: " + cmd);
}

static void cmd_board(String cmd) {
  cmd.trim();
  String sub_cmd = string_divide_by(&cmd, ' ');
  sub_cmd.trim();
  cmd.trim();
  for (int i = 0; i < size_of_array(board_commands); i++) {
    if (sub_cmd == board_commands[i].name) {
      board_commands[i].run(cmd);
      return;
    }
  }
  telnet.println("sub-command not found: " + cmd);
}

static void cmd_offset(String cmd) {
  cmd.trim();
  String sub_cmd = string_divide_by(&cmd, ' ');
  sub_cmd.trim();
  cmd.trim();
  for (int i = 0; i < size_of_array(offset_commands); i++) {
    if (sub_cmd == offset_commands[i].name) {
      offset_commands[i].run(cmd);
      return;
    }
  }
  telnet.println("sub-command not found: " + cmd);
}

static void cmd_reboot_count(String cmd) {
  cmd.trim();
  String sub_cmd = string_divide_by(&cmd, ' ');
  sub_cmd.trim();
  cmd.trim();
  for (int i = 0; i < size_of_array(reboot_count_commands); i++) {
    if (sub_cmd == reboot_count_commands[i].name) {
      reboot_count_commands[i].run(cmd);
      return;
    }
  }
  telnet.println("sub-command not found: " + cmd);
}

static void cmd_uptime(String _) {
  telnet.println(board_uptime());
}

static void cmd_ping(String _) {
  telnet.println("pong");
}

static void cmd_reboot(String _) {
  telnet.println("rebooting board... you will be disconnected");
  telnet.disconnectClient();
  reboot_board(BOARD_REBOOT_DELAY_NOW);
}

static void cmd_version(String _) {
  telnet.println("IoT UniUrb version " VERSION);
}

static void cmd_quit(String _) {
  telnet.println("disconnecting you... bye!");
  telnet.disconnectClient();
}

static void print_command_help(const TelnetCommand *cmd, int sz) {
  for (int i = 0; i < sz; i++) {
    telnet.print("  " + cmd[i].name);
    String spaces;
    for (int s = 0; s < (20 - cmd[i].name.length()); s++) {
      spaces += ' ';
    }
    telnet.print(spaces);
    telnet.println(cmd[i].help);
  }
}

static void cmd_help(String cmd) {
  cmd.trim();
  String sub_cmd = string_divide_by(&cmd, ' ');
  sub_cmd.trim();

  if (sub_cmd.isEmpty()) {
    telnet.println("IoT UniUrb");
    telnet.println("");
    telnet.println("commands:");
    print_command_help(commands, size_of_array(commands));
  } else {
    if (sub_cmd == "sensor") {
      telnet.println("sensor commands:");
      print_command_help(sensor_commands, size_of_array(sensor_commands));
    } else if (sub_cmd == "board") {
      telnet.println("board commands:");
      print_command_help(board_commands, size_of_array(board_commands));
    } else if (sub_cmd == "offset") {
      telnet.println("offset commands:");
      print_command_help(offset_commands, size_of_array(offset_commands));
    } else if (sub_cmd == "reboot-count") {
      telnet.println("reboot count commands:");
      print_command_help(reboot_count_commands, size_of_array(reboot_count_commands));
    } else {
      telnet.println("sub-command not found: " + sub_cmd);
    }
  }
}

// Sensor sub commands.

static void cmd_sensor_info(String _) {
  telnet.println("Values from available sensors:");
  print_available_sensors_info([](String line) { telnet.println(line); });
}

static void cmd_sensor_list(String _) {
  telnet.println(Preference.available_sensors_to_String());
}

static void cmd_sensor_add(String sensor_str) {
  app_traceln("cmd_sensor_add: enable sensor '" + sensor_str + "'");

  sensor_str.trim();
  sensor_str.toUpperCase();
  if (sensor_str.isEmpty()) {
    telnet.println(SENSORS_CMD_ERROR_STR("sensors-add"));
    return;
  }

  SensorType type;
  if (!sensor_type_by_name(sensor_str, &type)) {
    telnet.println("unknown sensor with name '" + sensor_str + "'");
    return;
  }

  if (!Preference.add_sensor(type)) {
    telnet.println("error adding sensor '" + sensor_str + "'");
  }
}

static void cmd_sensor_rm(String sensor_str) {
  app_traceln("cmd_sensor_rm: disable sensor '" + sensor_str + "'");

  sensor_str.trim();
  sensor_str.toUpperCase();
  if (sensor_str.isEmpty()) {
    telnet.println(SENSORS_CMD_ERROR_STR("sensors-rm"));
    return;
  }

  SensorType type;
  if (!sensor_type_by_name(sensor_str, &type)) {
    telnet.println("unknown sensor with name '" + sensor_str + "'");
    return;
  }

  if (!Preference.remove_sensor(type)) {
    telnet.println("error removing sensor '" + sensor_str + "'");
  }
}

// Board sub commands.

static void cmd_board_host(String new_name) {
  new_name.trim();
  BoardInfo bi;
  Preference.get_board_info(&bi);
  if (new_name.isEmpty()) {
    app_traceln("cmd_board_host: want to get the board host name");
    telnet.println(bi.host_name);
  } else {
    app_traceln("cmd_board_host: want to set the board host name to: '" + new_name + "'");
    bi.host_name = new_name;
    Preference.set_board_info(bi);
  }
}

static void cmd_board_location(String new_loc) {
  new_loc.trim();
  BoardInfo bi;
  Preference.get_board_info(&bi);
  if (new_loc.isEmpty()) {
    app_traceln("cmd_board_location: want to get the board location");
    telnet.println(bi.location);
  } else {
    app_traceln("cmd_board_location: want to set the board location to '" + new_loc + "'");
    bi.location = new_loc;
    Preference.set_board_info(bi);
  }
}

static void cmd_board_room(String new_room) {
  new_room.trim();
  BoardInfo bi;
  Preference.get_board_info(&bi);
  if (new_room.isEmpty()) {
    app_traceln("cmd_board_room: want to get the board room");
    telnet.println(bi.room);
  } else {
    app_traceln("cmd_board_room: want to set the board room to '" + new_room + "'");
    bi.room = new_room;
    Preference.set_board_info(bi);
  }
}

static void cmd_board_mac(String _) {
  app_traceln("cmd_mac_address: wants to get current MAC address");
  telnet.println(wifi_get_mac_address());
  return;
}

// Offset sub commands.

static void cmd_offset_temp(String str) {
  str.trim();
  SensorOffsets so;
  Preference.get_sensor_offsets(&so);
  if (str.isEmpty()) {
    app_traceln("cmd_offset_temp: wants to get current temperature offset");
    telnet.println(String(so.temperature));
  } else {
    app_traceln("cmd_offset_temp: wants to set temperature offset");
    so.temperature = (int16_t)str.toInt();
    if (!Preference.set_sensor_offsets(so)) {
      telnet.println("error setting temperature offset");
    }
  }
}

static void cmd_offset_hum(String str) {
  str.trim();
  SensorOffsets so;
  Preference.get_sensor_offsets(&so);
  if (str.isEmpty()) {
    app_traceln("cmd_offset_hum: wants to get current humidity offset");
    telnet.println(String(so.humidity));
  } else {
    app_traceln("cmd_offset_hum: wants to set humidity offset");
    so.humidity = (int16_t)str.toInt();
    if (!Preference.set_sensor_offsets(so)) {
      telnet.println("error setting humidity offset");
    }
  }
}

static void cmd_offset_co2(String str) {
  str.trim();
  SensorOffsets so;
  Preference.get_sensor_offsets(&so);
  if (str.isEmpty()) {
    app_traceln("cmd_offset_co2: wants to get current co2 offset");
    telnet.println(String(so.co2));
  } else {
    app_traceln("cmd_offset_co2: wants to set co2 offset");
    so.co2 = (int16_t)str.toInt();
    if (!Preference.set_sensor_offsets(so)) {
      telnet.println("error setting co2 offset");
    }
  }
}

static void cmd_offset_eco2(String str) {
  str.trim();
  SensorOffsets so;
  Preference.get_sensor_offsets(&so);
  if (str.isEmpty()) {
    app_traceln("cmd_offset_eco2: wants to get current eco2 offset");
    telnet.println(String(so.eco2));
  } else {
    app_traceln("cmd_offset_eco2: wants to set eco2 offset");
    so.eco2 = (int16_t)str.toInt();
    if (!Preference.set_sensor_offsets(so)) {
      telnet.println("error setting eco2 offset");
    }
  }
}

static void cmd_offset_tvoc(String str) {
  str.trim();
  SensorOffsets so;
  Preference.get_sensor_offsets(&so);
  if (str.isEmpty()) {
    app_traceln("cmd_offset_tvoc: wants to get current tvoc offset");
    telnet.println(String(so.tvoc));
  } else {
    app_traceln("cmd_offset_tvoc: wants to set tvoc offset");
    so.tvoc = (int16_t)str.toInt();
    if (!Preference.set_sensor_offsets(so)) {
      telnet.println("error setting tvoc offset");
    }
  }
}

static void cmd_offset_pm10(String str) {
  str.trim();
  SensorOffsets so;
  Preference.get_sensor_offsets(&so);
  if (str.isEmpty()) {
    app_traceln("cmd_offset_pm10: wants to get current pm 10 offset");
    telnet.println(String(so.pm10));
  } else {
    app_traceln("cmd_offset_pm10: wants to set pm 10 offset");
    so.pm10 = (int16_t)str.toInt();
    if (!Preference.set_sensor_offsets(so)) {
      telnet.println("error setting pm 10 offset");
    }
  }
}

// Reboot count sub commands.

static void cmd_reboot_count_get(String _) {
  app_traceln("cmd_reboot_count_get: wants to get reboot count");
  telnet.println(String(Preference.get_reboot_count()));
}

static void cmd_reboot_count_clear(String _) {
  app_traceln("cmd_reboot_count_clear: wants to clear reboot count");
  Preference.clear_reboot_count();
}

// Telnet callbacks.

static void telnet_on_connect_cb(String ip) {
  app_debugln("Telnet: IP '" + ip + "' connected");
  telnet.println("\nWelcome to IoT UniUrb " + telnet.getIP());
  telnet.println("Use help for all commands, quit to disconnect.");
  telnet.print("> ");
}

static void telnet_on_connection_attempt_cb(String ip) {
  app_debugln("Telnet: IP '" + ip + "' is trying to connect");
}

static void telnet_on_reconnect_cb(String ip) {
  app_debugln("Telnet: IP '" + ip + "' reconnected");
}

static void telnet_on_disconnect_cb(String ip) {
  app_debugln("Telnet: IP '" + ip + "' disconnected");
}

static void telnet_on_input_received_cb(String input) {
  String cmd;

  app_debugln("Telnet: new input '" + input + "'");
  // Parse command
  input.trim();
  if (input.isEmpty()) {
    goto telnet_parse_input_defer;
  }

  cmd = string_divide_by(&input, ' ');
  for (int i = 0; i < size_of_array(commands); i++) {
    if (cmd == commands[i].name) {
      commands[i].run(input);
      goto telnet_parse_input_defer;
    }
  }
  telnet.println("command not found: " + cmd);

telnet_parse_input_defer:
  telnet.print("> ");
}

bool telnet_init(int port) {
  app_traceln("telnet_init: init telnet server on port " + String(port));

  telnet.onConnect(telnet_on_connect_cb);
  telnet.onConnectionAttempt(telnet_on_connection_attempt_cb);
  telnet.onReconnect(telnet_on_reconnect_cb);
  telnet.onDisconnect(telnet_on_disconnect_cb);
  telnet.onInputReceived(telnet_on_input_received_cb);

  if (!telnet.begin(port)) {
    app_errorln("telnet_init: init error");
    return false;
  }

  app_infoln("telnet_init: running");
  return true;
}

void telnet_task_code(void *args) {
  TickType_t last_loop_time = xTaskGetTickCount();
  for (;;) {
    xTaskDelayUntil(&last_loop_time, pdMS_TO_TICKS(TELNET_LOOP_DELAY_MS));
    telnet.loop();
  }
}
