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

  m_pRtc->setTime();

  m_startTime = millis();
  m_currentTime = millis();
  m_pirPinState = LOW;
  m_pirPinStatePrev = m_pirPinState;
  m_isRecordPackageSent = false;
}

void WasteBin::SendObjectRegistrationRequest(const String kData) const
{
  xSemaphoreTake(m_mutex, portMAX_DELAY);

  Common::ObjectRegistrationRequestPackage *pObjectRegistrationRequestPackage =
      (Common::ObjectRegistrationRequestPackage *)malloc(sizeof(Common::ObjectRegistrationRequestPackage));

  pObjectRegistrationRequestPackage->packageType = OBJ_REG_REQ_PKG;
  pObjectRegistrationRequestPackage->packageVersion = OBJ_REG_REQ_PKG_V;
  WiFi.macAddress(pObjectRegistrationRequestPackage->mac);
  pObjectRegistrationRequestPackage->rtc[0] = m_pRtc->getYear();
  pObjectRegistrationRequestPackage->rtc[1] = m_pRtc->getMonth();
  pObjectRegistrationRequestPackage->rtc[2] = m_pRtc->getDay();
  pObjectRegistrationRequestPackage->rtc[3] = m_pRtc->getHour();
  pObjectRegistrationRequestPackage->rtc[4] = m_pRtc->getMinute();
  pObjectRegistrationRequestPackage->rtc[5] = m_pRtc->getSecond();

  const Common::BytesPackage *pBytesPackage =
      Common::GetInstance()
          ->GetPackageWithArgsInBytes<Common::ObjectRegistrationRequestPackage>(
              &pObjectRegistrationRequestPackage);

  pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, sizeof(pBytesPackage->length) - 1);

  if (m_pMqttHandler->GetMqttClientObject()->publish(m_pMqttHandler->GetMqttTopics()[0], (char *)pBytesPackage->pBytes, pBytesPackage->length))
  {
    for (size_t i = 0; i < pBytesPackage->length; i++)
    {
      Serial.print(pBytesPackage->pBytes[i]);

      if (i < pBytesPackage->length - 1)
      {
        Serial.print(" ");
      }
    }

    Serial.println("");
  }

  xSemaphoreGive(m_mutex);
}

void WasteBin::SendObjectActivationRequest(const String kData) const
{
  xSemaphoreTake(m_mutex, portMAX_DELAY);

  Common::ObjectActivationRequestPackage *pObjectActivationRequestPackage =
      (Common::ObjectActivationRequestPackage *)malloc(sizeof(Common::ObjectActivationRequestPackage));

  pObjectActivationRequestPackage->packageType = OBJ_ACT_REQ_PKG;
  pObjectActivationRequestPackage->packageVersion = OBJ_ACT_REQ_PKG_V;
  WiFi.macAddress(pObjectActivationRequestPackage->mac);
  pObjectActivationRequestPackage->rtc[0] = m_pRtc->getYear();
  pObjectActivationRequestPackage->rtc[1] = m_pRtc->getMonth();
  pObjectActivationRequestPackage->rtc[2] = m_pRtc->getDay();
  pObjectActivationRequestPackage->rtc[3] = m_pRtc->getHour();
  pObjectActivationRequestPackage->rtc[4] = m_pRtc->getMinute();
  pObjectActivationRequestPackage->rtc[5] = m_pRtc->getSecond();
  pObjectActivationRequestPackage->activationCode[0] = 0;
  pObjectActivationRequestPackage->activationCode[1] = 0;
  pObjectActivationRequestPackage->activationCode[2] = 0;
  pObjectActivationRequestPackage->activationCode[3] = 0;

  const Common::BytesPackage *pBytesPackage =
      Common::GetInstance()
          ->GetPackageWithArgsInBytes<Common::ObjectActivationRequestPackage>(
              &pObjectActivationRequestPackage);

  pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, sizeof(pBytesPackage->length) - 1);

  if (m_pMqttHandler->GetMqttClientObject()->publish(m_pMqttHandler->GetMqttTopics()[1], (char *)pBytesPackage->pBytes, pBytesPackage->length))
  {
    for (size_t i = 0; i < pBytesPackage->length; i++)
    {
      Serial.print(pBytesPackage->pBytes[i]);

      if (i < pBytesPackage->length - 1)
      {
        Serial.print(" ");
      }
    }

    Serial.println("");
  }

  xSemaphoreGive(m_mutex);
}

