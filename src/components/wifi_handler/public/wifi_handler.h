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
  /********************************
   *                              *
   * Methods.                     *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

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

protected:
  virtual void Task() override final;

private:
  /**
   * @fn AddWiFiCredentials
   * @brief Adds WiFi credentials.
   *
   * @param pWiFiSsid Pointer to the WiFi SSID.
   * @param pWiFiPassword Pointer to the WiFi password.
   */
  void AddWiFiCredentials(const char *pWiFiSsid, const char *pWiFiPassword);

  /**
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
};

#endif

#endif // WIFI_HANDLER_H_