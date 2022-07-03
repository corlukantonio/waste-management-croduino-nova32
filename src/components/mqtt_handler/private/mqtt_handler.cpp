#include "components/mqtt_handler/public/mqtt_handler.h"

#ifdef TARGET_ESP32DEV

MqttHandler::MqttHandler(const char *pName, uint32_t stackDepth, TaskHandle_t *pTaskHandler)
    : TaskHandler(pName, stackDepth, pTaskHandler)
{
  m_mutex = xSemaphoreCreateMutex();

  if (m_mutex == NULL)
  {
    Serial.println("Mutex can not be created.");
  }

  m_pMqttClient = new MQTTClient();
  m_pWiFiClient = new WiFiClient();
}

void MqttHandler::Task()
{
  m_mqttServer = "hairdresser.cloudmqtt.com";
  m_mqttPort = 18972;
  m_mqttUser = "jlrwtkuh";
  m_mqttPassword = "vsHQvxEaOaIE";

  m_pMqttClient->begin(m_mqttServer, m_mqttPort, *m_pWiFiClient);

  if (!m_pMqttClient->connect("esp32wastebin", m_mqttUser, m_mqttPassword))
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMqttConnectionFailed));
  }

  if (m_pMqttClient->connected())
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMqttConnected, 1, m_mqttServer));
  }
}

#endif