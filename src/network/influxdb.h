#ifndef INFLUXDB_H
#define INFLUXDB_H

// Delay in milliseconds for restart in case on errors.
#define INFLUXDB_ON_ERROR_DELAY_MS 1000 * 60

#define INFLUXDB_FIELD_DHT11_TEMPERATURE "dht11_temperature"
#define INFLUXDB_FIELD_DHT11_HUMIDITY "dht11_humidity"
#define INFLUXDB_FIELD_SGP30_TVOC "sgp30_TVOC"
#define INFLUXDB_FIELD_SGP30_ECO2 "sgp30_eCO2"
#define INFLUXDB_FIELD_SGP30_RAW_H2 "sgp30_raw_H2"
#define INFLUXDB_FIELD_SGP30_RAW_ETHANOL "sgp30_raw_ethanol"

/*
 * Init a new connection with InfluxDB server.
 */
bool influxdb_init();

/*
 * Write sensors values to InfluxDB remote server.
 */
bool influxdb_write_sensors();

#endif  // INFLUXDB_H
