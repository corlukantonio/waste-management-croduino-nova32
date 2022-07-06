#include "components/waste_bin/public/waste_bin.h"

#ifdef TARGET_ESP32DEV

WasteBin::WasteBin(const char *kpName, uint32_t stackDepth, UBaseType_t uxPriority, TaskHandle_t *pTaskHandler, BaseType_t xCoreID, BleHandler *pBleHandler, WiFiHandler *pWiFiHandler, MqttHandler *pMqttHandler, const uint8_t kBuzzerPin, const uint8_t kLedPin, const uint8_t kPirPin, const uint8_t kTempHumiSensPin, const uint8_t kUltrasonicSensEchoPin, const uint8_t kUltrasonicSensTrigPin)
    : TaskHandler(kpName, stackDepth, uxPriority, pTaskHandler, xCoreID),
      m_pBleHandler(pBleHandler),
      m_pWiFiHandler(pWiFiHandler),
      m_pMqttHandler(pMqttHandler),
      m_buzzerPin(kBuzzerPin),
      m_ledPin(kLedPin),
      m_pirPin(kPirPin),
      m_tempHumiSensPin(kTempHumiSensPin),
      m_ultrasonicSensEchoPin(kUltrasonicSensEchoPin),
      m_ultrasonicSensTrigPin(kUltrasonicSensTrigPin)
{
  m_mutex = xSemaphoreCreateMutex();

  if (m_mutex == NULL)
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMutexNotCreated));
  }

  m_pDht = new DHT(kTempHumiSensPin, DHT11);
  m_pRtc = new ESP32Time();

  m_startTime = millis();
  m_currentTime = millis();
  m_pirPinState = LOW;
  m_pirPinStatePrev = m_pirPinState;
}

void WasteBin::SendRecord(const String kData) const
{
  xSemaphoreTake(m_mutex, portMAX_DELAY);

  Common::ObjectRecordBasePackage *pObjectRecordBasePackage =
      (Common::ObjectRecordBasePackage *)malloc(sizeof(Common::ObjectRecordBasePackage));

  Common::ObjectRecordValue<double> *pDistance =
      (Common::ObjectRecordValue<double> *)malloc(sizeof(Common::ObjectRecordValue<double>));

  Common::ObjectRecordValue<double> *pHumidity =
      (Common::ObjectRecordValue<double> *)malloc(sizeof(Common::ObjectRecordValue<double>));

  Common::ObjectRecordValue<double> *pTemperatureCelsius =
      (Common::ObjectRecordValue<double> *)malloc(sizeof(Common::ObjectRecordValue<double>));

  pObjectRecordBasePackage->packageType = OBJ_REC_BASE_PKG;
  pObjectRecordBasePackage->packageVersion = OBJ_REC_BASE_PKG_V;
  WiFi.macAddress(pObjectRecordBasePackage->mac);
  pObjectRecordBasePackage->rtc[0] = 0;
  pObjectRecordBasePackage->rtc[1] = 0;
  pObjectRecordBasePackage->rtc[2] = 0;
  pObjectRecordBasePackage->rtc[3] = 0;
  pObjectRecordBasePackage->rtc[4] = 0;
  pObjectRecordBasePackage->rtc[5] = 0;
  pObjectRecordBasePackage->numberOfValues = 3;
  pObjectRecordBasePackage->rssi = WiFi.RSSI();

  pDistance->type = 1;
  pDistance->value = m_cm;

  pHumidity->type = 2;
  pHumidity->value = m_humidity;

  pTemperatureCelsius->type = 3;
  pTemperatureCelsius->value = m_temperatureCelsius;

  const Common::BytesPackage *pBytesPackage =
      Common::GetInstance()
          ->GetPackageWithArgsInBytes<Common::ObjectRecordBasePackage,
                                      Common::ObjectRecordValue<double> *,
                                      Common::ObjectRecordValue<double> *,
                                      Common::ObjectRecordValue<double> *>(
              &pObjectRecordBasePackage,
              pDistance,
              pHumidity,
              pTemperatureCelsius);

  pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, sizeof(pBytesPackage->length) - 1);

  m_pMqttHandler->GetMqttClientObject()->publish(m_pMqttHandler->GetMqttTopics()[0], (char *)pBytesPackage->pBytes, pBytesPackage->length);

  xSemaphoreGive(m_mutex);
}

void WasteBin::Task()
{
  m_pDht->begin();

  m_pBleHandler->AddCallback("wb.sendrecord", bind(&WasteBin::SendRecord, this, std::placeholders::_1));

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  while (true)
  {
    digitalWrite(m_ultrasonicSensTrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(m_ultrasonicSensTrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(m_ultrasonicSensTrigPin, LOW);

    m_duration = pulseIn(m_ultrasonicSensEchoPin, HIGH);
    m_cm = m_duration / 29 / 2;

    // Serial.println(m_cm);

    m_humidity = m_pDht->readHumidity();
    m_temperatureCelsius = m_pDht->readTemperature();

    if (isnan(m_humidity) || isnan(m_temperatureCelsius))
    {
      Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgDhtReadFail));
    }

    // Serial.println(m_humidity);
    // Serial.println(m_temperatureCelsius);

    m_currentTime = millis();

    if (!m_pBleHandler->GetIsDeviceConnected() && m_currentTime - m_startTime >= DEEP_SLEEP_COUNT)
    {
      Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgDeepSleep));

      m_pMqttHandler->GetMqttClientObject()->disconnect();

      m_pWiFiHandler->GetWiFiClassObject()->disconnect();
      m_pWiFiHandler->GetWiFiClassObject()->setSleep(WIFI_PS_MAX_MODEM);

      vTaskDelay(2000 / portTICK_PERIOD_MS);

      esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, HIGH);
      esp_deep_sleep_start();
    }

    // Serial.print("WasteBin: ");
    // Serial.println(uxTaskGetStackHighWaterMark(NULL));

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

#endif