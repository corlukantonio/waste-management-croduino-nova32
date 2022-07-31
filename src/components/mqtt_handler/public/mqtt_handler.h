#ifndef MQTT_HANDLER_H_
#define MQTT_HANDLER_H_

#ifdef TARGET_ESP32DEV

#include <Arduino.h>
#include <EEPROM.h>
#include <freertos/semphr.h>
#include <MQTT.h>
#include <WiFi.h>

#include "components/task_handler/public/task_handler.h"
#include "components/ble/public/ble_handler.h"
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
   * @param[in] pBleHandler Pointer to BLE handler.
   * @param[in] pWiFiHandler Pointer to WiFi handler.
   */
  MqttHandler(const char *kpName, uint32_t stackDepth, UBaseType_t uxPriority, TaskHandle_t *pTaskHandler, BaseType_t xCoreID, BleHandler *pBleHandler, WiFiHandler *pWiFiHandler);

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
   * @fn OnMessage
   * @brief On message.
   *
   * @param[in] topic Topic.
   * @param[in] payload Payload.
   */
  void OnMessage(String &topic, String &payload);

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

  SemaphoreHandle_t m_mutex;                   //!< Mutex.
  BleHandler *m_pBleHandler;                   //!< BLE handler.
  MQTTClient *m_pMqttClient;                   //!< MQTT client.
  WiFiClient *m_pWiFiClient;                   //!< WiFi client.
  WiFiHandler *m_pWiFiHandler;                 //!< WiFi handler.
  Common::MqttCredentials *m_pMqttCredentials; //!< MQTT credentials.
  const Common::BytesPackage *m_pBytesPackage; //!< Bytes package.
  Common::BytesPackage *m_pBytesPackageRead;   //!< Bytes package read.
  char **m_pMqttTopics;                        //!< MQTT topics.
};

#endif

#endif // MQTT_HANDLER_H_