#include "components/task_handler/public/task_handler.h"

#ifdef TARGET_ESP32DEV

TaskHandler::TaskHandler(const char *kpName, uint32_t stackDepth, UBaseType_t uxPriority, TaskHandle_t *pTaskHandler, BaseType_t xCoreID)
    : m_pTaskHandler(pTaskHandler)
{
  xTaskCreatePinnedToCore(this->StartTask, kpName, stackDepth, this, uxPriority, pTaskHandler, xCoreID);
}

void TaskHandler::StartTask(void *_this)
{
  ((TaskHandler *)_this)->Task();
  vTaskDelete(*((TaskHandler *)_this)->m_pTaskHandler);
}

#endif