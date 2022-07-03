#include "components/utils/public/custom_ble_server_callbacks.h"

#ifdef TARGET_ESP32DEV

void CustomBLEServerCallbacks::onConnect(BLEServer *pServer)
{
  m_isDeviceConnected = true;
}

void CustomBLEServerCallbacks::onDisconnect(BLEServer *pServer)
{
  pServer->getAdvertising()->start();

  m_isDeviceConnected = false;
}

bool CustomBLEServerCallbacks::GetIsDeviceConnected() const
{
  return m_isDeviceConnected;
}

#endif