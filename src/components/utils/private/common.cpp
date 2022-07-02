#include "components/utils/public/common.h"

Common *Common::ms_pCommon{nullptr};

/********************************
 *                              *
 * Methods.                     *
 * ---------------------------- *
 *                              *
 *                              *
 ********************************/

Common *Common::GetInstance()
{
  if (ms_pCommon == nullptr)
    ms_pCommon = new Common();

  return ms_pCommon;
}

const char *Common::GetAlertMessage(AlertMessageEnum alertMessage, size_t n, ...) const
{
  String s;

  va_list args;
  va_start(args, n);

  char *message = (char *)malloc(sizeof(char) * 50);

#ifdef TARGET_ESP32DEV
  switch (alertMessage)
  {
  case eAlertMsgWiFiConnecting:
    s = String("Connecting with \"");

    for (size_t i = 0; i < n; i++)
      s.concat(va_arg(args, char *));

    s.concat("\" WiFi network.");

    break;

  case eAlertMsgWiFiConnected:
    s = String("Connected to the \"");

    for (size_t i = 0; i < n; i++)
      s.concat(va_arg(args, char *));

    s.concat("\" WiFi network.");

    break;

  case eAlertMsgWiFiSsidFound:
    s = String("");

    for (size_t i = 0; i < n; i++)
    {
      if (i == 0)
      {
        s.concat(va_arg(args, size_t) + 1);
        s.concat(": ");
      }
      else if (i == 1)
      {
        s.concat(va_arg(args, char *));
        s.concat(" (");
      }
      else if (i == 2)
      {
        s.concat(va_arg(args, int32_t));
        s.concat(")");
      }
    }

    break;

  case eAlertMsgWiFiNetworksFound:
    s = String("");

    for (size_t i = 0; i < n; i++)
      s.concat(va_arg(args, size_t));

    if (n == 1)
      s.concat(" network found.");
    else
      s.concat(" networks found.");

    break;

  case eAlertMsgWiFiNoNetworksFound:
    s = String("No networks found.");

    break;

  case eAlertMsgWiFiMaxCredentialsReached:
    s = String("The maximum number of WiFi credentials has been reached.");

    break;

  case eAlertMsgMqttConnected:
    s = String("Connected to the ");
    for (size_t i = 0; i < n; i++)
      s.concat(va_arg(args, char *));
    s.concat(" MQTT broker.");

    break;

  case eAlertMsgMqttConnectionFailed:
    s = String("MQTT connection failed!");

    break;

  default:
    s = String("Undefined!");

    break;
  }
#endif

  va_end(args);

  strcpy(message, s.c_str());

  return message;
}

const uint8_t Common::GetCrc(const uint8_t *kpData, const size_t kSize) const
{
  uint8_t crc = 0x00;

  for (size_t i = 0; i < kSize; i++)
    crc += (i * kpData[i]);

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
    pBytes[rCounter] = pArgInBytes[i];

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

  if (std::is_same<TPackage, ObjectRegistrationRequestPackage>::value ||
      std::is_same<TPackage, ObjectActivationRequestPackage>::value ||
      std::is_same<TPackage, ObjectRecordConfigRequestPackage>::value ||
      std::is_same<TPackage, ObjectRecordConfigApprovalRequestPackage>::value)
  {
    packageLength = sizeof(TPackage);
    pPackage = (uint8_t *)malloc(packageLength);
    memcpy(pPackage, pBasePackageInBytes, packageLength);
  }
  else if (std::is_same<TPackage, ObjectRecordBasePackage>::value)
  {
    GetArgsLength(argsLength, args...);

    packageLength = sizeof(TPackage) + argsLength;
    pPackage = (uint8_t *)malloc(packageLength);

    for (size_t i = 0; i < packageLength; i++)
    {
      if (i == 15)
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
    Common::ObjectRegistrationRequestPackage>(
    Common::ObjectRegistrationRequestPackage **pObjectRegistrationRequestPackage) const;

template const Common::BytesPackage *Common::GetPackageWithArgsInBytes<
    Common::ObjectActivationRequestPackage>(
    Common::ObjectActivationRequestPackage **pObjectActivationRequestPackage) const;

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