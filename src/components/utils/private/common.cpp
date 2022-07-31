#include "components/utils/public/common.h"

Common *Common::ms_pCommon{nullptr};

Common *Common::GetInstance()
{
  if (ms_pCommon == nullptr)
  {
    ms_pCommon = new Common();
  }

  return ms_pCommon;
}

const char *Common::GetAlertMessage(AlertMessageEnum alertMessage, size_t n, ...) const
{
#ifdef TARGET_ESP32DEV
  String s;

  va_list args;
  va_start(args, n);

  char *message = (char *)malloc(sizeof(char) * 100);

  switch (alertMessage)
  {
  case eAlertMsgBleClientConnected:
    s = String("Connected.");
    break;

  case eAlertMsgBleClientDisconnected:
    s = String("Disconnected.");
    break;

  case eAlertMsgWiFiConnecting:
    s = String("Connecting with \"");
    s.concat(va_arg(args, char *));
    s.concat("\" WiFi network...");
    break;

  case eAlertMsgWiFiConnected:
    s = String("Connected to the \"");
    s.concat(va_arg(args, char *));
    s.concat("\" WiFi network.");
    break;

  case eAlertMsgWiFiSsidFound:
    s = String("");
    s.concat(va_arg(args, size_t) + 1);
    s.concat(": ");
    s.concat(va_arg(args, char *));
    s.concat(" (");
    s.concat(va_arg(args, int32_t));
    s.concat(")");
    break;

  case eAlertMsgWiFiNetworksFound:
    s = String("");
    s.concat(va_arg(args, size_t));
    s.concat(n == 1 ? " network found." : " networks found.");
    break;

  case eAlertMsgWiFiNoNetworksFound:
    s = String("No networks found.");
    break;

  case eAlertMsgWiFiMaxCredentialsReached:
    s = String("The maximum number of WiFi credentials has been reached.");
    break;

  case eAlertMsgMqttConnected:
    s = String("Connected to the ");
    s.concat(va_arg(args, char *));
    s.concat(" MQTT broker.");
    break;

  case eAlertMsgMqttConnectionFailed:
    s = String("MQTT connection failed.");
    break;

  case eAlertMsgMqttMaxTopicsReached:
    s = String("The maximum number of MQTT topics has been reached.");
    break;

  case eAlertMsgDhtReadFail:
    s = String("Failed to read from DHT sensor.");
    break;

  case eAlertMsgMutexNotCreated:
    s = String("Mutex can not be created.");
    break;

  case eAlertMsgDeepSleep:
    s = String("Going to deep sleep.");
    break;

  default:
    s = String("Undefined!");
    break;
  }

  va_end(args);

  strcpy(message, s.c_str());

  return message;
#endif
}

#ifdef TARGET_ESP32DEV
const std::vector<String> Common::GetArgs(const String kData) const
{
  std::vector<String> tokens;

  char *token = strtok((char *)kData.c_str(), " ");

  while (token != NULL)
  {
    tokens.push_back(String(token));
    token = strtok(NULL, " ");
  }

  return tokens;
}
#endif

std::string Common::GetHexStr(unsigned char *pData, int len)
{
  std::stringstream ss;
  ss << std::hex;

  for (int i(0); i < len; ++i)
    ss << std::setw(2) << std::setfill('0') << (int)pData[i];

  return ss.str();
}

const uint8_t Common::GetCrc(const uint8_t *kpData, const size_t kSize) const
{
  uint8_t crc = 0x00;

  for (size_t i = 0; i < kSize; i++)
  {
    crc += (i * kpData[i]);
  }

  return crc;
}

void Common::InsertArgToByteArray(size_t &rCounter, uint8_t *pBytes) const
{
}

template <typename TArg, typename... TArgs>
void Common::InsertArgToByteArray(size_t &rCounter, uint8_t *pBytes, TArg arg, TArgs... args) const
{
  uint8_t *pArgInBytes = reinterpret_cast<uint8_t *>(arg);

  for (size_t i = 0; i < sizeof(*arg); i++, rCounter++)
  {
    pBytes[rCounter] = pArgInBytes[i];
  }

  InsertArgToByteArray(rCounter, pBytes, args...);
}

void Common::GetArgsLength(size_t &rArgsLength) const
{
}

template <typename TArg, typename... TArgs>
void Common::GetArgsLength(size_t &rArgsLength, TArg arg, TArgs... args) const
{
  rArgsLength += sizeof(*arg);

  GetArgsLength(rArgsLength, args...);
}

