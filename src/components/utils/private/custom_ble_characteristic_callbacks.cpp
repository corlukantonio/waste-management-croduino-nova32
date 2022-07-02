#include "components/utils/public/custom_ble_characteristic_callbacks.h"

#ifdef TARGET_ESP32DEV
void CustomBLECharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
  String rxValue = String(pCharacteristic->getValue().c_str());

  if (rxValue.length() > 0)
  {
    Serial.println(rxValue);
  }
}
#endif