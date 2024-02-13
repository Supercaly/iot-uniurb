#ifndef INFLUXDB_H
#define INFLUXDB_H

/*
 * Init a new connection with InfluxDB server.
 */
bool influxdb_init();

/*
 * Write sensors values to InfluxDB remote server.
 */
bool influxdb_write_sensors();

#endif // INFLUXDB_H
