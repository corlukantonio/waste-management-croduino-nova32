#ifdef TARGET_ESP32DEV
#include <Arduino.h>
#else
#include <iostream>
#endif

/********************************
 *                              *
 * Defines.                     *
 * ---------------------------- *
 *                              *
 *                              *
 ********************************/

#define SERIAL_BAUD 9600

#define MAX_WIFI_CREDENTIALS 5
#define WAIT_WIFI_CONNECTION_MS 15000

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
 * @class CommonData
 * @brief Common data class.
 */
class CommonData
{

public:
  /********************************
   *                              *
   * Enums.                       *
   * ---------------------------- *
   *                              *
   *                              *
   ********************************/

  /**
   * @brief Alert message enum.
   */
  enum AlertMessageEnum : uint8_t
  {
    ALERT_MSG_WIFI_CONNECTING,
    ALERT_MSG_WIFI_CONNECTED,
    ALERT_MSG_WIFI_SSID_FOUND,
    ALERT_MSG_NETWORKS_FOUND,
    ALERT_MSG_NO_NETWORKS_FOUND,
    ALERT_MSG_MAX_WIFI_CREDENTIALS,
    ALERT_MSG_MQTT_CONNECTED
  };

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
   *
   * @tparam T
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
};