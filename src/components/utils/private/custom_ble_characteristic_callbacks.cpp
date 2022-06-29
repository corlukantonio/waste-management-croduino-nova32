#include "components/utils/public/custom_ble_characteristic_callbacks.h"

void CustomBLECharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
  String rxValue = String(pCharacteristic->getValue().c_str());

  if (rxValue.length() > 0)
  {
    for (size_t i = 0; i < rxValue.length(); i++)
    {
      Serial.println(rxValue[i]);
    }
  }
}