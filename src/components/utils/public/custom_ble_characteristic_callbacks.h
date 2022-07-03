#ifndef CUSTOM_BLE_CHARACTERISTIC_CALLBACKS_H_
#define CUSTOM_BLE_CHARACTERISTIC_CALLBACKS_H_

#ifdef TARGET_ESP32DEV

#include <Arduino.h>
#include <BLEServer.h>

#include <queue>

/**
 * @class CustomBLECharacteristicCallbacks
 * @brief CustomBLECharacteristicCallbacks class.
 */
class CustomBLECharacteristicCallbacks : public BLECharacteristicCallbacks
{
public:
  /**
   * @brief Constructor.
   *
   * @param[in] pqCallbacks Pointer to callbacks queue.
   */
  CustomBLECharacteristicCallbacks(std::queue<String> *pqCallbacks);

private:
  /********************************
   *                              *
   * Methods.                     *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  virtual void onWrite(BLECharacteristic *pCharacteristic) override final;

  /********************************
   *                              *
   * Data members.                *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  std::queue<String> *m_pqBleCallbacks; //!< BLE callbacks queue.
};

#endif

#endif // CUSTOM_BLE_CHARACTERISTIC_CALLBACKS_H_