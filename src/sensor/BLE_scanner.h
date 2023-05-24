#ifndef BLE_SCANNER_H
#define BLE_SCANNER_H

#include <NimBLEDevice.h>

#include "abstract_sensor.h"

#define BLE_SCANNER_DURATION_S 5
#define BLE_SCANNER_MIN_RSSI   -80

class BLE_scanner : public AbstractSensor {
  public:
  bool on_init() override;
  bool on_measure() override;

  int get_devices() const { return _devices_count; }

  private:
  NimBLEScan *_bleScan;
  int         _devices_count = 0;
};

extern BLE_scanner BLEScanner;

#endif // BLE_SCANNER_H