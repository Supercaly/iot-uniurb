#include "influxdb.h"

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#include "../common.h"
#include "../sensor/DHT11_sensor.h"
#include "../sensor/MHZ19_sensor.h"
#include "../sensor/SGP30_sensor.h"
#include "../sensor/SPS30_sensor.h"

static InfluxDBClient influxdb_client;
static Point          influxdb_point(INFLUXDB_POINT_NAME);

bool influxdb_init() {
  Log.debugln("influxdb_init: init influxDB with following parameters: "
              "url: '" INFLUXDB_URL "', "
              "org: '" INFLUXDB_ORG "', "
              "bucket: '" INFLUXDB_BUCKET "'");

  influxdb_client.setConnectionParams(
      INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

  influxdb_point.addTag("host", Preference.get_board_host_name());
  influxdb_point.addTag("location", Preference.get_board_location());
  influxdb_point.addTag("room", Preference.get_board_room());

  Log.traceln("influxdb_init: added tags: host: '" + Preference.get_board_host_name()
              + "', location: '" + Preference.get_board_location() + "', room: '"
              + Preference.get_board_room() + "'");

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
  // TODO: Remove all this sensor-checking code inside InfluxDB.
  if (Preference.has_sensor(SensorType::SENSOR_DHT11)) {
    Log.traceln("influxdb_write_sensors: sending DHT11 values");
    influxdb_point.addField(INFLUXDB_FIELD_DHT11_TEMPERATURE, DHT11Sensor.get_temperature());
    influxdb_point.addField(INFLUXDB_FIELD_DHT11_HUMIDITY, DHT11Sensor.get_humidity());
  }
  if (Preference.has_sensor(SensorType::SENSOR_SGP30)) {
    Log.traceln("influxdb_write_sensors: sending SGP30 values");
    influxdb_point.addField(INFLUXDB_FIELD_SGP30_TVOC, SGP30Sensor.get_TVOC());
    influxdb_point.addField(INFLUXDB_FIELD_SGP30_ECO2, SGP30Sensor.get_eCO2());
  }
  if (Preference.has_sensor(SensorType::SENSOR_SPS30)) {
    Log.traceln("influxdb_write_sensors: sending SPS30 values");
    influxdb_point.addField(INFLUXDB_FIELD_SPS30_MC_1p0, SPS30Sensor.get_mc_1p0());
    influxdb_point.addField(INFLUXDB_FIELD_SPS30_MC_2p5, SPS30Sensor.get_mc_2p5());
    influxdb_point.addField(INFLUXDB_FIELD_SPS30_MC_4p0, SPS30Sensor.get_mc_4p0());
    influxdb_point.addField(INFLUXDB_FIELD_SPS30_MC_10p0, SPS30Sensor.get_mc_10p0());
    influxdb_point.addField(INFLUXDB_FIELD_SPS30_PARTICLE_SIZE, SPS30Sensor.get_particle_size());
  }
  if (Preference.has_sensor(SensorType::SENSOR_MHZ19)) {
    Log.traceln("influxdb_write_sensors: sending MHZ19 values");
    influxdb_point.addField(INFLUXDB_FIELD_MHZ19_CO2, MHZ19Sensor.get_co2());
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
