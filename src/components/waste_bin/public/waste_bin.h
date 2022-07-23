#ifndef WASTE_BIN_H_
#define WASTE_BIN_H_

#ifdef TARGET_ESP32DEV

#include <Arduino.h>
#include <DHT.h>
#include <ESP32Time.h>
#include <freertos/semphr.h>

#include <vector>

#include "components/ble/public/ble_handler.h"
#include "components/mqtt_handler/public/mqtt_handler.h"
#include "components/task_handler/public/task_handler.h"
#include "components/utils/public/common.h"
#include "components/wifi_handler/public/wifi_handler.h"

/**
 * @class WasteBin
 * @brief Waste bin application.
 */
class WasteBin : public TaskHandler
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
   * @param[in] pMqttHandler Pointer to MQTT handler.
   * @param[in] kBuzzerPin Buzzer pin.
   * @param[in] kLedPin LED pin.
   * @param[in] kPirPin PIR motion sensor pin.
   * @param[in] kTempHumiSensPin Temperature and humidity sensor pin.
   * @param[in] kUltrasonicSensEchoPin Ultrasonic sensor echo pin.
   * @param[in] kUltrasonicSensTrigPin Ultrasonic sensor trig pin.
   */
  WasteBin(const char *kpName, uint32_t stackDepth, UBaseType_t uxPriority, TaskHandle_t *pTaskHandler, BaseType_t xCoreID, BleHandler *pBleHandler, WiFiHandler *pWiFiHandler, MqttHandler *pMqttHandler, const uint8_t kBuzzerPin, const uint8_t kLedPin, const uint8_t kPirPin, const uint8_t kTempHumiSensPin, const uint8_t kUltrasonicSensEchoPin, const uint8_t kUltrasonicSensTrigPin);

  /**
   * @brief Destructor.
   */
  ~WasteBin() = default;

  /********************************
   *                              *
   * Methods.                     *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

protected:
  virtual void Task() override final;

private:
  /**
   * @fn SendObjectRegistrationRequest
   * @brief Send object registration request.
   *
   * @param kData Data.
   */
  void SendObjectRegistrationRequest(const String kData = "") const;

  /**
   * @fn SendObjectActivationRequest
   * @brief Send object activation request.
   *
   * @param kData Data.
   */
  void SendObjectActivationRequest(const String kData = "") const;

  /**
   * @fn SendObjectSettingsPackage
   * @brief Send object settings package.
   *
   * @param kData Data.
   */
  void SendObjectSettingsPackage(const String kData = "") const;

  /**
   * @fn SendObjectRecordConfigRequest
   * @brief Send object record config request.
   *
   * @param kData Data.
   */
  void SendObjectRecordConfigRequest(const String kData = "") const;

  /**
   * @fn SendObjectRecordConfigApprovalRequest
   * @brief Send object record config approval request.
   *
   * @param kData Data.
   */
  void SendObjectRecordConfigApprovalRequest(const String kData = "") const;

  /**
   * @fn SendRecord
   * @brief Sends data to the MQTT topic.
   *
   * @param kData Data.
   */
  void SendRecord(const String kData = "") const;

  void ReadSensorValues();

  /********************************
   *                              *
   * Data members.                *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  SemaphoreHandle_t m_mutex;               //!< Mutex.
  BleHandler *m_pBleHandler;               //!< BLE handler.
  WiFiHandler *m_pWiFiHandler;             //!< WiFi handler.
  MqttHandler *m_pMqttHandler;             //!< MQTT handler.
  DHT *m_pDht;                             //!< DHT.
  ESP32Time *m_pRtc;                       //!< RTC.
  unsigned long m_startTime;               //!< Start time.
  unsigned long m_currentTime;             //!< Current time.
  double m_duration, m_cm;                 //!< Ultrasonic sensor value.
  double m_humidity, m_temperatureCelsius; //!< Temperature and humidity sensor value.
  uint8_t m_buzzerPin;                     //!< Buzzer pin.
  uint8_t m_ledPin;                        //!< LED pin.
  uint8_t m_pirPin;                        //!< PIR motion sensor pin.
  uint8_t m_pirPinState;                   //!< PIR motion sensor pin state.
  uint8_t m_pirPinStatePrev;               //!< PIR motion sensor pin previous state.
  uint8_t m_tempHumiSensPin;               //!< Temperature and humidity sensor pin.
  uint8_t m_ultrasonicSensEchoPin;         //!< Ultrasonic sensor echo pin.
  uint8_t m_ultrasonicSensTrigPin;         //!< Ultrasonic sensor trig pin.
  bool m_isRecordPackageSent;              //!< Is record package sent.
};

#endif

#endif // WASTE_BIN_H_