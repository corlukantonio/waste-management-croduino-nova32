#ifdef TARGET_ESP32DEV
#include <Arduino.h>
#else
#include <iostream>
#include <cstring>
#endif
#include <unity.h>

#include "components/utils/public/common.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void GetHexStr_ValidHexStr(void)
{
    // Arrange

    uint8_t mac[6] = {0xA2, 0x74, 0xDC, 0x5E, 0x9F, 0x18};

    // Act

    std::string macHexStr = Common::GetInstance()->GetHexStr(mac, 6);

    // Assert

    TEST_ASSERT_EQUAL_STRING("a274dc5e9f18", macHexStr.c_str());
}

void GetPackageWithArgsInBytes_ValidObjectRegistrationRequestPackage(void)
{
    // Arrange

    Common::ObjectRegistrationRequestPackage *pObjectRegistrationRequestPackage =
        (Common::ObjectRegistrationRequestPackage *)malloc(sizeof(Common::ObjectRegistrationRequestPackage));

    pObjectRegistrationRequestPackage->packageType = OBJ_REG_REQ_PKG;
    pObjectRegistrationRequestPackage->packageVersion = OBJ_REG_REQ_PKG_V;
    pObjectRegistrationRequestPackage->mac[0] = 100;
    pObjectRegistrationRequestPackage->mac[1] = 101;
    pObjectRegistrationRequestPackage->mac[2] = 102;
    pObjectRegistrationRequestPackage->mac[3] = 103;
    pObjectRegistrationRequestPackage->mac[4] = 104;
    pObjectRegistrationRequestPackage->mac[5] = 105;
    pObjectRegistrationRequestPackage->rtc[0] = 150;
    pObjectRegistrationRequestPackage->rtc[1] = 151;
    pObjectRegistrationRequestPackage->rtc[2] = 152;
    pObjectRegistrationRequestPackage->rtc[3] = 153;
    pObjectRegistrationRequestPackage->rtc[4] = 154;
    pObjectRegistrationRequestPackage->rtc[5] = 155;

    // Act

    const Common::BytesPackage *pBytesPackage =
        Common::GetInstance()
            ->GetPackageWithArgsInBytes<Common::ObjectRegistrationRequestPackage>(
                &pObjectRegistrationRequestPackage);

    pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, pBytesPackage->length - 1);

    // Assert

    TEST_ASSERT_EQUAL(15, pBytesPackage->length);
    TEST_ASSERT_EQUAL_UINT8(OBJ_REG_REQ_PKG, pBytesPackage->pBytes[0]);
    TEST_ASSERT_EQUAL_UINT8(OBJ_REG_REQ_PKG_V, pBytesPackage->pBytes[1]);
    TEST_ASSERT_EQUAL_UINT8(100, pBytesPackage->pBytes[2]);
    TEST_ASSERT_EQUAL_UINT8(101, pBytesPackage->pBytes[3]);
    TEST_ASSERT_EQUAL_UINT8(102, pBytesPackage->pBytes[4]);
    TEST_ASSERT_EQUAL_UINT8(103, pBytesPackage->pBytes[5]);
    TEST_ASSERT_EQUAL_UINT8(104, pBytesPackage->pBytes[6]);
    TEST_ASSERT_EQUAL_UINT8(105, pBytesPackage->pBytes[7]);
    TEST_ASSERT_EQUAL_UINT8(150, pBytesPackage->pBytes[8]);
    TEST_ASSERT_EQUAL_UINT8(151, pBytesPackage->pBytes[9]);
    TEST_ASSERT_EQUAL_UINT8(152, pBytesPackage->pBytes[10]);
    TEST_ASSERT_EQUAL_UINT8(153, pBytesPackage->pBytes[11]);
    TEST_ASSERT_EQUAL_UINT8(154, pBytesPackage->pBytes[12]);
    TEST_ASSERT_EQUAL_UINT8(155, pBytesPackage->pBytes[13]);
}

