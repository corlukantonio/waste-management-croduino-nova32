#ifndef BLE_HANDLER_H_
#define BLE_HANDLER_H_

#ifdef TARGET_ESP32DEV

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <freertos/semphr.h>

#include <vector>
#include <queue>

#include "components/ble/public/custom_ble_characteristic_callbacks.h"
#include "components/ble/public/custom_ble_server_callbacks.h"
#include "components/pwm_handler/public/pwm_handler.h"
#include "components/task_handler/public/task_handler.h"
#include "components/utils/public/common.h"

/**
 * @class BleHandler
 * @brief BleHandler class.
 */
class BleHandler : public TaskHandler
{

public:
  /********************************
   *                              *
   * Structs.                     *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  /**
   * @struct BleCallback
   * @brief BLE callback.
   */
  struct BleCallback
  {
    String name;
    TCallback cb;
  };

  /**
   * @brief Constructor.
   *
   * @param[in] kpName Pointer to the application name.
   * @param[in] stackDepth Stack depth.
   * @param[in] uxPriority Priority.
   * @param[in] pTaskHandler Pointer to the task handler.
   * @param[in] xCoreID Core ID.
   * @param[in] pPwmHandler PWM handler.
   * @param[in] kBuzzerPin Buzzer pin.
   */
  BleHandler(const char *kpName, uint32_t stackDepth, UBaseType_t uxPriority, TaskHandle_t *pTaskHandler, BaseType_t xCoreID, PwmHandler *pPwmHandle, const uint8_t kBuzzerPin);

  /**
   * @brief Destructor.
   */
  ~BleHandler() = default;

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

  /**
   * @fn AddCallback
   * @brief Adds callback.
   *
   * @param[in] kpName Callback name.
   * @param[in] cb Callback function.
   */
  void AddCallback(const char *kpName, TCallback cb);

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

  SemaphoreHandle_t m_mutex;                                             //!< Mutex.
  PwmHandler *m_pPwmHandler;                                             //!< PWM handler.
  CustomBLECharacteristicCallbacks *m_pCustomBLECharacteristicCallbacks; //!< Custom BLE characteristic callbacks.
  CustomBLEServerCallbacks *m_pCustomBLEServerCallbacks;                 //!< Custom BLE server callbacks.
  BLEAdvertising *m_pBleAdvertising;                                     //!< BLE advertising.
  BLECharacteristic *m_pBleCharacteristic;                               //!< BLE characteristic.
  BLEServer *m_pBleServer;                                               //!< BLE server.
  BLEService *m_pBleService;                                             //!< BLE service.
  std::vector<BleCallback> m_vBleCallbacks;                              //!< BLE callbacks.
  std::queue<String> m_qBleCallbacks;                                    //!< BLE callbacks queue.
  uint8_t m_buzzerPin;                                                   //!< Buzzer pin.
};

#endif

#endif // BLE_HANDLER_H_