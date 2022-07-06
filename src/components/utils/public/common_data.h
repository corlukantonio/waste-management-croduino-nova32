#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_

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
#define CPU_FREQUENCY 160
#define DEEP_SLEEP_COUNT 60000

#define BLE_DEV_NAME "ESP32 Waste Bin"
#define BLE_SERVICE_UUID "5f93f3f7-75ae-4b70-bb4a-e6859be4b105"
#define BLE_CHARACTERISTIC_UUID "0e746bf8-fb7f-4d7e-9fda-b8911cf1d599"

#define WIFI_CREDS_MAX 5
#define WIFI_CRED_MAX_CHAR 30
#define WIFI_WAIT_CONNECTION_MS 15000

#define MQTT_CLIENT_ID "esp32dev"
#define MQTT_TOPICS_MAX 5
#define MQTT_TOPIC_MAX_CHAR 30
#define MQTT_KEEP_ALIVE 30

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
   * @enum AlertMessageEnum
   * @brief Alert message enum.
   */
  enum AlertMessageEnum : uint8_t
  {
    eAlertMsgBleClientConnected,
    eAlertMsgBleClientDisconnected,
    eAlertMsgWiFiConnecting,
    eAlertMsgWiFiConnected,
    eAlertMsgWiFiSsidFound,
    eAlertMsgWiFiNetworksFound,
    eAlertMsgWiFiNoNetworksFound,
    eAlertMsgWiFiMaxCredentialsReached,
    eAlertMsgMqttConnected,
    eAlertMsgMqttConnectionFailed,
    eAlertMsgMqttMaxTopicsReached,
    eAlertMsgDhtReadFail,
    eAlertMsgMutexNotCreated,
    eAlertMsgDeepSleep
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

#endif // COMMON_DATA_H_