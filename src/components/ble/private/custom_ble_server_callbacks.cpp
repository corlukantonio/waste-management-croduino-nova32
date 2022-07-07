#include "components/ble/public/custom_ble_server_callbacks.h"

#ifdef TARGET_ESP32DEV

CustomBLEServerCallbacks::CustomBLEServerCallbacks(PwmHandler *pPwmHandler, const uint8_t kBuzzerPin)
    : m_pPwmHandler(pPwmHandler),
      m_buzzerPin(kBuzzerPin)
{
}

void CustomBLEServerCallbacks::onConnect(BLEServer *pServer)
{
  Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgBleClientConnected));

  m_pPwmHandler->PlayConnect(m_buzzerPin);

  m_isDeviceConnected = true;
}

void CustomBLEServerCallbacks::onDisconnect(BLEServer *pServer)
{
  Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgBleClientDisconnected));

  pServer->getAdvertising()->start();

  m_pPwmHandler->PlayDisconnect(m_buzzerPin);

  m_isDeviceConnected = false;
}

bool CustomBLEServerCallbacks::GetIsDeviceConnected() const
{
  return m_isDeviceConnected;
}

#endif