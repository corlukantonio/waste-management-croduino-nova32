#include "components/mqtt_handler/public/mqtt_handler.h"

#ifdef TARGET_ESP32DEV

MqttHandler::MqttHandler(const char *kpName, uint32_t stackDepth, UBaseType_t uxPriority, TaskHandle_t *pTaskHandler, BaseType_t xCoreID, WiFiHandler *pWiFiHandler)
    : TaskHandler(kpName, stackDepth, uxPriority, pTaskHandler, xCoreID),
      m_pWiFiHandler(pWiFiHandler)
{
  m_mutex = xSemaphoreCreateMutex();

  if (m_mutex == NULL)
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMutexNotCreated));
  }

  m_pMqttClient = new MQTTClient(256);
  m_pMqttClient->setKeepAlive(MQTT_KEEP_ALIVE);
  m_pWiFiClient = new WiFiClient();

  m_pMqttServer = "driver.cloudmqtt.com";
  m_pMqttPort = 18850;
  m_pMqttUser = "gdnuxjsy";
  m_pMqttPassword = "dc5a1o4kQlmi";
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
  AddMqttTopic("iot/wastemanagement/record");

  while (true)
  {
    if (m_pWiFiHandler->GetIsDeviceConnected())
    {
      try
      {
        m_pMqttClient->begin(m_pMqttServer, m_pMqttPort, *m_pWiFiClient);

        if (!m_pMqttClient->connect(MQTT_CLIENT_ID, m_pMqttUser, m_pMqttPassword))
        {
          Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMqttConnectionFailed));
        }

        if (m_pMqttClient->connected())
        {
          Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMqttConnected, 1, m_pMqttServer));
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
    // Serial.print("MqttHandler: ");
    // Serial.println(uxTaskGetStackHighWaterMark(NULL));

    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

#endif