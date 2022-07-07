#ifndef PWM_HANDLER_H_
#define PWM_HANDLER_H_

#ifdef TARGET_ESP32DEV

#include <Arduino.h>

#include <queue>

#include "components/task_handler/public/task_handler.h"
#include "components/utils/public/common.h"

/**
 * @class PwmHandler
 * @brief PwmHandler class.
 */
class PwmHandler : public TaskHandler
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
  PwmHandler(const char *kpName, uint32_t stackDepth, UBaseType_t uxPriority, TaskHandle_t *pTaskHandler, BaseType_t xCoreID);

  /**
   * @brief Destructor.
   */
  ~PwmHandler() = default;

  /********************************
   *                              *
   * Methods.                     *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  /**
   * @brief Plays connect sound.
   *
   * @param[in] kBuzzerPin Buzzer pin.
   */
  void PlayConnect(const uint8_t kBuzzerPin) const;

  /**
   * @brief Plays disconnect sound.
   *
   * @param[in] kBuzzerPin Buzzer pin.
   */
  void PlayDisconnect(const uint8_t kBuzzerPin) const;

protected:
  virtual void Task() override final;

private:
  /********************************
   *                              *
   * Data members.                *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  SemaphoreHandle_t m_mutex; //!< Mutex.
};

#endif

#endif // PWM_HANDLER_H_