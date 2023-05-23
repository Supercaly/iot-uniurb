#ifndef INFLUXDB_H
#define INFLUXDB_H

// Constants Field Names.
#define INFLUXDB_FIELD_DHT11_TEMPERATURE   "dht11_temperature"
#define INFLUXDB_FIELD_DHT11_HUMIDITY      "dht11_humidity"
#define INFLUXDB_FIELD_SGP30_TVOC          "sgp30_TVOC"
#define INFLUXDB_FIELD_SGP30_ECO2          "sgp30_eCO2"
#define INFLUXDB_FIELD_SPS30_MC_1p0        "sps30_mc_1p0"
#define INFLUXDB_FIELD_SPS30_MC_2p5        "sps30_mc_2p5"
#define INFLUXDB_FIELD_SPS30_MC_4p0        "sps30_mc_4p0"
#define INFLUXDB_FIELD_SPS30_MC_10p0       "sps30_mc_10p0"
#define INFLUXDB_FIELD_SPS30_PARTICLE_SIZE "sps30_particle_size"
#define INFLUXDB_FIELD_MHZ19_CO2           "mhz19_co2"
#define INFLUXDB_FIELD_BLE_DEVICES         "ble_device_count"

/*
 * Init a new connection with InfluxDB server.
 */
bool influxdb_init();

/*
 * Write sensors values to InfluxDB remote server.
 */
bool influxdb_write_sensors();

#endif // INFLUXDB_H