void WasteBin::SendObjectSettingsPackage(const String kData) const
{
  xSemaphoreTake(m_mutex, portMAX_DELAY);

  Common::ObjectSettingsPackage *pObjectSettingsPackage =
      (Common::ObjectSettingsPackage *)malloc(sizeof(Common::ObjectSettingsPackage));

  Common::ObjectRecordValue<double> *pWasteBinCapacityLimit =
      (Common::ObjectRecordValue<double> *)malloc(sizeof(Common::ObjectRecordValue<double>));

  pObjectSettingsPackage->packageType = OBJ_STG_PKG;
  pObjectSettingsPackage->packageVersion = OBJ_STG_PKG_V;
  WiFi.macAddress(pObjectSettingsPackage->mac);
  pObjectSettingsPackage->numberOfValues = 1;

  pWasteBinCapacityLimit->type = WASTE_BIN_CAPACITY_LIMIT;

  if (Common::GetInstance()->GetArgs(kData).size() > 1)
  {
    pWasteBinCapacityLimit->value = Common::GetInstance()->GetArgs(kData).at(1).toDouble();
  }
  else
  {
    pWasteBinCapacityLimit->value = 7.0;
  }

  const Common::BytesPackage *pBytesPackage =
      Common::GetInstance()
          ->GetPackageWithArgsInBytes<Common::ObjectSettingsPackage,
                                      Common::ObjectRecordValue<double> *>(
              &pObjectSettingsPackage,
              pWasteBinCapacityLimit);

  pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, pBytesPackage->length - 1);

  if (m_pMqttHandler->GetMqttClientObject()->publish(m_pMqttHandler->GetMqttTopics()[2], (char *)pBytesPackage->pBytes, pBytesPackage->length))
  {
    for (size_t i = 0; i < pBytesPackage->length; i++)
    {
      Serial.print(pBytesPackage->pBytes[i]);

      if (i < pBytesPackage->length - 1)
      {
        Serial.print(" ");
      }
    }

    Serial.println("");
  }

  xSemaphoreGive(m_mutex);
}

void WasteBin::SendObjectRecordConfigRequest(const String kData) const
{
  xSemaphoreTake(m_mutex, portMAX_DELAY);

  Common::ObjectRecordConfigRequestPackage *pObjectRecordConfigRequestPackage =
      (Common::ObjectRecordConfigRequestPackage *)malloc(sizeof(Common::ObjectRecordConfigRequestPackage));

  pObjectRecordConfigRequestPackage->packageType = OBJ_REC_CFG_REQ_PKG;
  pObjectRecordConfigRequestPackage->packageVersion = OBJ_REC_CFG_REQ_PKG_V;
  WiFi.macAddress(pObjectRecordConfigRequestPackage->mac);
  pObjectRecordConfigRequestPackage->rtc[0] = m_pRtc->getYear();
  pObjectRecordConfigRequestPackage->rtc[1] = m_pRtc->getMonth();
  pObjectRecordConfigRequestPackage->rtc[2] = m_pRtc->getDay();
  pObjectRecordConfigRequestPackage->rtc[3] = m_pRtc->getHour();
  pObjectRecordConfigRequestPackage->rtc[4] = m_pRtc->getMinute();
  pObjectRecordConfigRequestPackage->rtc[5] = m_pRtc->getSecond();
  pObjectRecordConfigRequestPackage->recordBasePackageVersion = OBJ_REC_BASE_PKG_V;
  pObjectRecordConfigRequestPackage->numberOfValues = 3;

  const Common::BytesPackage *pBytesPackage =
      Common::GetInstance()
          ->GetPackageWithArgsInBytes<Common::ObjectRecordConfigRequestPackage>(
              &pObjectRecordConfigRequestPackage);

  pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, pBytesPackage->length - 1);

  if (m_pMqttHandler->GetMqttClientObject()->publish(m_pMqttHandler->GetMqttTopics()[3], (char *)pBytesPackage->pBytes, pBytesPackage->length))
  {
    for (size_t i = 0; i < pBytesPackage->length; i++)
    {
      Serial.print(pBytesPackage->pBytes[i]);

      if (i < pBytesPackage->length - 1)
      {
        Serial.print(" ");
      }
    }

    Serial.println("");
  }

  xSemaphoreGive(m_mutex);
}

