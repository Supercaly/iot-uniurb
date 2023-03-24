#include "influxdb.h"
#include "../common.h"
#include "../sensor/DHT11_sensor.h"
#include "../sensor/SGP30_sensor.h"
#include "../sensor/SPS30_sensor.h"
#include "../sensor/MHZ19_sensor.h"

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

InfluxDBClient influxdb_client;
Point influxdb_point(INFLUXDB_POINT_NAME);

bool influxdb_init() {
  Log.traceln("influxdb_init: init influxDB with following parameters: "
              "url '" INFLUXDB_URL "' "
              "org '" INFLUXDB_ORG "' "
              "bucket '" INFLUXDB_BUCKET "'");

  influxdb_client.setConnectionParams(INFLUXDB_URL,
                                      INFLUXDB_ORG,
                                      INFLUXDB_BUCKET,
                                      INFLUXDB_TOKEN,
                                      InfluxDbCloud2CACert);

  influxdb_point.addTag("host", HOST);
  influxdb_point.addTag("location", LOC);
  influxdb_point.addTag("room", ROOM);

  Log.traceln("influxdb_init: added tags: "
              "host '" HOST "'"
              " location '" LOC "'"
              " room '" ROOM "'");

  if (!influxdb_client.validateConnection()) {
    Log.errorln("InfluxDB: connection error: " + influxdb_client.getLastErrorMessage());
    return false;
  }

  Log.debugln("InfluxDB: connected");
  return true;
}

bool influxdb_write_sensors() {
  Log.traceln("influxdb_write_sensors: sending data to influxdb...");

  influxdb_point.clearFields();

  // Add available sensors
  Log.traceln("influxdb_write_sensors: sending DHT11 values");
  influxdb_point.addField(INFLUXDB_FIELD_DHT11_TEMPERATURE, DHT11_get_temperature());
  influxdb_point.addField(INFLUXDB_FIELD_DHT11_HUMIDITY, DHT11_get_humidity());
  Log.traceln("influxdb_write_sensors: sending SGP30 values");
  influxdb_point.addField(INFLUXDB_FIELD_SGP30_TVOC, SGP30_get_TVOC());
  influxdb_point.addField(INFLUXDB_FIELD_SGP30_ECO2, SGP30_get_eCO2());
  influxdb_point.addField(INFLUXDB_FIELD_SGP30_RAW_H2, SGP30_get_raw_H2());
  influxdb_point.addField(INFLUXDB_FIELD_SGP30_RAW_ETHANOL, SGP30_get_raw_ethanol());
  Log.traceln("influxdb_write_sensors: sending SPS30 values");
  Log.traceln("influxdb_write_sensors: sending MHZ19 values");

  Log.debugln("InfluxDB: writing data: " + influxdb_point.toLineProtocol());

  if (!influxdb_client.writePoint(influxdb_point)) {
    Log.errorln("InfluxDB: write failed: " + influxdb_client.getLastErrorMessage());
    delay(INFLUXDB_ON_ERROR_DELAY_MS);
    return false;
  }

  Log.traceln("influxdb_write_sensors: write complete");
  return true;
}
