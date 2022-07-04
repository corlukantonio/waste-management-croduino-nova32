#ifndef TASK_HANDLER_H_
#define TASK_HANDLER_H_

#ifdef TARGET_ESP32DEV

#include <Arduino.h>

/**
 * @class TaskHandler
 * @brief TaskHandler.
 * @brief TaskHandler class.
 */
class TaskHandler
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
  TaskHandler(const char *pName, uint32_t stackDepth, TaskHandle_t *pTaskHandler);

  /**
   * @brief Destructor.
   */
  ~TaskHandler() = default;

protected:
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