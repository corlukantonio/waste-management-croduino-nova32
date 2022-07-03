#include "components/ble_handler/public/ble_handler.h"

#ifdef TARGET_ESP32DEV

BleHandler::BleHandler(const char *pName, uint32_t stackDepth, TaskHandle_t *pTaskHandler)
    : TaskHandler(pName, stackDepth, pTaskHandler)
{
  m_mutex = xSemaphoreCreateMutex();

  if (m_mutex == NULL)
  {
    Serial.println("Mutex can not be created.");
  }

  m_pCustomBLECharacteristicCallbacks = new CustomBLECharacteristicCallbacks();
  m_pCustomBLEServerCallbacks = new CustomBLEServerCallbacks();
}

void BleHandler::Task()
{
  BLEDevice::init(DEV_NAME);

  m_pBleServer = BLEDevice::createServer();
  m_pBleServer->setCallbacks(m_pCustomBLEServerCallbacks);

  m_pBleService = m_pBleServer->createService(SERVICE_UUID);

  m_pBleCharacteristic = m_pBleService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  m_pBleCharacteristic->setValue("Hello from ESP32 device.");
  m_pBleCharacteristic->setCallbacks(m_pCustomBLECharacteristicCallbacks);
  m_pBleService->start();

  m_pBleAdvertising = m_pBleServer->getAdvertising();
  m_pBleAdvertising->addServiceUUID(SERVICE_UUID);
  m_pBleAdvertising->setScanResponse(true);
  m_pBleAdvertising->setMinPreferred(0x06);
  m_pBleAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

#endif