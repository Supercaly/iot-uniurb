#include "BLE_scanner.h"

#include <NimBLEDevice.h>

#include "../board_preference.h"
#include "../log.h"

BLE_scanner BLEScanner;

bool BLE_scanner::on_init() {
  NimBLEDevice::setScanFilterMode(CONFIG_BTDM_SCAN_DUPL_TYPE_DEVICE);
  NimBLEDevice::setScanDuplicateCacheSize(200);
  std::string ble_name = std::string(String("ble-" + Preference.get_board_host_name()).c_str());
  NimBLEDevice::init(ble_name);
  _bleScan = NimBLEDevice::getScan();
  _bleScan->setActiveScan(true);
  _bleScan->setInterval(100);
  _bleScan->setWindow(99);
  app_traceln("BLE_scanner::on_init: BLE scanner initialized");

  return true;
}

bool BLE_scanner::on_measure() {
  _devices_count                 = 0;
  NimBLEScanResults foundDevices = _bleScan->start(BLE_SCANNER_DURATION_S, false);
  app_debugln("BLE_scanner::on_measure: BLE devices found:");
  for (int i = 0; i < foundDevices.getCount(); i++) {
    NimBLEAdvertisedDevice device = foundDevices.getDevice(i);
    app_debug("name: '" + String(device.getName().c_str()) + "' ");
    app_debug("addr: '" + String(device.getAddress().toString().c_str()) + "' ");
    app_debugln("rssi: '" + String(device.getRSSI()) + "' ");
    if (device.getRSSI() > BLE_SCANNER_MIN_RSSI) {
      _devices_count++;
    }
  }
  _bleScan->clearResults();
  app_debugln("BLE_scanner::on_measure: found " + String(_devices_count) + " devices");
  return true;
}