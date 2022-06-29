#include "components/installer/public/installer.h"

#ifdef TARGET_ESP32DEV
Installer *Installer::ms_pInstaller{nullptr};

Installer *Installer::GetInstance()
{
    if (ms_pInstaller == nullptr)
        ms_pInstaller = new Installer();

    return ms_pInstaller;
}

void Installer::Setup()
{
    Serial.begin(SERIAL_BAUD);

    BLEDevice::init("MyESP32Device");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);
    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic->setValue("Hello World says Neil");
    pCharacteristic->setCallbacks(new CustomBLECharacteristicCallbacks());
    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    // BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Characteristic defined! Now you can read it in your phone!");

    buzzerPin = 16;
    ledPin = 18;
    pirPin = 4;
    tempHumiSensPin = 15;
    ultrasonicSensEchoPin = 17;
    ultrasonicSensTrigPin = 23;

    pirPinState = LOW;
    pirPinStatePrev = pirPinState;

    pDht = new DHT(tempHumiSensPin, DHT11);
    pRtc = new ESP32Time();

    pDht->begin();

    const char *ssid = "HTEronet-NMHA85";
    const char *pass = "45803511";

    delay(3000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     delay(500);
    //     Serial.print(".");
    // }

    Serial.println("Connected to the WiFi network");

    delay(1000);

    pinMode(buzzerPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(pirPin, INPUT);
    pinMode(ultrasonicSensEchoPin, INPUT);
    pinMode(ultrasonicSensTrigPin, OUTPUT);

    Common::ObjectRecordBasePackage *pObjectRecordBasePackage =
        (Common::ObjectRecordBasePackage *)malloc(sizeof(Common::ObjectRecordBasePackage));

    Common::ObjectRecordValue<double> *pDistance =
        (Common::ObjectRecordValue<double> *)malloc(sizeof(Common::ObjectRecordValue<double>));

    Common::ObjectRecordValue<double> *pTemperatureCelsius =
        (Common::ObjectRecordValue<double> *)malloc(sizeof(Common::ObjectRecordValue<double>));

    Common::ObjectRecordValue<double> *pHumidity =
        (Common::ObjectRecordValue<double> *)malloc(sizeof(Common::ObjectRecordValue<double>));

    pObjectRecordBasePackage->packageType = OBJ_REC_BASE_PKG;
    pObjectRecordBasePackage->packageVersion = OBJ_REC_BASE_PKG_V;
    WiFi.macAddress(pObjectRecordBasePackage->mac);
    pObjectRecordBasePackage->rtc[0] = 0;
    pObjectRecordBasePackage->rtc[1] = 0;
    pObjectRecordBasePackage->rtc[2] = 0;
    pObjectRecordBasePackage->rtc[3] = 0;
    pObjectRecordBasePackage->rtc[4] = 0;
    pObjectRecordBasePackage->rtc[5] = 0;
    pObjectRecordBasePackage->numberOfValues = 3;
    pObjectRecordBasePackage->rssi = WiFi.RSSI();

    const Common::BytesPackage *pBytesPackage =
        Common::GetInstance()
            ->GetPackageWithArgsInBytes<Common::ObjectRecordBasePackage,
                                        Common::ObjectRecordValue<double> *,
                                        Common::ObjectRecordValue<double> *,
                                        Common::ObjectRecordValue<double> *>(
                &pObjectRecordBasePackage,
                pDistance,
                pTemperatureCelsius,
                pHumidity);

    pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, sizeof(pBytesPackage->length) - 1);
}

void Installer::Loop()
{
    delay(2000);
    // // int n = WiFi.scanNetworks();

    // // if (n == 0)
    // // {
    // //     Serial.println("No networks found!");
    // // }
    // // else
    // // {
    // //     Serial.print(n);
    // //     Serial.println(" networks found!");

    // //     for (size_t i = 0; i < n; ++i)
    // //     {
    // //         Serial.println(WiFi.SSID(i));
    // //     }
    // // }

    // double duration, cm;

    // digitalWrite(ultrasonicSensTrigPin, LOW);
    // delayMicroseconds(2);
    // digitalWrite(ultrasonicSensTrigPin, HIGH);
    // delayMicroseconds(10);
    // digitalWrite(ultrasonicSensTrigPin, LOW);

    // duration = pulseIn(ultrasonicSensEchoPin, HIGH);
    // cm = duration / 29 / 2;

    // Serial.print("cm: ");
    // Serial.println(cm);

    // delay(1000);

    // double humidity = pDht->readHumidity();
    // double temperatureCelsius = pDht->readTemperature();

    // if (isnan(humidity) || isnan(temperatureCelsius))
    // {
    //     Serial.println("Failed to read from DHT sensor!");
    // }

    // Serial.print("Humidity: ");
    // Serial.println(humidity);
    // Serial.print("Temperature: ");
    // Serial.println(temperatureCelsius);

    // delay(1000);

    // pirPinState = digitalRead(pirPin);

    // if (pirPinState == HIGH)
    // {
    //     if (pirPinStatePrev == LOW)
    //     {
    //         Serial.println("Hey I got you!!!");

    //         pirPinStatePrev = pirPinState;
    //     }

    //     analogWrite(ledPin, 180);
    // }
    // else if (pirPinState == LOW && pirPinStatePrev == HIGH)
    // {
    //     analogWrite(ledPin, LOW);

    //     Serial.println("No movement around!");

    //     pirPinStatePrev = pirPinState;
    // }
    // else
    // {
    //     analogWrite(ledPin, LOW);
    // }

    // // esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, HIGH);

    // // esp_deep_sleep_start();
    // // Serial.println("You shouldn't see this");
}
#endif