#include "components/ble/public/ble_handler.h"

#ifdef TARGET_ESP32DEV

BleHandler::BleHandler(const char *kpName, uint32_t stackDepth, UBaseType_t uxPriority, TaskHandle_t *pTaskHandler, BaseType_t xCoreID, PwmHandler *pPwmHandler, const uint8_t kBuzzerPin)
    : TaskHandler(kpName, stackDepth, uxPriority, pTaskHandler, xCoreID),
      m_pPwmHandler(pPwmHandler),
      m_buzzerPin(kBuzzerPin)
{
  m_mutex = xSemaphoreCreateMutex();

  if (m_mutex == NULL)
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMutexNotCreated));
  }

  m_pCustomBLECharacteristicCallbacks = new CustomBLECharacteristicCallbacks(&m_qBleCallbacks);
  m_pCustomBLEServerCallbacks = new CustomBLEServerCallbacks(pPwmHandler, kBuzzerPin);

  BLEDevice::init(BLE_DEV_NAME);
  m_pBleServer = BLEDevice::createServer();
  m_pBleServer->setCallbacks(m_pCustomBLEServerCallbacks);
  m_pBleService = m_pBleServer->createService(BLE_SERVICE_UUID);
  m_pBleCharacteristic = m_pBleService->createCharacteristic(
      BLE_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  m_pBleCharacteristic->setCallbacks(m_pCustomBLECharacteristicCallbacks);
  m_pBleService->start();
  m_pBleAdvertising = m_pBleServer->getAdvertising();
  m_pBleAdvertising->addServiceUUID(BLE_SERVICE_UUID);
  m_pBleAdvertising->setScanResponse(true);
  m_pBleAdvertising->setMinPreferred(0x06);
  m_pBleAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

bool BleHandler::GetIsDeviceConnected() const
{
  return m_pCustomBLEServerCallbacks->GetIsDeviceConnected();
}

void BleHandler::AddCallback(const char *kpName, TCallback cb)
{
  xSemaphoreTake(m_mutex, portMAX_DELAY);

  m_vBleCallbacks.push_back(BleCallback{kpName, cb});

  xSemaphoreGive(m_mutex);
}

void BleHandler::Task()
{
  while (true)
  {
    if (!m_qBleCallbacks.empty())
    {
      for (BleCallback bleCallback : m_vBleCallbacks)
      {
        if (Common::GetInstance()->GetArgs(bleCallback.name).at(0).equals(Common::GetInstance()->GetArgs(m_qBleCallbacks.front()).at(0)))
        {
          bleCallback.cb(m_qBleCallbacks.front());

          break;
        }
      }

      m_qBleCallbacks.pop();
    }

#if LOG_STACK == 1
    Serial.print("BleHandler: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
#endif

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

#endif