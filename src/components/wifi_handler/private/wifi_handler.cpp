#include "components/wifi_handler/public/wifi_handler.h"

#ifdef TARGET_ESP32DEV

WiFiHandler::WiFiHandler(const char *pName, uint32_t stackDepth, TaskHandle_t *pTaskHandler)
    : TaskHandler(pName, stackDepth, pTaskHandler)
{
  m_mutex = xSemaphoreCreateMutex();

  if (m_mutex == NULL)
  {
    Serial.println("Mutex can not be created.");
  }
}

void WiFiHandler::AddWiFiCredentials(const char *pWiFiSsid, const char *pWiFiPassword)
{
  xSemaphoreTake(m_mutex, portMAX_DELAY);

  bool isMax = true;

  for (size_t i = 0; i < WIFI_MAX_CREDENTIALS; i++)
  {
    if (m_pWiFiSsids[i] == nullptr && m_pWiFiPasswords[i] == nullptr)
    {
      m_pWiFiSsids[i] = (char *)malloc(sizeof(char) * 30);
      m_pWiFiPasswords[i] = (char *)malloc(sizeof(char) * 30);

      memcpy(m_pWiFiSsids[i], pWiFiSsid, sizeof(char) * 30);
      memcpy(m_pWiFiPasswords[i], pWiFiPassword, sizeof(char) * 30);

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

  size_t index = 0;
  int16_t n = 0;
  int32_t maxRssi = 0;
  unsigned long startMillis = 0;
  unsigned long currentMillis = 0;

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

  for (size_t i = 0; i < WIFI_MAX_CREDENTIALS; i++)
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
    delay(5000);

    Serial.println(".");

    currentMillis = millis();

    if (currentMillis - startMillis >= WIFI_WAIT_CONNECTION_MS)
    {
      break;
    }

    WiFi.reconnect();
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgWiFiConnected, 1, WiFi.SSID(index).c_str()));
  }

  xSemaphoreGive(m_mutex);
}

void WiFiHandler::Task()
{
  m_pWiFiSsids = (char **)malloc(sizeof(char *) * 5);
  m_pWiFiPasswords = (char **)malloc(sizeof(char *) * 5);

  for (size_t i = 0; i < WIFI_MAX_CREDENTIALS; i++)
  {
    m_pWiFiSsids[i] = nullptr;
    m_pWiFiPasswords[i] = nullptr;
  }

  AddWiFiCredentials("HTEronet-NMHA85", "45803511");
  AddWiFiCredentials("MYA-L41", "03b4a12d");

  WiFi.mode(WIFI_STA);

  ConnectToWiFi();
}

#endif