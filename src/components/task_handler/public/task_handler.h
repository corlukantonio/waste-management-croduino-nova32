#ifndef TASK_HANDLER_H_
#define TASK_HANDLER_H_

#ifdef TARGET_ESP32DEV

#include <Arduino.h>

/**
 * @class TaskHandler
 * @brief TaskHandler class.
 */
class TaskHandler
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
  TaskHandler(const char *kpName, uint32_t stackDepth, UBaseType_t uxPriority, TaskHandle_t *pTaskHandler, BaseType_t xCoreID);

  /**
   * @brief Destructor.
   */
  ~TaskHandler() = default;

protected:
  /********************************
   *                              *
   * Methods.                     *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  /**
   * @fn Task
   * @brief Main entry function for the task implementation.
   */
  virtual void Task() = 0;

private:
  /**
   * @fn StartTask
   * @brief Main entry function for starting the task.
   */
  static void StartTask(void *);

  /********************************
   *                              *
   * Data members.                *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  TaskHandle_t *m_pTaskHandler; //!< Pointer to the task handler.
};

#endif

#endif // TASK_HANDLER_H_