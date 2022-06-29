#include <Arduino.h>
#include <unity.h>

#include "components/utils/public/common.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void GetPackageWithArgsInBytes_ValidPackage(void)
{
    // Arrange

    uint8_t *pBytes;

    double arg1Value = 0.0;
    int32_t arg2Value = 0;
    float arg3Value = 0.0;
    uint8_t arg4Value = 0x00;
    uint8_t crc = 0;

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

    pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, sizeof(pBytesPackage->length) - 1);

    pBytes = (uint8_t *)malloc(sizeof(double));

    for (size_t i = 0, j = 16; i < sizeof(*pArg1); i++, j++)
        pBytes[i] = pBytesPackage->pBytes[j];

    memcpy(&arg1Value, pBytes, sizeof(double));

    pBytes = (uint8_t *)malloc(sizeof(int32_t));

    for (size_t i = 0, j = 16 + sizeof(*pArg1); i < sizeof(*pArg2); i++, j++)
        pBytes[i] = pBytesPackage->pBytes[j];

    memcpy(&arg2Value, pBytes, sizeof(int32_t));

    pBytes = (uint8_t *)malloc(sizeof(float));

    for (size_t i = 0, j = 16 + sizeof(*pArg1) + sizeof(*pArg2); i < sizeof(*pArg3); i++, j++)
        pBytes[i] = pBytesPackage->pBytes[j];

    memcpy(&arg3Value, pBytes, sizeof(float));

    pBytes = (uint8_t *)malloc(sizeof(uint8_t));

    for (size_t i = 0, j = 16 + sizeof(*pArg1) + sizeof(*pArg2) + sizeof(*pArg3); i < sizeof(*pArg4); i++, j++)
        pBytes[i] = pBytesPackage->pBytes[j];

    memcpy(&arg4Value, pBytes, sizeof(uint8_t));

    // Assert

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

    TEST_ASSERT_EQUAL(39, pBytesPackage->length);
    TEST_ASSERT_EQUAL_DOUBLE(3.14159, arg1Value);
    TEST_ASSERT_EQUAL_INT32(570239, arg2Value);
    TEST_ASSERT_EQUAL_FLOAT(39.11, arg3Value);
    TEST_ASSERT_EQUAL_UINT8(247, arg4Value);
}

int RunSrcTests(void)
{
    UNITY_BEGIN();

    RUN_TEST(GetPackageWithArgsInBytes_ValidPackage);

    return UNITY_END();
}

int main(int argc, char **argv)
{
    RunSrcTests();
}

#if TEST_MAIN == 1
void setup()
{
    delay(1000);

    RunSrcTests();
}

void loop()
{
}
#endif