void GetPackageWithArgsInBytes_ValidObjectActivationRequestPackage(void)
{
    // Arrange

    Common::ObjectActivationRequestPackage *pObjectActivationRequestPackage =
        (Common::ObjectActivationRequestPackage *)malloc(sizeof(Common::ObjectActivationRequestPackage));

    pObjectActivationRequestPackage->packageType = OBJ_ACT_REQ_PKG;
    pObjectActivationRequestPackage->packageVersion = OBJ_ACT_REQ_PKG_V;
    pObjectActivationRequestPackage->mac[0] = 100;
    pObjectActivationRequestPackage->mac[1] = 101;
    pObjectActivationRequestPackage->mac[2] = 102;
    pObjectActivationRequestPackage->mac[3] = 103;
    pObjectActivationRequestPackage->mac[4] = 104;
    pObjectActivationRequestPackage->mac[5] = 105;
    pObjectActivationRequestPackage->rtc[0] = 150;
    pObjectActivationRequestPackage->rtc[1] = 151;
    pObjectActivationRequestPackage->rtc[2] = 152;
    pObjectActivationRequestPackage->rtc[3] = 153;
    pObjectActivationRequestPackage->rtc[4] = 154;
    pObjectActivationRequestPackage->rtc[5] = 155;
    pObjectActivationRequestPackage->activationCode[0] = 6;
    pObjectActivationRequestPackage->activationCode[1] = 5;
    pObjectActivationRequestPackage->activationCode[2] = 4;
    pObjectActivationRequestPackage->activationCode[3] = 3;

    // Act

    const Common::BytesPackage *pBytesPackage =
        Common::GetInstance()
            ->GetPackageWithArgsInBytes<Common::ObjectActivationRequestPackage>(
                &pObjectActivationRequestPackage);

    pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, pBytesPackage->length - 1);

    // Assert

    TEST_ASSERT_EQUAL(19, pBytesPackage->length);
    TEST_ASSERT_EQUAL_UINT8(OBJ_ACT_REQ_PKG, pBytesPackage->pBytes[0]);
    TEST_ASSERT_EQUAL_UINT8(OBJ_ACT_REQ_PKG_V, pBytesPackage->pBytes[1]);
    TEST_ASSERT_EQUAL_UINT8(100, pBytesPackage->pBytes[2]);
    TEST_ASSERT_EQUAL_UINT8(101, pBytesPackage->pBytes[3]);
    TEST_ASSERT_EQUAL_UINT8(102, pBytesPackage->pBytes[4]);
    TEST_ASSERT_EQUAL_UINT8(103, pBytesPackage->pBytes[5]);
    TEST_ASSERT_EQUAL_UINT8(104, pBytesPackage->pBytes[6]);
    TEST_ASSERT_EQUAL_UINT8(105, pBytesPackage->pBytes[7]);
    TEST_ASSERT_EQUAL_UINT8(150, pBytesPackage->pBytes[8]);
    TEST_ASSERT_EQUAL_UINT8(151, pBytesPackage->pBytes[9]);
    TEST_ASSERT_EQUAL_UINT8(152, pBytesPackage->pBytes[10]);
    TEST_ASSERT_EQUAL_UINT8(153, pBytesPackage->pBytes[11]);
    TEST_ASSERT_EQUAL_UINT8(154, pBytesPackage->pBytes[12]);
    TEST_ASSERT_EQUAL_UINT8(155, pBytesPackage->pBytes[13]);
    TEST_ASSERT_EQUAL_UINT8(6, pBytesPackage->pBytes[14]);
    TEST_ASSERT_EQUAL_UINT8(5, pBytesPackage->pBytes[15]);
    TEST_ASSERT_EQUAL_UINT8(4, pBytesPackage->pBytes[16]);
    TEST_ASSERT_EQUAL_UINT8(3, pBytesPackage->pBytes[17]);
}

