#include "components/ble/public/custom_ble_server_callbacks.h"

#ifdef TARGET_ESP32DEV

void CustomBLEServerCallbacks::onConnect(BLEServer *pServer)
{
  Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgBleClientConnected));

  m_isDeviceConnected = true;
}

void CustomBLEServerCallbacks::onDisconnect(BLEServer *pServer)
{
  Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgBleClientDisconnected));

  pServer->getAdvertising()->start();

  m_isDeviceConnected = false;
}

bool CustomBLEServerCallbacks::GetIsDeviceConnected() const
{
  return m_isDeviceConnected;
}

#endif