void WasteBin::SendObjectRecordConfigApprovalRequest(const String kData) const
{
  xSemaphoreTake(m_mutex, portMAX_DELAY);

  Common::ObjectRecordConfigApprovalRequestPackage *pObjectRecordConfigApprovalRequestPackage =
      (Common::ObjectRecordConfigApprovalRequestPackage *)malloc(sizeof(Common::ObjectRecordConfigApprovalRequestPackage));

  pObjectRecordConfigApprovalRequestPackage->packageType = OBJ_REC_CFG_REQ_PKG;
  pObjectRecordConfigApprovalRequestPackage->packageVersion = OBJ_REC_CFG_REQ_PKG_V;
  WiFi.macAddress(pObjectRecordConfigApprovalRequestPackage->mac);
  pObjectRecordConfigApprovalRequestPackage->rtc[0] = m_pRtc->getYear();
  pObjectRecordConfigApprovalRequestPackage->rtc[1] = m_pRtc->getMonth();
  pObjectRecordConfigApprovalRequestPackage->rtc[2] = m_pRtc->getDay();
  pObjectRecordConfigApprovalRequestPackage->rtc[3] = m_pRtc->getHour();
  pObjectRecordConfigApprovalRequestPackage->rtc[4] = m_pRtc->getMinute();
  pObjectRecordConfigApprovalRequestPackage->rtc[5] = m_pRtc->getSecond();
  pObjectRecordConfigApprovalRequestPackage->approvalCode[0] = 0;
  pObjectRecordConfigApprovalRequestPackage->approvalCode[1] = 0;
  pObjectRecordConfigApprovalRequestPackage->approvalCode[2] = 0;
  pObjectRecordConfigApprovalRequestPackage->approvalCode[3] = 0;

  const Common::BytesPackage *pBytesPackage =
      Common::GetInstance()
          ->GetPackageWithArgsInBytes<Common::ObjectRecordConfigApprovalRequestPackage>(
              &pObjectRecordConfigApprovalRequestPackage);

  pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, pBytesPackage->length - 1);

  if (m_pMqttHandler->GetMqttClientObject()->publish(m_pMqttHandler->GetMqttTopics()[4], (char *)pBytesPackage->pBytes, pBytesPackage->length))
  {
    for (size_t i = 0; i < pBytesPackage->length; i++)
    {
      Serial.print(pBytesPackage->pBytes[i]);

      if (i < pBytesPackage->length - 1)
      {
        Serial.print(" ");
      }
    }

    Serial.println("");
  }

  xSemaphoreGive(m_mutex);
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
  pObjectRecordBasePackage->rtc[0] = m_pRtc->getYear();
  pObjectRecordBasePackage->rtc[1] = m_pRtc->getMonth();
  pObjectRecordBasePackage->rtc[2] = m_pRtc->getDay();
  pObjectRecordBasePackage->rtc[3] = m_pRtc->getHour();
  pObjectRecordBasePackage->rtc[4] = m_pRtc->getMinute();
  pObjectRecordBasePackage->rtc[5] = m_pRtc->getSecond();
  pObjectRecordBasePackage->numberOfValues = 3;
  pObjectRecordBasePackage->rssi = WiFi.RSSI();

  pDistance->type = DISTANCE;
  pDistance->value = m_cm;

  pHumidity->type = HUMIDITY;
  pHumidity->value = m_humidity;

  pTemperatureCelsius->type = TEMPERATURE_CELSIUS;
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

  pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, pBytesPackage->length - 1);

  if (m_pMqttHandler->GetMqttClientObject()->publish(m_pMqttHandler->GetMqttTopics()[5], (char *)pBytesPackage->pBytes, pBytesPackage->length))
  {
    for (size_t i = 0; i < pBytesPackage->length; i++)
    {
      Serial.print(pBytesPackage->pBytes[i]);

      if (i < pBytesPackage->length - 1)
      {
        Serial.print(" ");
      }
    }

    Serial.println("");
  }

  xSemaphoreGive(m_mutex);
}

void WasteBin::ReadSensorValues()
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
}

void WasteBin::Task()
{
  m_pDht->begin();

  m_pBleHandler->AddCallback("wb.devreg", bind(&WasteBin::SendObjectRegistrationRequest, this, std::placeholders::_1));
  m_pBleHandler->AddCallback("wb.devact", bind(&WasteBin::SendObjectActivationRequest, this, std::placeholders::_1));
  m_pBleHandler->AddCallback("wb.devstg", bind(&WasteBin::SendObjectSettingsPackage, this, std::placeholders::_1));
  m_pBleHandler->AddCallback("wb.reccfg", bind(&WasteBin::SendObjectRecordConfigRequest, this, std::placeholders::_1));
  m_pBleHandler->AddCallback("wb.recapp", bind(&WasteBin::SendObjectRecordConfigApprovalRequest, this, std::placeholders::_1));
  m_pBleHandler->AddCallback("wb.sendrecord", bind(&WasteBin::SendRecord, this, std::placeholders::_1));

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  while (true)
  {
    ReadSensorValues();

    if (m_pMqttHandler->GetMqttClientObject()->connected() && !isnan(m_humidity) && !isnan(m_temperatureCelsius) && !m_isRecordPackageSent)
    {
      SendRecord();

      m_isRecordPackageSent = true;
    }

    m_currentTime = millis();

    if (!m_pBleHandler->GetIsDeviceConnected() && (m_currentTime - m_startTime >= DEEP_SLEEP_COUNT || m_isRecordPackageSent))
    {
      Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgDeepSleep));

      m_pMqttHandler->GetMqttClientObject()->disconnect();

      m_pWiFiHandler->GetWiFiClassObject()->disconnect();
      m_pWiFiHandler->GetWiFiClassObject()->setSleep(WIFI_PS_MAX_MODEM);

      vTaskDelay(2000 / portTICK_PERIOD_MS);

      esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, HIGH);
      esp_sleep_enable_timer_wakeup(1800 * uS_TO_S_FACTOR);
      esp_deep_sleep_start();
    }

#if LOG_STACK == 1
    Serial.print("WasteBin: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
#endif

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

#endif