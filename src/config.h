#ifndef CONFIG_H
#define CONFIG_H

/*
 * Global configuration header.
 * 
 * Here we put all the configurations for the
 * various elements of the project.
 */

// Enable Debug Mode.
#define IS_DEBUG
// Print the value of sensors after reading them.
#define PRINT_SENSORS_ON_READ
// Default serial used for print.
#define LOGGER_SERIAL Serial
// Serial Baud Rate.
#define BAUD_RATE 115200

#endif  // CONFIG_H
