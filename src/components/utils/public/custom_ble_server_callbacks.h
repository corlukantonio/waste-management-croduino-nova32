#ifndef CUSTOM_BLE_SERVER_CALLBACKS_H_
#define CUSTOM_BLE_SERVER_CALLBACKS_H_

#ifdef TARGET_ESP32DEV

#include <Arduino.h>
#include <BLEServer.h>

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
   * @brief Gets the connection status of the device via BLE.
   *
   * @return Device connection status via BLE.
   */
  bool GetIsDeviceConnected() const;

private:
  void onConnect(BLEServer *pServer) override final;

  void onDisconnect(BLEServer *pServer) override final;

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