#ifndef MQTT_HANDLER_H_
#define MQTT_HANDLER_H_

#ifdef TARGET_ESP32DEV

#include <Arduino.h>
#include <freertos/semphr.h>
#include <MQTT.h>
#include <WiFi.h>

#include "components/task_handler/public/task_handler.h"
#include "components/utils/public/common.h"
#include "components/wifi_handler/public/wifi_handler.h"

/**
 * @class MqttHandler
 * @brief MqttHandler class.
 */
class MqttHandler : public TaskHandler
{

public:
  /**
   * @brief Constructor.
   *
   * @param[in] kpName Pointer to the application name.
   * @param[in] stackDepth Stack depth.
   * @param[in] uxPriority Priority.
   * @param[in] pTaskHandler Pointer to the task handler.
   * @param[in] xCoreID Core ID.
   * @param[in] pWiFiHandler Pointer to WiFi handler.
   */
  MqttHandler(const char *kpName, uint32_t stackDepth, UBaseType_t uxPriority, TaskHandle_t *pTaskHandler, BaseType_t xCoreID, WiFiHandler *pWiFiHandler);

  /**
   * @brief Destructor.
   */
  ~MqttHandler() = default;

  /********************************
   *                              *
   * Methods.                     *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  /**
   * @fn GetMqttTopics
   * @brief Gets the MQTT topics.
   *
   * @return MQTT topics.
   */
  char **GetMqttTopics() const;

  /**
   * @fn GetMqttClientObject
   * @brief Gets MQTT client object.
   *
   * @return MQTT client object.
   */
  MQTTClient *GetMqttClientObject() const;

protected:
  virtual void Task() override final;

private:
  /**
   * @fn AddMqttTopic
   * @brief Adds MQTT topic.
   *
   * @param[in] kpMqttTopic Pointer to the MQTT topic.
   */
  void AddMqttTopic(const char *kpMqttTopic);

  /********************************
   *                              *
   * Data members.                *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  SemaphoreHandle_t m_mutex;   //!< Mutex.
  MQTTClient *m_pMqttClient;   //!< MQTT client.
  WiFiClient *m_pWiFiClient;   //!< WiFi client.
  WiFiHandler *m_pWiFiHandler; //!< WiFi handler.
  const char *m_pMqttServer;   //!< MQTT server.
  int32_t m_pMqttPort;         //!< MQTT port.
  const char *m_pMqttUser;     //!< MQTT user.
  const char *m_pMqttPassword; //!< MQTT password.
  char **m_pMqttTopics;        //!< MQTT topics.
};

#endif

#endif // MQTT_HANDLER_H_