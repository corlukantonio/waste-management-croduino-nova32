#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <DHT.h>
#include <ESP32Time.h>
#include <WiFi.h>

#include "components/utils/public/common.h"
#include "components/utils/public/custom_ble_characteristic_callbacks.h"

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
  /********************************
   *                              *
   * Data members.                *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  static Installer *ms_pInstaller; //!< Singleton instance.
  DHT *pDht;                       //!< Pointer to `DHT` object.
  ESP32Time *pRtc;                 //!< Pointer to `ESP32Time` object.
  uint8_t buzzerPin;               //!< Buzzer pin.
  uint8_t ledPin;                  //!< LED pin.
  uint8_t pirPin;                  //!< PIR motion sensor pin.
  uint8_t pirPinState;             //!< PIR motion sensor pin state.
  uint8_t pirPinStatePrev;         //!< PIR motion sensor pin previous state.
  uint8_t tempHumiSensPin;         //!< Temperature and humidity sensor pin.
  uint8_t ultrasonicSensEchoPin;   //!< Ultrasonic sensor echo pin.
  uint8_t ultrasonicSensTrigPin;   //!< Ultrasonic sensor trig pin.
};