void GetPackageWithArgsInBytes_ValidObjectSettingsPackage(void)
{
    // Arrange

    uint8_t *pBytes;

    double arg1Value = 0.0;

    Common::ObjectSettingsPackage *pObjectSettingsPackage =
        (Common::ObjectSettingsPackage *)malloc(sizeof(Common::ObjectSettingsPackage));

    Common::ObjectRecordValue<double> *pArg1 =
        (Common::ObjectRecordValue<double> *)malloc(sizeof(Common::ObjectRecordValue<double>));

    pObjectSettingsPackage->packageType = OBJ_STG_PKG;
    pObjectSettingsPackage->packageVersion = OBJ_STG_PKG_V;
    pObjectSettingsPackage->mac[0] = 100;
    pObjectSettingsPackage->mac[1] = 101;
    pObjectSettingsPackage->mac[2] = 102;
    pObjectSettingsPackage->mac[3] = 103;
    pObjectSettingsPackage->mac[4] = 104;
    pObjectSettingsPackage->mac[5] = 105;
    pObjectSettingsPackage->numberOfValues = 1;

    pArg1->type = 1;
    pArg1->value = 3.14159;

    // Act

    const Common::BytesPackage *pBytesPackage =
        Common::GetInstance()
            ->GetPackageWithArgsInBytes<Common::ObjectSettingsPackage,
                                        Common::ObjectRecordValue<double> *>(
                &pObjectSettingsPackage,
                pArg1);

    pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, pBytesPackage->length - 1);

    pBytes = (uint8_t *)malloc(sizeof(double));

    for (size_t i = 0, j = 10; i < sizeof(double); i++, j++)
        pBytes[i] = pBytesPackage->pBytes[j];

    memcpy(&arg1Value, pBytes, sizeof(double));
    free(pBytes);

    // Assert

    TEST_ASSERT_EQUAL(19, pBytesPackage->length);
    TEST_ASSERT_EQUAL_UINT8(OBJ_STG_PKG, pBytesPackage->pBytes[0]);
    TEST_ASSERT_EQUAL_UINT8(OBJ_STG_PKG_V, pBytesPackage->pBytes[1]);
    TEST_ASSERT_EQUAL_UINT8(100, pBytesPackage->pBytes[2]);
    TEST_ASSERT_EQUAL_UINT8(101, pBytesPackage->pBytes[3]);
    TEST_ASSERT_EQUAL_UINT8(102, pBytesPackage->pBytes[4]);
    TEST_ASSERT_EQUAL_UINT8(103, pBytesPackage->pBytes[5]);
    TEST_ASSERT_EQUAL_UINT8(104, pBytesPackage->pBytes[6]);
    TEST_ASSERT_EQUAL_UINT8(105, pBytesPackage->pBytes[7]);
    TEST_ASSERT_EQUAL_UINT8(1, pBytesPackage->pBytes[8]);
    TEST_ASSERT_EQUAL_DOUBLE(3.14159, arg1Value);
}

void GetPackageWithArgsInBytes_ValidObjectRecordConfigRequestPackage(void)
{
    // Arrange

    Common::ObjectRecordConfigRequestPackage *pObjectRecordConfigRequestPackage =
        (Common::ObjectRecordConfigRequestPackage *)malloc(sizeof(Common::ObjectRecordConfigRequestPackage));

    pObjectRecordConfigRequestPackage->packageType = OBJ_REC_CFG_REQ_PKG;
    pObjectRecordConfigRequestPackage->packageVersion = OBJ_REC_CFG_REQ_PKG_V;
    pObjectRecordConfigRequestPackage->mac[0] = 100;
    pObjectRecordConfigRequestPackage->mac[1] = 101;
    pObjectRecordConfigRequestPackage->mac[2] = 102;
    pObjectRecordConfigRequestPackage->mac[3] = 103;
    pObjectRecordConfigRequestPackage->mac[4] = 104;
    pObjectRecordConfigRequestPackage->mac[5] = 105;
    pObjectRecordConfigRequestPackage->rtc[0] = 150;
    pObjectRecordConfigRequestPackage->rtc[1] = 151;
    pObjectRecordConfigRequestPackage->rtc[2] = 152;
    pObjectRecordConfigRequestPackage->rtc[3] = 153;
    pObjectRecordConfigRequestPackage->rtc[4] = 154;
    pObjectRecordConfigRequestPackage->rtc[5] = 155;
    pObjectRecordConfigRequestPackage->recordBasePackageVersion = OBJ_REC_BASE_PKG_V;
    pObjectRecordConfigRequestPackage->numberOfValues = 4;

    // Act

    const Common::BytesPackage *pBytesPackage =
        Common::GetInstance()
            ->GetPackageWithArgsInBytes<Common::ObjectRecordConfigRequestPackage>(
                &pObjectRecordConfigRequestPackage);

    pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, pBytesPackage->length - 1);

    // Assert

    TEST_ASSERT_EQUAL(17, pBytesPackage->length);
    TEST_ASSERT_EQUAL_UINT8(OBJ_REC_CFG_REQ_PKG, pBytesPackage->pBytes[0]);
    TEST_ASSERT_EQUAL_UINT8(OBJ_REC_CFG_REQ_PKG_V, pBytesPackage->pBytes[1]);
    TEST_ASSERT_EQUAL_UINT8(100, pBytesPackage->pBytes[2]);
    TEST_ASSERT_EQUAL_UINT8(101, pBytesPackage->pBytes[3]);
    TEST_ASSERT_EQUAL_UINT8(102, pBytesPackage->pBytes[4]);
    TEST_ASSERT_EQUAL_UINT8(103, pBytesPackage->pBytes[5]);
    TEST_ASSERT_EQUAL_UINT8(104, pBytesPackage->pBytes[6]);
    TEST_ASSERT_EQUAL_UINT8(105, pBytesPackage->pBytes[7]);
    TEST_ASSERT_EQUAL_UINT8(150, pBytesPackage->pBytes[8]);
    TEST_ASSERT_EQUAL_UINT8(151, pBytesPackage->pBytes[9]);
    TEST_ASSERT_EQUAL_UINT8(152, pBytesPackage->pBytes[10]);
    TEST_ASSERT_EQUAL_UINT8(153, pBytesPackage->pBytes[11]);
    TEST_ASSERT_EQUAL_UINT8(154, pBytesPackage->pBytes[12]);
    TEST_ASSERT_EQUAL_UINT8(155, pBytesPackage->pBytes[13]);
    TEST_ASSERT_EQUAL_UINT8(OBJ_REC_BASE_PKG_V, pBytesPackage->pBytes[14]);
    TEST_ASSERT_EQUAL_UINT8(4, pBytesPackage->pBytes[15]);
}

