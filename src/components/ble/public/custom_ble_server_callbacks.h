#ifndef CUSTOM_BLE_SERVER_CALLBACKS_H_
#define CUSTOM_BLE_SERVER_CALLBACKS_H_

#ifdef TARGET_ESP32DEV

#include <Arduino.h>
#include <BLEServer.h>

#include "components/utils/public/common.h"

/**
 * @class CustomBLEServerCallbacks
 * @brief CustomBLEServerCallbacks class.
 */
class CustomBLEServerCallbacks : public BLEServerCallbacks
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

  bool m_isDeviceConnected; //!< Is device connected.
};

#endif

#endif // CUSTOM_BLE_SERVER_CALLBACKS_H_