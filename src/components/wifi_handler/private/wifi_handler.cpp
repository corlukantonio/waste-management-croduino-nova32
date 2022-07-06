#include "components/wifi_handler/public/wifi_handler.h"

#ifdef TARGET_ESP32DEV

WiFiHandler::WiFiHandler(const char *kpName, uint32_t stackDepth, UBaseType_t uxPriority, TaskHandle_t *pTaskHandler, BaseType_t xCoreID)
    : TaskHandler(kpName, stackDepth, uxPriority, pTaskHandler, xCoreID)
{
  m_mutex = xSemaphoreCreateMutex();

  if (m_mutex == NULL)
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMutexNotCreated));
  }

  m_pWiFiSsids = (char **)malloc(sizeof(char *) * WIFI_CREDS_MAX);
  m_pWiFiPasswords = (char **)malloc(sizeof(char *) * WIFI_CREDS_MAX);

  for (size_t i = 0; i < WIFI_CREDS_MAX; i++)
  {
    m_pWiFiSsids[i] = nullptr;
    m_pWiFiPasswords[i] = nullptr;
  }

  m_isDeviceConnected = false;
}

bool WiFiHandler::GetIsDeviceConnected() const
{
  return m_isDeviceConnected;
}

WiFiClass *WiFiHandler::GetWiFiClassObject() const
{
  return &WiFi;
}

void WiFiHandler::AddWiFiCredentials(const char *kpWiFiSsid, const char *kpWiFiPassword)
{
  xSemaphoreTake(m_mutex, portMAX_DELAY);

  bool isMax = true;

  for (size_t i = 0; i < WIFI_CREDS_MAX; i++)
  {
    if (m_pWiFiSsids[i] == nullptr && m_pWiFiPasswords[i] == nullptr)
    {
      m_pWiFiSsids[i] = (char *)malloc(sizeof(char) * WIFI_CRED_MAX_CHAR);
      m_pWiFiPasswords[i] = (char *)malloc(sizeof(char) * WIFI_CRED_MAX_CHAR);

      memcpy(m_pWiFiSsids[i], kpWiFiSsid, sizeof(char) * WIFI_CRED_MAX_CHAR);
      memcpy(m_pWiFiPasswords[i], kpWiFiPassword, sizeof(char) * WIFI_CRED_MAX_CHAR);

      isMax = false;

      break;
    }
  }

  if (isMax)
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgWiFiMaxCredentialsReached));
  }

  xSemaphoreGive(m_mutex);
}

void WiFiHandler::ConnectToWiFi()
{
  xSemaphoreTake(m_mutex, portMAX_DELAY);

  unsigned long startMillis = 0;
  unsigned long currentMillis = 0;
  size_t index = 0;
  int16_t n = 0;
  int32_t maxRssi = 0;

  if (WiFi.status() == WL_CONNECTED)
  {
    WiFi.disconnect();
  }

  n = WiFi.scanNetworks();

  if (n == 0)
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgWiFiNoNetworksFound));
  }
  else
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgWiFiNetworksFound, 1, n));

    for (size_t i = 0; i < n; ++i)
    {
      Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgWiFiSsidFound, 3, i, WiFi.SSID(i).c_str(), WiFi.RSSI(i)));

      if (maxRssi < WiFi.RSSI(i) || maxRssi == 0)
      {
        index = i;
        maxRssi = WiFi.RSSI(i);
      }
    }
  }

  for (size_t i = 0; i < WIFI_CREDS_MAX; i++)
  {
    if (strcmp(WiFi.SSID(index).c_str(), m_pWiFiSsids[i]) == 0)
    {
      WiFi.begin(m_pWiFiSsids[i], m_pWiFiPasswords[i]);

      Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgWiFiConnecting, 1, m_pWiFiSsids[i]));

      break;
    }
  }

  startMillis = millis();

  while (WiFi.status() != WL_CONNECTED)
  {
    WiFi.reconnect();

    currentMillis = millis();

    if (currentMillis - startMillis >= WIFI_WAIT_CONNECTION_MS)
    {
      break;
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgWiFiConnected, 1, WiFi.SSID(index).c_str()));

    m_isDeviceConnected = true;
  }

  xSemaphoreGive(m_mutex);
}

void WiFiHandler::Task()
{
  WiFi.mode(WIFI_STA);

  AddWiFiCredentials("HTEronet-NMHA85", "45803511");
  AddWiFiCredentials("MYA-L41", "03b4a12d");

  ConnectToWiFi();

  while (true)
  {
    // Serial.print("WiFiHandler: ");
    // Serial.println(uxTaskGetStackHighWaterMark(NULL));

    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

#endif