#ifndef COMMON_H_
#define COMMON_H_

#ifdef TARGET_ESP32DEV
#include <Arduino.h>
#else
#include <iostream>
#include <cstring>
#endif

#include <type_traits>
#include <vector>

#include "components/utils/public/common_data.h"

#ifdef TARGET_ESP32DEV
typedef std::function<void(String)> TCallback;
#endif

/**
 * @class Common
 * @brief Common class.
 * @brief The Singleton class defines the `GetInstance` method that serves as an alternative to constructor and lets clients access the same instance of this class over and over. Common provides structures and methods used in multiple files.
 */
class Common : public CommonData
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
   * @brief Singletons should not be cloneable.
   *
   * @param[in, out] rCommon Reference to `Common` object.
   */
  Common(Common &rCommon) = delete;

  /**
   * @brief Singletons should not be assignable.
   */
  void operator=(const Common &) = delete;

  /**
   * @fn GetInstance
   * @brief This is the static method that controls the access to the singleton instance. On the first run, it creates a singleton object and places it into the static field. On subsequent runs, it returns the client existing object stored in the static field.
   *
   * @return Singleton instance.
   */
  static Common *GetInstance();

  /**
   * @fn GetAlertMessage
   * @brief Gets the alert message.
   *
   * @param[in] alertMessage Alert message.
   * @param[in] n Number of arguments.
   * @param ...
   * @return constant pointer to char array.
   */
  const char *GetAlertMessage(AlertMessageEnum alertMessage, size_t n = 0, ...) const;

#ifdef TARGET_ESP32DEV
  /**
   * @fn GetArgs
   * @brief Gets arguments.
   *
   * @param[in] kData Data.
   * @return Constant value of `vector<String>` data type.
   */
  const std::vector<String> GetArgs(const String kData) const;
#endif

  /**
   * @fn GetCrc
   * @brief Calculates and retrieves CRC (Cyclic Redundancy Check) from the given data.
   *
   * @param[in] kpData Constant pointer to value of `uint8_t` data type.
   * @param[in] kSize Constant value of `size_t` data type.
   * @return Constant value of `uint8_t` data type that represents the calculated CRC.
   */
  const uint8_t GetCrc(const uint8_t *kpData, const size_t kSize) const;

  /**
   * @fn GetPackageWithArgsInBytes
   * @brief Gets a package with arguments in bytes.
   *
   * @tparam TPackage
   * @tparam TArgs
   * @param[in, out] pBasePackage Double pointer to `TPackage` object.
   * @param[in] args Arguments of the `TArgs` types.
   * @return const BytesPackage*
   */
  template <typename TPackage, typename... TArgs>
  const BytesPackage *GetPackageWithArgsInBytes(TPackage **pBasePackage, TArgs... args) const;

  /**
   * @fn GetStructFromBytes
   * @brief Gets structure from bytes.
   *
   * @tparam T
   * @param[in] kpData Data.
   * @return Pointer to structure.
   */
  template <typename T>
  T *GetStructFromBytes(const uint8_t *kpData) const;

protected:
  /**
   * @brief The Singleton's constructor should always be private to prevent direct construction calls with the `new` operator.
   */
  Common() = default;

  /**
   * @brief The Singleton's destructor should always be private to prevent direct desctruction calls with the `delete` operator.
   */
  ~Common() = default;

private:
  /**
   * @fn InsertArgToByteArray
   * @brief Inserts argument to byte array.
   *
   * @param[in, out] rCounter Reference to counter.
   * @param[in, out] pBytes Pointer to byte array.
   */
  void InsertArgToByteArray(size_t &rCounter, uint8_t *pBytes) const;

  /**
   * @fn InsertArgToByteArray
   * @brief Inserts argument to byte array.
   *
   * @tparam TArg
   * @tparam TArgs
   * @param[in, out] rCounter Reference to counter.
   * @param[in, out] pBytes Pointer to byte array.
   * @param[in] arg Argument of the `TArg` type.
   * @param[in] args Arguments of the `TArgs` types.
   */
  template <typename TArg, typename... TArgs>
  void InsertArgToByteArray(size_t &rCounter, uint8_t *pBytes, TArg arg, TArgs... args) const;

  /**
   * @fn GetArgsLength
   * @brief Gets the total size of all forwarded arguments.
   *
   * @param[in, out] rArgsLength Reference to arguments length.
   */
  void GetArgsLength(size_t &rArgsLength) const;

  /**
   * @fn GetArgsLength
   * @brief Gets the total size of all forwarded arguments.
   *
   * @tparam TArg
   * @tparam TArgs
   * @param[in, out] rArgsLength Reference to arguments length.
   * @param[in] arg Argument of the `TArg` type.
   * @param[in] args Arguments of the `TArgs` types.
   */
  template <typename TArg, typename... TArgs>
  void GetArgsLength(size_t &rArgsLength, TArg arg, TArgs... args) const;

  /********************************
   *                              *
   * Data members.                *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  static Common *ms_pCommon; //!< Singleton instance.
};

#endif // COMMON_H_