#ifndef WIFI_HANDLER_H_
#define WIFI_HANDLER_H_

#ifdef TARGET_ESP32DEV

#include <Arduino.h>
#include <freertos/semphr.h>
#include <WiFi.h>

#include "components/task_handler/public/task_handler.h"
#include "components/utils/public/common.h"

/**
 * @class WiFiHandler
 * @brief WiFiHandler class.
 */
class WiFiHandler : public TaskHandler
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
   */
  WiFiHandler(const char *kpName, uint32_t stackDepth, UBaseType_t uxPriority, TaskHandle_t *pTaskHandler, BaseType_t xCoreID);

  /**
   * @brief Destructor.
   */
  ~WiFiHandler() = default;

  /********************************
   *                              *
   * Methods.                     *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  /**
   * @fn GetIsDeviceConnected
   * @brief Gets the connection status of the device via WiFi.
   *
   * @return Device connection status via WiFi.
   */
  bool GetIsDeviceConnected() const;

  /**
   * @fn GetWiFiClassObject
   * @brief Gets WiFi class object.
   *
   * @return WiFi class object.
   */
  WiFiClass *GetWiFiClassObject() const;

protected:
  virtual void Task() override final;

private:
  /**
   * @fn AddWiFiCredentials
   * @brief Adds WiFi credentials.
   *
   * @param[in] kpWiFiSsid Pointer to the WiFi SSID.
   * @param[in] kpWiFiPassword Pointer to the WiFi password.
   */
  void AddWiFiCredentials(const char *kpWiFiSsid, const char *kpWiFiPassword);

  /**
   * @fn ConnectToWiFi
   * @brief Connects to the WiFi network.
   */
  void ConnectToWiFi();

  /********************************
   *                              *
   * Data members.                *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  SemaphoreHandle_t m_mutex; //!< Mutex.
  char **m_pWiFiSsids;       //!< WiFi SSIDs.
  char **m_pWiFiPasswords;   //!< WiFi passwords.
  bool m_isDeviceConnected;  //!< Is device connected.
};

#endif

#endif // WIFI_HANDLER_H_