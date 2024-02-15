#include "influxdb.h"

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#include "../board_preference.h"
#include "../config.h"
#include "../log.h"
#include "../sensor_helper.h"

static InfluxDBClient influxdb_client;
static Point          influxdb_point(INFLUXDB_POINT_NAME);

bool influxdb_init() {
  app_debugln("influxdb_init: init influxDB");

  app_infoln("InfluxDB url:       '" INFLUXDB_URL "'");
  app_infoln("InfluxDB org:       '" INFLUXDB_ORG "'");
  app_infoln("InfluxDB bucket:    '" INFLUXDB_BUCKET "'");

  influxdb_client.setConnectionParams(
      INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

  BoardInfo bi;
  Preference.get_board_info(&bi);
  influxdb_point.addTag("host", bi.host_name);
  influxdb_point.addTag("location", bi.location);
  influxdb_point.addTag("room", bi.room);

  app_traceln("influxdb_init: added tags host: '" + bi.host_name + "', location: '" + bi.location
              + "', room: '" + bi.room + "'");

  if (!influxdb_client.validateConnection()) {
    app_errorln("influxdb_init: connection error: " + influxdb_client.getLastErrorMessage());
    return false;
  }

  app_debugln("influxdb_init: connected");
  return true;
}

bool influxdb_write_sensors() {
  app_traceln("influxdb_write_sensors: sending data to influxdb");

  influxdb_point.clearFields();

  // Add available sensors
  for (int i = 0; i < COUNT_SENSORS; ++i) {
    if (Preference.has_sensor((SensorType)i)) {
      sensor_type_to_impl[i]->to_influx(&influxdb_point);
    }
  }

  // If the point to write does not have fields it means no sensor is available
  // and we can exit without writing to the server.
  if (!influxdb_point.hasFields()) {
    app_debugln("influxdb_write_sensors: no sensor data to write");
    return true;
  }

  app_traceln("influxdb_write_sensors: writing data: " + influxdb_point.toLineProtocol());
  if (!influxdb_client.writePoint(influxdb_point)) {
    app_errorln("influxdb_write_sensors: write failed: " + influxdb_client.getLastErrorMessage());
    return false;
  }

  app_infoln("influxdb_write_sensors: write complete");
  return true;
}
