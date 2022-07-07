#include "components/pwm_handler/public/pwm_handler.h"

#ifdef TARGET_ESP32DEV

PwmHandler::PwmHandler(const char *kpName, uint32_t stackDepth, UBaseType_t uxPriority, TaskHandle_t *pTaskHandler, BaseType_t xCoreID)
    : TaskHandler(kpName, stackDepth, uxPriority, pTaskHandler, xCoreID)
{
  m_mutex = xSemaphoreCreateMutex();

  if (m_mutex == NULL)
  {
    Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMutexNotCreated));
  }
}

void PwmHandler::PlayConnect(const uint8_t kBuzzerPin) const
{
  xSemaphoreTake(m_mutex, portMAX_DELAY);

  unsigned long startTime = millis();
  unsigned long currentTime = millis();

  // D# (octave 4)

  while (currentTime - startTime < 100)
  {
    analogWrite(kBuzzerPin, 255);
    delayMicroseconds(1607.05);
    analogWrite(kBuzzerPin, 0);
    delayMicroseconds(1607.05);

    currentTime = millis();
  }

  delay(5);

  startTime = millis();
  currentTime = millis();

  // F# (octave 4)

  while (currentTime - startTime < 100)
  {
    analogWrite(kBuzzerPin, 255);
    delayMicroseconds(1351.39);
    analogWrite(kBuzzerPin, 0);
    delayMicroseconds(1351.39);

    currentTime = millis();
  }

  delay(5);

  startTime = millis();
  currentTime = millis();

  // B (octave 4)

  while (currentTime - startTime < 100)
  {
    analogWrite(kBuzzerPin, 255);
    delayMicroseconds(1012.39);
    analogWrite(kBuzzerPin, 0);
    delayMicroseconds(1012.39);

    currentTime = millis();
  }

  xSemaphoreGive(m_mutex);
}

void PwmHandler::PlayDisconnect(const uint8_t kBuzzerPin) const
{
  xSemaphoreTake(m_mutex, portMAX_DELAY);

  unsigned long startTime = millis();
  unsigned long currentTime = millis();

  // B (octave 4)

  while (currentTime - startTime < 200)
  {
    analogWrite(kBuzzerPin, 255);
    delayMicroseconds(1012.39);
    analogWrite(kBuzzerPin, 0);
    delayMicroseconds(1012.39);

    currentTime = millis();
  }

  delay(5);

  startTime = millis();
  currentTime = millis();

  // F# (octave 4)

  while (currentTime - startTime < 200)
  {
    analogWrite(kBuzzerPin, 255);
    delayMicroseconds(1351.39);
    analogWrite(kBuzzerPin, 0);
    delayMicroseconds(1351.39);

    currentTime = millis();
  }

  xSemaphoreGive(m_mutex);
}

void PwmHandler::Task()
{
  while (true)
  {
#if LOG_STACK == 1
    Serial.print("PwmHandler: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
#endif

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

#endif