template <typename TPackage, typename... TArgs>
const Common::BytesPackage *Common::GetPackageWithArgsInBytes(TPackage **pBasePackage, TArgs... args) const
{
  size_t counterOffset = 0;
  size_t argsLength = 0;
  size_t packageLength = 0;

  uint8_t *pBasePackageInBytes = reinterpret_cast<uint8_t *>(*pBasePackage);
  uint8_t *pPackage;

  if (std::is_same<TPackage, MqttCredentials>::value ||
      std::is_same<TPackage, ObjectRegistrationRequestPackage>::value ||
      std::is_same<TPackage, ObjectActivationRequestPackage>::value ||
      std::is_same<TPackage, ObjectRecordConfigRequestPackage>::value ||
      std::is_same<TPackage, ObjectRecordConfigApprovalRequestPackage>::value)
  {
    packageLength = sizeof(TPackage);
    pPackage = (uint8_t *)malloc(packageLength);
    memcpy(pPackage, pBasePackageInBytes, packageLength);
  }
  else if (std::is_same<TPackage, ObjectSettingsPackage>::value ||
           std::is_same<TPackage, ObjectRecordBasePackage>::value)
  {
    GetArgsLength(argsLength, args...);

    packageLength = sizeof(TPackage) + argsLength;
    pPackage = (uint8_t *)malloc(packageLength);

    for (size_t i = 0; i < packageLength; i++)
    {
      if ((std::is_same<TPackage, ObjectSettingsPackage>::value && i == 9) ||
          (std::is_same<TPackage, ObjectRecordBasePackage>::value && i == 15))
      {
        InsertArgToByteArray(i, pPackage, args...);

        counterOffset = argsLength;
      }

      pPackage[i] = pBasePackageInBytes[i - counterOffset];
    }
  }

  BytesPackage *pBytesPackage = (BytesPackage *)malloc(sizeof(BytesPackage));

  pBytesPackage->pBytes = pPackage;
  pBytesPackage->length = packageLength;

  return pBytesPackage;
}

// Explicit instantiations for `GetPackageWithArgsInBytes` method.

template const Common::BytesPackage *Common::GetPackageWithArgsInBytes<
    Common::MqttCredentials>(
    Common::MqttCredentials **pMqttCredentials) const;

template const Common::BytesPackage *Common::GetPackageWithArgsInBytes<
    Common::ObjectRegistrationRequestPackage>(
    Common::ObjectRegistrationRequestPackage **pObjectRegistrationRequestPackage) const;

template const Common::BytesPackage *Common::GetPackageWithArgsInBytes<
    Common::ObjectActivationRequestPackage>(
    Common::ObjectActivationRequestPackage **pObjectActivationRequestPackage) const;

template const Common::BytesPackage *Common::GetPackageWithArgsInBytes<
    Common::ObjectSettingsPackage,
    Common::ObjectRecordValue<double> *>(
    Common::ObjectSettingsPackage **pObjectSettingsPackage,
    Common::ObjectRecordValue<double> *pArg1) const;

template const Common::BytesPackage *Common::GetPackageWithArgsInBytes<
    Common::ObjectRecordConfigRequestPackage>(
    Common::ObjectRecordConfigRequestPackage **pObjectRecordConfigRequestPackage) const;

template const Common::BytesPackage *Common::GetPackageWithArgsInBytes<
    Common::ObjectRecordConfigApprovalRequestPackage>(
    Common::ObjectRecordConfigApprovalRequestPackage **pObjectRecordConfigApprovalRequestPackage) const;

template const Common::BytesPackage *Common::GetPackageWithArgsInBytes<
    Common::ObjectRecordBasePackage,
    Common::ObjectRecordValue<double> *,
    Common::ObjectRecordValue<int> *,
    Common::ObjectRecordValue<float> *,
    Common::ObjectRecordValue<uint8_t> *>(
    Common::ObjectRecordBasePackage **pBasePackage,
    Common::ObjectRecordValue<double> *pArg1,
    Common::ObjectRecordValue<int> *pArg2,
    Common::ObjectRecordValue<float> *pArg3,
    Common::ObjectRecordValue<uint8_t> *pArg4) const;

template const Common::BytesPackage *Common::GetPackageWithArgsInBytes<
    Common::ObjectRecordBasePackage,
    Common::ObjectRecordValue<double> *,
    Common::ObjectRecordValue<double> *,
    Common::ObjectRecordValue<double> *>(
    Common::ObjectRecordBasePackage **pBasePackage,
    Common::ObjectRecordValue<double> *pArg1,
    Common::ObjectRecordValue<double> *pArg2,
    Common::ObjectRecordValue<double> *pArg3) const;

// End of `GetPackageWithArgsInBytes` method explicit instantiations.

template <typename T>
T *Common::GetStructFromBytes(const uint8_t *kpData) const
{
  T *pStruct = (T *)malloc(sizeof(T));
  memset(pStruct, 0, sizeof(T));
  memcpy(pStruct, kpData, sizeof(T));

  return pStruct;
}

// Explicit instantiations for `GetStructFromBytes` method.

template Common::MqttCredentials *Common::GetStructFromBytes<Common::MqttCredentials>(const uint8_t *kpData) const;

// End of `GetStructFromBytes` method explicit instantiations.