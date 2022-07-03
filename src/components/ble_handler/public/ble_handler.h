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

#include "components/installer/public/installer.h"
#include "components/task_handler/public/task_handler.h"
#include "components/utils/public/common.h"
#include "components/utils/public/custom_ble_characteristic_callbacks.h"
#include "components/utils/public/custom_ble_server_callbacks.h"

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
   * @param[in] pName Pointer to the application name.
   * @param[in] stackDepth Stack depth.
   * @param[in] pTaskHandler Pointer to the task handler.
   */
  BleHandler(const char *pName, uint32_t stackDepth, TaskHandle_t *pTaskHandler);

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
   * @brief Adds callback.
   *
   * @param[in] name Callback name.
   * @param[in] cb Callback function.
   */
  void AddCallback(const char *name, TCallback cb);

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
  CustomBLECharacteristicCallbacks *m_pCustomBLECharacteristicCallbacks; //!< Custom BLE characteristic callbacks.
  CustomBLEServerCallbacks *m_pCustomBLEServerCallbacks;                 //!< Custom BLE server callbacks.
  BLEAdvertising *m_pBleAdvertising;                                     //!< BLE advertising.
  BLECharacteristic *m_pBleCharacteristic;                               //!< BLE characteristic.
  BLEServer *m_pBleServer;                                               //!< BLE server.
  BLEService *m_pBleService;                                             //!< BLE service.
  std::vector<BleCallback> m_vBleCallbacks;                              //!< BLE callbacks.
  std::queue<String> m_qBleCallbacks;                                    //!< BLE callbacks queue.
};

#endif

#endif // BLE_HANDLER_H_