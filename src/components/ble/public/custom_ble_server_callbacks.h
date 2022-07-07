#ifndef CUSTOM_BLE_SERVER_CALLBACKS_H_
#define CUSTOM_BLE_SERVER_CALLBACKS_H_

#ifdef TARGET_ESP32DEV

#include <Arduino.h>
#include <BLEServer.h>

#include "components/pwm_handler/public/pwm_handler.h"
#include "components/utils/public/common.h"

/**
 * @class CustomBLEServerCallbacks
 * @brief CustomBLEServerCallbacks class.
 */
class CustomBLEServerCallbacks : public BLEServerCallbacks
{

public:
  /**
   * @brief Constructor.
   *
   * @param[in] pPwmHandler PWM handler.
   * @param[in] kBuzzerPin Buzzer pin.
   */
  CustomBLEServerCallbacks(PwmHandler *pPwmHandler, const uint8_t kBuzzerPin);

  /********************************
   *                              *
   * Methods.                     *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  /**
   * @fn GetIsDeviceConnected
   * @brief Gets the connection status of the device via BLE.
   *
   * @return Device connection status via BLE.
   */
  bool GetIsDeviceConnected() const;

private:
  virtual void onConnect(BLEServer *pServer) override final;

  virtual void onDisconnect(BLEServer *pServer) override final;

  /********************************
   *                              *
   * Data members.                *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  PwmHandler *m_pPwmHandler; //!< PWM handler.
  uint8_t m_buzzerPin;       //!< Buzzer pin.
  bool m_isDeviceConnected;  //!< Is device connected.
};

#endif

#endif // CUSTOM_BLE_SERVER_CALLBACKS_H_