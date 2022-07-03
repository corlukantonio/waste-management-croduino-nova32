#include "components/installer/public/installer.h"

#ifdef TARGET_ESP32DEV

Installer *Installer::ms_pInstaller{nullptr};

Installer *Installer::GetInstance()
{
    if (ms_pInstaller == nullptr)
        ms_pInstaller = new Installer();

    return ms_pInstaller;
}

void Installer::InitPins(const String kData)
{
    m_buzzerPin = 16;
    m_ledPin = 18;
    m_pirPin = 4;
    m_tempHumiSensPin = 15;
    m_ultrasonicSensEchoPin = 17;
    m_ultrasonicSensTrigPin = 23;

    pinMode(m_buzzerPin, OUTPUT);
    pinMode(m_ledPin, OUTPUT);
    pinMode(m_pirPin, INPUT);
    pinMode(m_ultrasonicSensEchoPin, INPUT);
    pinMode(m_ultrasonicSensTrigPin, OUTPUT);

    Serial.println("Init pins.");
}

void Installer::Setup()
{
    Serial.begin(SERIAL_BAUD);

    InitPins("");

    m_startTime = millis();
    m_pirPinState = LOW;
    m_pirPinStatePrev = m_pirPinState;

    m_pDht = new DHT(m_tempHumiSensPin, DHT11);
    m_pRtc = new ESP32Time();

    m_pDht->begin();

    TaskHandle_t taskBleHandler;
    TaskHandle_t taskWiFiHandler;
    TaskHandle_t taskMqttHandler;

    BleHandler *pBleHandler = new BleHandler("BleHandler", 4096, &taskBleHandler);
    WiFiHandler *pWiFiHandler = new WiFiHandler("WiFiHandler", 2048, &taskWiFiHandler);
    MqttHandler *pMqttHandler = new MqttHandler("MqttHandler", 2048, &taskMqttHandler, pWiFiHandler);

    pBleHandler->AddCallback("installer.initpins", std::bind(&Installer::InitPins, this, std::placeholders::_1));

    while (true)
    {
        delay(10000);
    }

    // Common::ObjectRecordBasePackage *pObjectRecordBasePackage =
    //     (Common::ObjectRecordBasePackage *)malloc(sizeof(Common::ObjectRecordBasePackage));

    // Common::ObjectRecordValue<double> *pDistance =
    //     (Common::ObjectRecordValue<double> *)malloc(sizeof(Common::ObjectRecordValue<double>));

    // Common::ObjectRecordValue<double> *pTemperatureCelsius =
    //     (Common::ObjectRecordValue<double> *)malloc(sizeof(Common::ObjectRecordValue<double>));

    // Common::ObjectRecordValue<double> *pHumidity =
    //     (Common::ObjectRecordValue<double> *)malloc(sizeof(Common::ObjectRecordValue<double>));

    // pObjectRecordBasePackage->packageType = OBJ_REC_BASE_PKG;
    // pObjectRecordBasePackage->packageVersion = OBJ_REC_BASE_PKG_V;
    // WiFi.macAddress(pObjectRecordBasePackage->mac);
    // pObjectRecordBasePackage->rtc[0] = 0;
    // pObjectRecordBasePackage->rtc[1] = 0;
    // pObjectRecordBasePackage->rtc[2] = 0;
    // pObjectRecordBasePackage->rtc[3] = 0;
    // pObjectRecordBasePackage->rtc[4] = 0;
    // pObjectRecordBasePackage->rtc[5] = 0;
    // pObjectRecordBasePackage->numberOfValues = 3;
    // pObjectRecordBasePackage->rssi = WiFi.RSSI();

    // const Common::BytesPackage *pBytesPackage =
    //     Common::GetInstance()
    //         ->GetPackageWithArgsInBytes<Common::ObjectRecordBasePackage,
    //                                     Common::ObjectRecordValue<double> *,
    //                                     Common::ObjectRecordValue<double> *,
    //                                     Common::ObjectRecordValue<double> *>(
    //             &pObjectRecordBasePackage,
    //             pDistance,
    //             pTemperatureCelsius,
    //             pHumidity);

    // pBytesPackage->pBytes[pBytesPackage->length - 1] = Common::GetInstance()->GetCrc(pBytesPackage->pBytes, sizeof(pBytesPackage->length) - 1);
}

void Installer::Loop()
{
    // // mqttClient.publish(topic, "hello from esp32device");

    // double duration, cm;

    // digitalWrite(m_ultrasonicSensTrigPin, LOW);
    // delayMicroseconds(2);
    // digitalWrite(m_ultrasonicSensTrigPin, HIGH);
    // delayMicroseconds(10);
    // digitalWrite(m_ultrasonicSensTrigPin, LOW);

    // duration = pulseIn(m_ultrasonicSensEchoPin, HIGH);
    // cm = duration / 29 / 2;

    // Serial.print("cm: ");
    // Serial.println(cm);

    // delay(1000);

    // double humidity = m_pDht->readHumidity();
    // double temperatureCelsius = m_pDht->readTemperature();

    // if (isnan(humidity) || isnan(temperatureCelsius))
    // {
    //     Serial.println("Failed to read from DHT sensor!");
    // }

    // Serial.print("Humidity: ");
    // Serial.println(humidity);
    // Serial.print("Temperature: ");
    // Serial.println(temperatureCelsius);

    // delay(1000);

    // m_pirPinState = digitalRead(m_pirPin);

    // if (m_pirPinState == HIGH)
    // {
    //     if (m_pirPinStatePrev == LOW)
    //     {
    //         Serial.println("Hey I got you!!!");

    //         m_pirPinStatePrev = m_pirPinState;
    //     }

    //     analogWrite(m_ledPin, 180);
    // }
    // else if (m_pirPinState == LOW && m_pirPinStatePrev == HIGH)
    // {
    //     analogWrite(m_ledPin, LOW);

    //     Serial.println("No movement around!");

    //     m_pirPinStatePrev = m_pirPinState;
    // }
    // else
    // {
    //     analogWrite(m_ledPin, LOW);
    // }

    // m_currentTime = millis();

    // if (m_pCustomBLEServerCallbacks->GetIsDeviceConnected() != 1 && m_currentTime - m_startTime >= 30000)
    // {
    //     Serial.println("Going to deep sleep mode");

    //     WiFi.disconnect();
    //     WiFi.setSleep(WIFI_PS_MAX_MODEM);

    //     esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, HIGH);
    //     esp_deep_sleep_start();
    // }
}

#endif