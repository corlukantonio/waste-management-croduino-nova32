#ifndef WIFI_HANDLER_H_
#define WIFI_HANDLER_H_

#ifdef TARGET_ESP32DEV

#include <Arduino.h>
#include <freertos/semphr.h>
#include <WiFi.h>

#include "components/task_handler/public/task_handler.h"
#include "components/utils/public/common.h"

class WiFiHandler : public TaskHandler
{

public:
  /**
   * @brief Constructor.
   *
   * @param[in] pName Pointer to the application name.
   * @param[in] stackDepth Stack depth.
   * @param[in] pTaskHandler Pointer to the task handler.
   */
  WiFiHandler(const char *pName, uint32_t stackDepth, TaskHandle_t *pTaskHandler);

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
   * @param[in] pWiFiSsid Pointer to the WiFi SSID.
   * @param[in] pWiFiPassword Pointer to the WiFi password.
   */
  void AddWiFiCredentials(const char *pWiFiSsid, const char *pWiFiPassword);

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