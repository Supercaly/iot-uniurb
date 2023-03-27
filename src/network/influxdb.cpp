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
  Log.debugln("influxdb_init: init influxDB with following parameters: "
              "url: '" INFLUXDB_URL "', "
              "org: '" INFLUXDB_ORG "', "
              "bucket: '" INFLUXDB_BUCKET "'");

  influxdb_client.setConnectionParams(INFLUXDB_URL,
                                      INFLUXDB_ORG,
                                      INFLUXDB_BUCKET,
                                      INFLUXDB_TOKEN,
                                      InfluxDbCloud2CACert);

  influxdb_point.addTag("host", get_board_host_name());
  influxdb_point.addTag("location", get_board_location());
  influxdb_point.addTag("room", get_board_room());

  Log.traceln("influxdb_init: added tags: host: '" + get_board_host_name()
              + "', location: '" + get_board_location()
              + "', room: '" + get_board_room() + "'");

  if (!influxdb_client.validateConnection()) {
    Log.errorln("influxdb_init: connection error: " + influxdb_client.getLastErrorMessage());
    return false;
  }

  Log.debugln("influxdb_init: connected");
  return true;
}

bool influxdb_write_sensors() {
  Log.traceln("influxdb_write_sensors: sending data to influxdb...");

  influxdb_point.clearFields();

  // Add available sensors
  if (has_sensor(SensorType::SENSOR_DHT11)) {
    Log.traceln("influxdb_write_sensors: sending DHT11 values");
    influxdb_point.addField(INFLUXDB_FIELD_DHT11_TEMPERATURE, DHT11_get_temperature());
    influxdb_point.addField(INFLUXDB_FIELD_DHT11_HUMIDITY, DHT11_get_humidity());
  }
  if (has_sensor(SensorType::SENSOR_SGP30)) {
    Log.traceln("influxdb_write_sensors: sending SGP30 values");
    influxdb_point.addField(INFLUXDB_FIELD_SGP30_TVOC, SGP30_get_TVOC());
    influxdb_point.addField(INFLUXDB_FIELD_SGP30_ECO2, SGP30_get_eCO2());
    influxdb_point.addField(INFLUXDB_FIELD_SGP30_RAW_H2, SGP30_get_raw_H2());
    influxdb_point.addField(INFLUXDB_FIELD_SGP30_RAW_ETHANOL, SGP30_get_raw_ethanol());
  }
  if (has_sensor(SensorType::SENSOR_SPS30)) {
    Log.traceln("influxdb_write_sensors: sending SPS30 values");
    influxdb_point.addField(INFLUXDB_FIELD_SPS30_MC_1p0, SPS30_get_mc1p0());
    influxdb_point.addField(INFLUXDB_FIELD_SPS30_MC_2p5, SPS30_get_mc2p5());
    influxdb_point.addField(INFLUXDB_FIELD_SPS30_MC_4p0, SPS30_get_mc4p0());
    influxdb_point.addField(INFLUXDB_FIELD_SPS30_MC_10p0, SPS30_get_mc10p0());
    influxdb_point.addField(INFLUXDB_FIELD_SPS30_PARTICLE_SIZE, SPS30_get_particle_size());
  }
  if (has_sensor(SensorType::SENSOR_MHZ19)) {
    Log.traceln("influxdb_write_sensors: sending MHZ19 values");
    influxdb_point.addField(INFLUXDB_FIELD_MHZ19_CO2, MHZ19_get_co2());
    influxdb_point.addField(INFLUXDB_FIELD_MHZ19_MIN_CO2, MHZ19_get_minCo2());
    influxdb_point.addField(INFLUXDB_FIELD_MHZ19_TEMPERATURE, MHZ19_get_temperature());
    influxdb_point.addField(INFLUXDB_FIELD_MHZ19_ACCURACY, MHZ19_get_accuracy());
  }

  // If the point to write does not have fields it means no sensor is available
  // and we can exit without writing to the server.
  if (!influxdb_point.hasFields()) {
    Log.debugln("influxdb_write_sensors: no sensor data to write");
    return true;
  }

  Log.debugln("influxdb_write_sensors: writing data: " + influxdb_point.toLineProtocol());
  if (!influxdb_client.writePoint(influxdb_point)) {
    Log.errorln("influxdb_write_sensors: write failed: " + influxdb_client.getLastErrorMessage());
    return false;
  }

  Log.traceln("influxdb_write_sensors: write complete");
  return true;
}