void GetPackageWithArgsInBytes_ValidObjectRecordConfigApprovalRequestPackage(void)
{
    // Arrange

    Common::ObjectRecordConfigApprovalRequestPackage *pObjectRecordConfigApprovalRequestPackage =
        (Common::ObjectRecordConfigApprovalRequestPackage *)malloc(sizeof(Common::ObjectRecordConfigApprovalRequestPackage));

    pObjectRecordConfigApprovalRequestPackage->packageType = OBJ_REC_CFG_APV_REQ_PKG;
    pObjectRecordConfigApprovalRequestPackage->packageVersion = OBJ_REC_CFG_APV_REQ_PKG_V;
    pObjectRecordConfigApprovalRequestPackage->mac[0] = 100;
    pObjectRecordConfigApprovalRequestPackage->mac[1] = 101;
    pObjectRecordConfigApprovalRequestPackage->mac[2] = 102;
    pObjectRecordConfigApprovalRequestPackage->mac[3] = 103;
    pObjectRecordConfigApprovalRequestPackage->mac[4] = 104;
    pObjectRecordConfigApprovalRequestPackage->mac[5] = 105;
    pObjectRecordConfigApprovalRequestPackage->rtc[0] = 150;
    pObjectRecordConfigApprovalRequestPackage->rtc[1] = 151;
    pObjectRecordConfigApprovalRequestPackage->rtc[2] = 152;
    pObjectRecordConfigApprovalRequestPackage->rtc[3] = 153;
    pObjectRecordConfigApprovalRequestPackage->rtc[4] = 154;
    pObjectRecordConfigApprovalRequestPackage->rtc[5] = 155;
    pObjectRecordConfigApprovalRequestPackage->approvalCode[0] = 6;
    pObjectRecordConfigApprovalRequestPackage->approvalCode[1] = 5;
    pObjectRecordConfigApprovalRequestPackage->approvalCode[2] = 4;
    pObjectRecordConfigApprovalRequestPackage->approvalCode[3] = 3;

    // Act

    const Common::BytesPackage *pBytesPackage =
        Common::GetInstance()
            ->GetPackageWithArgsInBytes<Common::ObjectRecordConfigApprovalRequestPackage>(
                &pObjectRecordConfigApprovalRequestPackage);

    pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, pBytesPackage->length - 1);

    // Assert

    TEST_ASSERT_EQUAL(19, pBytesPackage->length);
    TEST_ASSERT_EQUAL_UINT8(OBJ_REC_CFG_APV_REQ_PKG, pBytesPackage->pBytes[0]);
    TEST_ASSERT_EQUAL_UINT8(OBJ_REC_CFG_APV_REQ_PKG_V, pBytesPackage->pBytes[1]);
    TEST_ASSERT_EQUAL_UINT8(100, pBytesPackage->pBytes[2]);
    TEST_ASSERT_EQUAL_UINT8(101, pBytesPackage->pBytes[3]);
    TEST_ASSERT_EQUAL_UINT8(102, pBytesPackage->pBytes[4]);
    TEST_ASSERT_EQUAL_UINT8(103, pBytesPackage->pBytes[5]);
    TEST_ASSERT_EQUAL_UINT8(104, pBytesPackage->pBytes[6]);
    TEST_ASSERT_EQUAL_UINT8(105, pBytesPackage->pBytes[7]);
    TEST_ASSERT_EQUAL_UINT8(150, pBytesPackage->pBytes[8]);
    TEST_ASSERT_EQUAL_UINT8(151, pBytesPackage->pBytes[9]);
    TEST_ASSERT_EQUAL_UINT8(152, pBytesPackage->pBytes[10]);
    TEST_ASSERT_EQUAL_UINT8(153, pBytesPackage->pBytes[11]);
    TEST_ASSERT_EQUAL_UINT8(154, pBytesPackage->pBytes[12]);
    TEST_ASSERT_EQUAL_UINT8(155, pBytesPackage->pBytes[13]);
    TEST_ASSERT_EQUAL_UINT8(6, pBytesPackage->pBytes[14]);
    TEST_ASSERT_EQUAL_UINT8(5, pBytesPackage->pBytes[15]);
    TEST_ASSERT_EQUAL_UINT8(4, pBytesPackage->pBytes[16]);
    TEST_ASSERT_EQUAL_UINT8(3, pBytesPackage->pBytes[17]);
}

