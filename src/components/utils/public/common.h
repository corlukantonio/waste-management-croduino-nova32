#ifdef TARGET_ESP32DEV
#include <Arduino.h>
#else
#include <iostream>
#include <cstring>
#endif

#include <type_traits>

/********************************
 *                              *
 * Defines.                     *
 * ---------------------------- *
 *                              *
 *                              *
 ********************************/

#define SERIAL_BAUD 9600

#define SERVICE_UUID "5f93f3f7-75ae-4b70-bb4a-e6859be4b105"
#define CHARACTERISTIC_UUID "0e746bf8-fb7f-4d7e-9fda-b8911cf1d599"

#define OBJ_REG_REQ_PKG 0x01
#define OBJ_REG_REQ_PKG_V 0x01

#define OBJ_ACT_REQ_PKG 0x02
#define OBJ_ACT_REQ_PKG_V 0x01

#define OBJ_REC_CFG_REQ_PKG 0x03
#define OBJ_REC_CFG_REQ_PKG_V 0x01

#define OBJ_REC_CFG_APV_REQ_PKG 0x04
#define OBJ_REC_CFG_APV_REQ_PKG_V 0x01

#define OBJ_REC_BASE_PKG 0x05
#define OBJ_REC_BASE_PKG_V 0x01

/**
 * @class Common
 * @brief Common class.
 * @brief The Singleton class defines the `GetInstance` method that serves as an alternative to constructor and lets clients access the same instance of this class over and over. Common provides structures and methods used in multiple files.
 */
class Common
{

public:
  /********************************
   *                              *
   * Structs.                     *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

#pragma pack(push, 1)
  /**
   * @struct BytesPackage
   * @brief Bytes package.
   */
  struct BytesPackage
  {
    uint8_t *pBytes; //!< Byte array.
    size_t length;   //!< Byte array length.
  };
#pragma pack(pop)

#pragma pack(push, 1)
  /**
   * @struct ObjectRegistrationRequestPackage
   * @brief Object registration request package. This package is sent when the device wants to be registered.
   */
  struct ObjectRegistrationRequestPackage
  {
    uint8_t packageType;
    uint8_t packageVersion;
    uint8_t mac[6];
    uint8_t rtc[6];
    uint8_t crc;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  /**
   * @struct ObjectActivationRequestPackage
   * @brief Object activation request package. This package is sent when the device wants to activate its registration.
   */
  struct ObjectActivationRequestPackage
  {
    uint8_t packageType;
    uint8_t packageVersion;
    uint8_t mac[6];
    uint8_t rtc[6];
    uint8_t activationCode[4];
    uint8_t crc;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  /**
   * @struct ObjectRecordConfigRequestPackage
   * @brief Object record config request package.
   */
  struct ObjectRecordConfigRequestPackage
  {
    uint8_t packageType;
    uint8_t packageVersion;
    uint8_t mac[6];
    uint8_t rtc[6];
    uint8_t recordBasePackageVersion;
    uint8_t numberOfValues;
    uint8_t crc;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  /**
   * @struct ObjectRecordConfigApprovalRequestPackage
   * @brief Object record config approval request package.
   */
  struct ObjectRecordConfigApprovalRequestPackage
  {
    uint8_t packageType;
    uint8_t packageVersion;
    uint8_t mac[6];
    uint8_t rtc[6];
    uint8_t approvalCode[4];
    uint8_t crc;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  /**
   * @struct ObjectRecordValue
   * @brief Object record value.
   */
  template <class T>
  struct ObjectRecordValue
  {
    uint8_t type;
    T value;
  };
#pragma pack(pop)

#pragma pack(push, 1)
  /**
   * @brief ObjectRecordBasePackage
   * @brief Object record base package.
   */
  struct ObjectRecordBasePackage
  {
    uint8_t packageType;
    uint8_t packageVersion;
    uint8_t mac[6];
    uint8_t rtc[6];
    uint8_t numberOfValues;
    // ...
    // This space will be occupied by objects of type `ObjectRecordValue`.
    // ...
    int16_t rssi; //!< Received Signal Strength Indicator.
    uint8_t crc;  //!< Cyclic Redundancy Check.
  };
#pragma pack(pop)

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