#include "components/mqtt_handler/public/mqtt_handler.h"

#ifdef TARGET_ESP32DEV

MqttHandler::MqttHandler(const char *kpName, uint32_t stackDepth, UBaseType_t uxPriority, TaskHandle_t *pTaskHandler, BaseType_t xCoreID, BleHandler *pBleHandler, WiFiHandler *pWiFiHandler)
    : TaskHandler(kpName, stackDepth, uxPriority, pTaskHandler, xCoreID),
      m_pBleHandler(pBleHandler),
      m_pWiFiHandler(pWiFiHandler)
{
  m_mutex = xSemaphoreCreateMutex();

  if (m_mutex == NULL)
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMutexNotCreated));
  }

  EEPROM.begin(EEPROM_SIZE);

  m_pMqttClient = new MQTTClient(256);
  m_pMqttClient->setKeepAlive(MQTT_KEEP_ALIVE);
  m_pWiFiClient = new WiFiClient();

  if (EEPROM.readByte(4) != 0xFF)
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgEepromWriteMqttCreds));

    m_pMqttCredentials = (Common::MqttCredentials *)malloc(sizeof(Common::MqttCredentials));

    m_pMqttCredentials->flag = 0xFF;
    strcpy(m_pMqttCredentials->server, "");
    m_pMqttCredentials->port = NULL;
    strcpy(m_pMqttCredentials->user, "");
    strcpy(m_pMqttCredentials->password, "");

    m_pBytesPackage = Common::GetInstance()->GetPackageWithArgsInBytes<Common::MqttCredentials>(&m_pMqttCredentials);

    EEPROM.writeInt(0, m_pBytesPackage->length);
    EEPROM.writeBytes(4, m_pBytesPackage->pBytes, m_pBytesPackage->length);
    EEPROM.commit();
  }
  else
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgEepromReadMqttCreds));

    m_pBytesPackageRead = (Common::BytesPackage *)malloc(sizeof(Common::BytesPackage));
    EEPROM.get(0, m_pBytesPackageRead->length);
    m_pBytesPackageRead->pBytes = (uint8_t *)malloc(m_pBytesPackageRead->length);
    EEPROM.readBytes(4, m_pBytesPackageRead->pBytes, m_pBytesPackageRead->length);

    m_pMqttCredentials = Common::GetInstance()->GetStructFromBytes<Common::MqttCredentials>(m_pBytesPackageRead->pBytes);
  }

  EEPROM.end();

  Serial.print("MQTT server: ");
  Serial.println(m_pMqttCredentials->server);
  Serial.print("MQTT port: ");
  Serial.println(m_pMqttCredentials->port);
  Serial.print("MQTT user: ");
  Serial.println(m_pMqttCredentials->user);

  m_pMqttTopics = (char **)malloc(sizeof(char *) * MQTT_TOPICS_MAX);

  for (size_t i = 0; i < MQTT_TOPICS_MAX; i++)
  {
    m_pMqttTopics[i] = nullptr;
  }
}

char **MqttHandler::GetMqttTopics() const
{
  return m_pMqttTopics;
}

MQTTClient *MqttHandler::GetMqttClientObject() const
{
  return m_pMqttClient;
}

void MqttHandler::OnMessage(String &topic, String &payload)
{
  m_pBleHandler->AddCommand(payload);
}

void MqttHandler::AddMqttTopic(const char *kpMqttTopic)
{
  xSemaphoreTake(m_mutex, portMAX_DELAY);

  bool isMax = true;

  for (size_t i = 0; i < MQTT_TOPICS_MAX; i++)
  {
    if (m_pMqttTopics[i] == nullptr)
    {
      m_pMqttTopics[i] = (char *)malloc(sizeof(char) * MQTT_TOPIC_MAX_CHAR);

      memcpy(m_pMqttTopics[i], kpMqttTopic, sizeof(char) * MQTT_TOPIC_MAX_CHAR);

      isMax = false;

      break;
    }
  }

  if (isMax)
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMqttMaxTopicsReached));
  }

  xSemaphoreGive(m_mutex);
}

void MqttHandler::Task()
{
  /**
   * The order in which you add MQTT topics is very important.
   */

  AddMqttTopic("iot/wm/devreg");
  AddMqttTopic("iot/wm/devact");
  AddMqttTopic("iot/wm/devstg");
  AddMqttTopic("iot/wm/reccfg");
  AddMqttTopic("iot/wm/recapp");
  AddMqttTopic("iot/wm/record");

  while (true)
  {
    if (m_pWiFiHandler->GetIsDeviceConnected())
    {
      try
      {
        m_pMqttClient->begin(m_pMqttCredentials->server, m_pMqttCredentials->port, *m_pWiFiClient);

        m_pMqttClient->onMessage(std::bind(&MqttHandler::OnMessage, this, std::placeholders::_1, std::placeholders::_2));

        if (!m_pMqttClient->connect(MQTT_CLIENT_ID, m_pMqttCredentials->user, m_pMqttCredentials->password))
        {
          Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMqttConnectionFailed));
        }

        if (m_pMqttClient->connected())
        {
          Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMqttConnected, 1, m_pMqttCredentials->server));

          String topicSub = String("iot/wm/response/");
          uint8_t mac[6];

          WiFi.macAddress(mac);

          topicSub.concat(Common::GetInstance()->GetHexStr(mac, 6).c_str());

          m_pMqttClient->subscribe(topicSub);
        }
      }
      catch (const std::exception &e)
      {
      }

      break;
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  while (true)
  {
    if (m_pMqttClient->connected())
    {
      m_pMqttClient->loop();
    }

#if LOG_STACK == 1
    Serial.print("MqttHandler: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
#endif

    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

#endif