void GetPackageWithArgsInBytes_ValidObjectRecordBasePackageWithValues(void)
{
    // Arrange

    uint8_t *pBytes;

    double arg1Value = 0.0;
    int32_t arg2Value = 0;
    float arg3Value = 0.0;
    uint8_t arg4Value = 0x00;

    Common::ObjectRecordBasePackage *pObjectRecordBasePackage =
        (Common::ObjectRecordBasePackage *)malloc(sizeof(Common::ObjectRecordBasePackage));

    Common::ObjectRecordValue<double> *pArg1 =
        (Common::ObjectRecordValue<double> *)malloc(sizeof(Common::ObjectRecordValue<double>));

    Common::ObjectRecordValue<int32_t> *pArg2 =
        (Common::ObjectRecordValue<int32_t> *)malloc(sizeof(Common::ObjectRecordValue<int32_t>));

    Common::ObjectRecordValue<float> *pArg3 =
        (Common::ObjectRecordValue<float> *)malloc(sizeof(Common::ObjectRecordValue<float>));

    Common::ObjectRecordValue<uint8_t> *pArg4 =
        (Common::ObjectRecordValue<uint8_t> *)malloc(sizeof(Common::ObjectRecordValue<uint8_t>));

    pObjectRecordBasePackage->packageType = OBJ_REC_BASE_PKG;
    pObjectRecordBasePackage->packageVersion = OBJ_REC_BASE_PKG_V;
    pObjectRecordBasePackage->mac[0] = 100;
    pObjectRecordBasePackage->mac[1] = 101;
    pObjectRecordBasePackage->mac[2] = 102;
    pObjectRecordBasePackage->mac[3] = 103;
    pObjectRecordBasePackage->mac[4] = 104;
    pObjectRecordBasePackage->mac[5] = 105;
    pObjectRecordBasePackage->rtc[0] = 150;
    pObjectRecordBasePackage->rtc[1] = 151;
    pObjectRecordBasePackage->rtc[2] = 152;
    pObjectRecordBasePackage->rtc[3] = 153;
    pObjectRecordBasePackage->rtc[4] = 154;
    pObjectRecordBasePackage->rtc[5] = 155;
    pObjectRecordBasePackage->numberOfValues = 4;
    pObjectRecordBasePackage->rssi = -63;

    pArg1->type = 1;
    pArg1->value = 3.14159;

    pArg2->type = 2;
    pArg2->value = 570239;

    pArg3->type = 3;
    pArg3->value = 39.11;

    pArg4->type = 4;
    pArg4->value = 247;

    // Act

    const Common::BytesPackage *pBytesPackage =
        Common::GetInstance()
            ->GetPackageWithArgsInBytes<Common::ObjectRecordBasePackage,
                                        Common::ObjectRecordValue<double> *,
                                        Common::ObjectRecordValue<int32_t> *,
                                        Common::ObjectRecordValue<float> *,
                                        Common::ObjectRecordValue<uint8_t> *>(
                &pObjectRecordBasePackage,
                pArg1,
                pArg2,
                pArg3,
                pArg4);

    pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, pBytesPackage->length - 1);

    pBytes = (uint8_t *)malloc(sizeof(double));

    for (size_t i = 0, j = 16; i < sizeof(double); i++, j++)
        pBytes[i] = pBytesPackage->pBytes[j];

    memcpy(&arg1Value, pBytes, sizeof(double));
    free(pBytes);

    pBytes = (uint8_t *)malloc(sizeof(int32_t));

    for (size_t i = 0, j = 16 + sizeof(*pArg1); i < sizeof(int32_t); i++, j++)
        pBytes[i] = pBytesPackage->pBytes[j];

    memcpy(&arg2Value, pBytes, sizeof(int32_t));
    free(pBytes);

    pBytes = (uint8_t *)malloc(sizeof(float));

    for (size_t i = 0, j = 16 + sizeof(*pArg1) + sizeof(*pArg2); i < sizeof(float); i++, j++)
        pBytes[i] = pBytesPackage->pBytes[j];

    memcpy(&arg3Value, pBytes, sizeof(float));
    free(pBytes);

    pBytes = (uint8_t *)malloc(sizeof(uint8_t));

    for (size_t i = 0, j = 16 + sizeof(*pArg1) + sizeof(*pArg2) + sizeof(*pArg3); i < sizeof(uint8_t); i++, j++)
        pBytes[i] = pBytesPackage->pBytes[j];

    memcpy(&arg4Value, pBytes, sizeof(uint8_t));
    free(pBytes);

    // Assert

    TEST_ASSERT_EQUAL(39, pBytesPackage->length);
    TEST_ASSERT_EQUAL_UINT8(OBJ_REC_BASE_PKG, pBytesPackage->pBytes[0]);
    TEST_ASSERT_EQUAL_UINT8(OBJ_REC_BASE_PKG_V, pBytesPackage->pBytes[1]);
    TEST_ASSERT_EQUAL_UINT8(100, pBytesPackage->pBytes[2]);
    TEST_ASSERT_EQUAL_UINT8(101, pBytesPackage->pBytes[3]);
    TEST_ASSERT_EQUAL_UINT8(102, pBytesPackage->pBytes[4]);
    TEST_ASSERT_EQUAL_UINT8(103, pBytesPackage->pBytes[5]);
    TEST_ASSERT_EQUAL_UINT8(104, pBytesPackage->pBytes[6]);
    TEST_ASSERT_EQUAL_UINT8(105, pBytesPackage->pBytes[7]);
    TEST_ASSERT_EQUAL_UINT8(150, pBytesPackage->pBytes[8]);
    TEST_ASSERT_EQUAL_UINT8(151, pBytesPackage->pBytes[9]);
    TEST_ASSERT_EQUAL_UINT8(152, pBytesPackage->pBytes[10]);
    TEST_ASSERT_EQUAL_UINT8(153, pBytesPackage->pBytes[11]);
    TEST_ASSERT_EQUAL_UINT8(154, pBytesPackage->pBytes[12]);
    TEST_ASSERT_EQUAL_UINT8(155, pBytesPackage->pBytes[13]);
    TEST_ASSERT_EQUAL_UINT8(4, pBytesPackage->pBytes[14]);
    TEST_ASSERT_EQUAL_DOUBLE(3.14159, arg1Value);
    TEST_ASSERT_EQUAL_INT32(570239, arg2Value);
    TEST_ASSERT_EQUAL_FLOAT(39.11, arg3Value);
    TEST_ASSERT_EQUAL_UINT8(247, arg4Value);
}

int RunSrcTests(void)
{
    UNITY_BEGIN();

    RUN_TEST(GetHexStr_ValidHexStr);
    RUN_TEST(GetPackageWithArgsInBytes_ValidObjectRegistrationRequestPackage);
    RUN_TEST(GetPackageWithArgsInBytes_ValidObjectActivationRequestPackage);
    RUN_TEST(GetPackageWithArgsInBytes_ValidObjectSettingsPackage);
    RUN_TEST(GetPackageWithArgsInBytes_ValidObjectRecordConfigRequestPackage);
    RUN_TEST(GetPackageWithArgsInBytes_ValidObjectRecordConfigApprovalRequestPackage);
    RUN_TEST(GetPackageWithArgsInBytes_ValidObjectRecordBasePackageWithValues);

    return UNITY_END();
}

#if TEST_MAIN == 1
#ifdef TARGET_ESP32DEV
void setup()
{
    RunSrcTests();
}

void loop()
{
}
#elif defined(TARGET_NATIVE)
int main(int argc, char **argv)
{
    RunSrcTests();

    return 0;
}
#endif
#endif