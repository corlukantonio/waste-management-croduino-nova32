#ifndef MQTT_HANDLER_H_
#define MQTT_HANDLER_H_

#ifdef TARGET_ESP32DEV

#include <Arduino.h>
#include <freertos/semphr.h>
#include <MQTT.h>
#include <WiFi.h>

#include "components/task_handler/public/task_handler.h"
#include "components/utils/public/common.h"

class MqttHandler : public TaskHandler
{

public:
  /********************************
   *                              *
   * Methods.                     *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  /**
   * @brief Constructor.
   *
   * @param[in] pName Pointer to the application name.
   * @param[in] stackDepth Stack depth.
   * @param[in] pTaskHandler Pointer to the task handler.
   */
  MqttHandler(const char *pName, uint32_t stackDepth, TaskHandle_t *pTaskHandler);

  /**
   * @brief Destructor.
   */
  ~MqttHandler() = default;

protected:
  virtual void Task() override final;

private:
  /********************************
   *                              *
   * Data members.                *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  SemaphoreHandle_t m_mutex;  //!< Mutex.
  MQTTClient *m_pMqttClient;  //!< MQTT client.
  WiFiClient *m_pWiFiClient;  //!< WiFi client.
  const char *m_mqttServer;   //!< MQTT server.
  int32_t m_mqttPort;         //!< MQTT port.
  const char *m_mqttUser;     //!< MQTT user.
  const char *m_mqttPassword; //!< MQTT password.
  char **m_mqttTopics;        //!< MQTT topics.
};

#endif

#endif // MQTT_HANDLER_H_