#ifdef TARGET_ESP32DEV
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <DHT.h>
#include <ESP32Time.h>
#include <MQTT.h>
#include <WiFi.h>

#include "components/utils/public/common.h"
#include "components/utils/public/custom_ble_characteristic_callbacks.h"
#include "components/utils/public/custom_ble_server_callbacks.h"

/**
 * @class Installer
 * @brief Installer class.
 * @brief The Singleton class defines the `GetInstance` method that serves as an alternative to constructor and lets clients access the same instance of this class over and over.
 */
class Installer
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
   * @brief Singletons should not be cloneable.
   *
   * @param[in, out] rInstaller Reference to `Installer` object.
   */
  Installer(Installer &rInstaller) = delete;

  /**
   * @brief Singletons should not be assignable.
   */
  void operator=(const Installer &) = delete;

  /**
   * @fn GetInstance
   * @brief This is the static method that controls the access to the singleton instance. On the first run, it creates a singleton object and places it into the static field. On subsequent runs, it returns the client existing object stored in the static field.
   *
   * @return Singleton instance.
   */
  static Installer *GetInstance();

  /**
   * @fn Setup
   * @brief Setup.
   */
  void Setup();

  /**
   * @fn Loop
   * @brief Loop.
   */
  void Loop();

protected:
  /**
   * @brief The Singleton's constructor should always be private to prevent direct construction calls with the `new` operator.
   */
  Installer() = default;

  /**
   * @brief The Singleton's destructor should always be private to prevent direct desctruction calls with the `delete` operator.
   */
  ~Installer() = default;

private:
  /**
   * @fn AddWiFiCredentials
   * @brief Adds WiFi credentials.
   *
   * @param pWiFiSsid Pointer to the char array.
   * @param pWiFiPassword Pointer to the char array.
   */
  void AddWiFiCredentials(const char *pWiFiSsid, const char *pWiFiPassword);

  /**
   * @brief Connect to WiFi network.
   */
  void ConnectToWiFi();

  /********************************
   *                              *
   * Data members.                *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  static Installer *ms_pInstaller;                                       //!< Singleton instance.
  CustomBLECharacteristicCallbacks *m_pCustomBLECharacteristicCallbacks; //!< Custom BLE characteristic callbacks.
  CustomBLEServerCallbacks *m_pCustomBLEServerCallbacks;                 //!< Custom BLE server callbacks.
  BLEAdvertising *m_pBleAdvertising;                                     //!< BLE advertising.
  BLECharacteristic *m_pBleCharacteristic;                               //!< BLE characteristic.
  BLEServer *m_pBleServer;                                               //!< BLE server.
  BLEService *m_pBleService;                                             //!< BLE service.
  DHT *m_pDht;                                                           //!< Pointer to `DHT` object.
  ESP32Time *m_pRtc;                                                     //!< Pointer to `ESP32Time` object.
  MQTTClient *m_pMqttClient;                                             //!< MQTT client.
  WiFiClient *m_pWiFiClient;                                             //!< WiFi client.
  char **m_pWiFiSsids;                                                   //!< WiFi SSIDs.
  char **m_pWiFiPasswords;                                               //!< WiFi passwords.
  const char *m_mqttServer;                                              //!< MQTT server.
  int32_t m_mqttPort;                                                    //!< MQTT port.
  const char *m_mqttUser;                                                //!< MQTT user.
  const char *m_mqttPassword;                                            //!< MQTT password.
  char **m_mqttTopics;                                                   //!< MQTT topics.
  unsigned long m_startTime;                                             //!< Start time.
  unsigned long m_currentTime;                                           //!< Current time.
  uint8_t m_buzzerPin;                                                   //!< Buzzer pin.
  uint8_t m_ledPin;                                                      //!< LED pin.
  uint8_t m_pirPin;                                                      //!< PIR motion sensor pin.
  uint8_t m_pirPinState;                                                 //!< PIR motion sensor pin state.
  uint8_t m_pirPinStatePrev;                                             //!< PIR motion sensor pin previous state.
  uint8_t m_tempHumiSensPin;                                             //!< Temperature and humidity sensor pin.
  uint8_t m_ultrasonicSensEchoPin;                                       //!< Ultrasonic sensor echo pin.
  uint8_t m_ultrasonicSensTrigPin;                                       //!< Ultrasonic sensor trig pin.
};
#endif