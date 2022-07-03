#include "components/task_handler/public/task_handler.h"

#ifdef TARGET_ESP32DEV

TaskHandler::TaskHandler(const char *pName, uint32_t stackDepth, TaskHandle_t *pTaskHandler)
    : m_pTaskHandler(pTaskHandler)
{
  xTaskCreate(this->StartTask, pName, stackDepth, this, 1, pTaskHandler);
}

void TaskHandler::StartTask(void *_this)
{
  ((TaskHandler *)_this)->Task();
  vTaskSuspend(*((TaskHandler *)_this)->m_pTaskHandler);
}

#endif