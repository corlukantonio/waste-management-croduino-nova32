#include <Arduino.h>
#include <unity.h>

#include "components/utils/public/common.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void GetPackageWithArgsInBytes_(void)
{
    // Arrange

    Common::ObjectRecordBasePackage *pObjectRecordBasePackage =
        (Common::ObjectRecordBasePackage *)malloc(sizeof(Common::ObjectRecordBasePackage));

    Common::ObjectRecordValue<double> *pTemperatureCelsius =
        (Common::ObjectRecordValue<double> *)malloc(sizeof(Common::ObjectRecordValue<double>));

    Common::ObjectRecordValue<int> *pHumidity =
        (Common::ObjectRecordValue<int> *)malloc(sizeof(Common::ObjectRecordValue<int>));

    Common::ObjectRecordValue<float> *pDistance =
        (Common::ObjectRecordValue<float> *)malloc(sizeof(Common::ObjectRecordValue<float>));

    Common::ObjectRecordValue<uint8_t> *pFlag =
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
    pObjectRecordBasePackage->rssi = -6;
    pObjectRecordBasePackage->crc = 107;

    pTemperatureCelsius->type = 1;
    pTemperatureCelsius->value = 27.81;
    pHumidity->type = 2;
    pHumidity->value = 170230;
    pDistance->type = 3;
    pDistance->value = 39.11;
    pFlag->type = 4;
    pFlag->value = 200;

    // Common::GetInstance()->GetPackageWithArgsInBytes<Common::ObjectRecordBasePackage, Common::ObjectRecordValue<double> *>(&pObjectRecordBasePackage, pTemperatureCelsius);

    size_t i = 0;

    const Common::BytesPackage *pBytesPackage =
        Common::GetInstance()
            ->GetPackageWithArgsInBytes<Common::ObjectRecordBasePackage,
                                        Common::ObjectRecordValue<double> *,
                                        Common::ObjectRecordValue<int> *,
                                        Common::ObjectRecordValue<float> *,
                                        Common::ObjectRecordValue<uint8_t> *>(
                &pObjectRecordBasePackage,
                pTemperatureCelsius,
                pHumidity,
                pDistance,
                pFlag);

    TEST_ASSERT_EQUAL(39, pBytesPackage->length);
}

int RunSrcTests(void)
{
    UNITY_BEGIN();

    RUN_TEST(GetPackageWithArgsInBytes_);

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