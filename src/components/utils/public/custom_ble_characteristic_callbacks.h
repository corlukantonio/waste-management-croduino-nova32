#ifdef TARGET_ESP32DEV
#include <Arduino.h>
#include <BLEServer.h>

/**
 * @class CustomBLECharacteristicCallbacks
 * @brief CustomBLECharacteristicCallbacks class.
 */
class CustomBLECharacteristicCallbacks : public BLECharacteristicCallbacks
{

private:
  virtual void onWrite(BLECharacteristic *pCharacteristic) override final;
};
#endif