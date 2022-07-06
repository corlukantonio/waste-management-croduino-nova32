#include "components/ble/public/custom_ble_characteristic_callbacks.h"

#ifdef TARGET_ESP32DEV

CustomBLECharacteristicCallbacks::CustomBLECharacteristicCallbacks(std::queue<String> *pqBleCallbacks)
    : m_pqBleCallbacks(pqBleCallbacks)
{
}

void CustomBLECharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
  String rxValue = String(pCharacteristic->getValue().c_str());

  if (rxValue.length() > 0)
  {
    Serial.println(rxValue);

    m_pqBleCallbacks->push(rxValue);
  }
}

#endif