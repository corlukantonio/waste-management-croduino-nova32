#include "components/installer/public/installer.h"

#ifdef TARGET_ESP32DEV
Installer *Installer::ms_pInstaller{nullptr};

Installer *Installer::GetInstance()
{
    if (ms_pInstaller == nullptr)
        ms_pInstaller = new Installer();

    return ms_pInstaller;
}

void Installer::AddWiFiCredentials(const char *pWiFiSsid, const char *pWiFiPassword)
{
    bool isMax = true;

    for (size_t i = 0; i < MAX_WIFI_CREDENTIALS; i++)
    {
        if (pWiFiSsids[i] == nullptr || pWiFiPasswords[i] == nullptr)
        {
            pWiFiSsids[i] = (char *)malloc(sizeof(char) * 30);
            pWiFiPasswords[i] = (char *)malloc(sizeof(char) * 30);

            memcpy(pWiFiSsids[i], pWiFiSsid, sizeof(char) * 30);
            memcpy(pWiFiPasswords[i], pWiFiPassword, sizeof(char) * 30);

            isMax = false;

            break;
        }
    }

    if (isMax)
        Serial.println(Common::GetInstance()->GetAlertMessage(Common::ALERT_MSG_MAX_WIFI_CREDENTIALS));
}

void Installer::ConnectToWiFi()
{
    unsigned long startMillis;
    unsigned long currentMillis;
    size_t index = 0;
    int16_t n = 0;
    int32_t maxRssi = 0;

    if (WiFi.status() == WL_CONNECTED)
        WiFi.disconnect();

    n = WiFi.scanNetworks();

    if (n == 0)
        Serial.println(Common::GetInstance()->GetAlertMessage(Common::ALERT_MSG_NO_NETWORKS_FOUND));

    else
    {
        Serial.println(Common::GetInstance()->GetAlertMessage(Common::ALERT_MSG_NETWORKS_FOUND, 1, n));

        for (size_t i = 0; i < n; ++i)
        {
            Serial.println(Common::GetInstance()->GetAlertMessage(Common::ALERT_MSG_WIFI_SSID_FOUND, 3, i, WiFi.SSID(i).c_str(), WiFi.RSSI(i)));

            if (maxRssi < WiFi.RSSI(i) || maxRssi == 0)
            {
                index = i;
                maxRssi = WiFi.RSSI(i);
            }
        }
    }

    for (size_t i = 0; i < MAX_WIFI_CREDENTIALS; i++)
    {
        if (strcmp(WiFi.SSID(index).c_str(), pWiFiSsids[i]) == 0)
        {
            WiFi.begin(pWiFiSsids[i], pWiFiPasswords[i]);

            Serial.println(Common::GetInstance()->GetAlertMessage(Common::ALERT_MSG_WIFI_CONNECTING, 1, pWiFiSsids[i]));

            break;
        }
    }

    startMillis = millis();

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(5000);
        Serial.print(".");

        currentMillis = millis();

        if (currentMillis - startMillis >= WAIT_WIFI_CONNECTION_MS)
            break;

        WiFi.reconnect();
    }
    Serial.println("");

    if (WiFi.status() == WL_CONNECTED)
        Serial.println(Common::GetInstance()->GetAlertMessage(Common::ALERT_MSG_WIFI_CONNECTED, 1, WiFi.SSID(index).c_str()));
}

void Installer::Setup()
{
    Serial.begin(SERIAL_BAUD);

    // Pins.

    buzzerPin = 16;
    ledPin = 18;
    pirPin = 4;
    tempHumiSensPin = 15;
    ultrasonicSensEchoPin = 17;
    ultrasonicSensTrigPin = 23;

    pinMode(buzzerPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(pirPin, INPUT);
    pinMode(ultrasonicSensEchoPin, INPUT);
    pinMode(ultrasonicSensTrigPin, OUTPUT);

    pirPinState = LOW;
    pirPinStatePrev = pirPinState;

    pDht = new DHT(tempHumiSensPin, DHT11);
    pRtc = new ESP32Time();
    pMqttClient = new MQTTClient();
    pWiFiClient = new WiFiClient();

    pDht->begin();

    // WiFi.

    pWiFiSsids = (char **)malloc(sizeof(char *) * 5);
    pWiFiPasswords = (char **)malloc(sizeof(char *) * 5);

    for (size_t i = 0; i < MAX_WIFI_CREDENTIALS; i++)
    {
        pWiFiSsids[i] = nullptr;
        pWiFiPasswords[i] = nullptr;
    }

    AddWiFiCredentials("HTEronet-NMHA85", "45803511");
    AddWiFiCredentials("MYA-L41", "03b4a12d");

    WiFi.mode(WIFI_STA);

    ConnectToWiFi();

    // MQTT.

    mqttServer = "hairdresser.cloudmqtt.com";
    mqttPort = 18972;
    mqttUser = "jlrwtkuh";
    mqttPassword = "vsHQvxEaOaIE";

    pMqttClient->begin(mqttServer, mqttPort, *pWiFiClient);

    if (!pMqttClient->connect("esp32device", mqttUser, mqttPassword))
    {
        Serial.println("MQTT connection failed!");
    }

    Serial.println(Common::GetInstance()->GetAlertMessage(Common::ALERT_MSG_MQTT_CONNECTED, 1, mqttServer));

    // BLEDevice::init("MyESP32Device");
    // BLEServer *pServer = BLEDevice::createServer();
    // BLEService *pService = pServer->createService(SERVICE_UUID);
    // BLECharacteristic *pCharacteristic = pService->createCharacteristic(
    //     CHARACTERISTIC_UUID,
    //     BLECharacteristic::PROPERTY_READ |
    //         BLECharacteristic::PROPERTY_WRITE);

    // pCharacteristic->setValue("Hello World says Neil");
    // pCharacteristic->setCallbacks(new CustomBLECharacteristicCallbacks());
    // pService->start();

    // BLEAdvertising *pAdvertising = pServer->getAdvertising();
    // // BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    // pAdvertising->addServiceUUID(SERVICE_UUID);
    // pAdvertising->setScanResponse(true);
    // pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    // pAdvertising->setMinPreferred(0x12);
    // BLEDevice::startAdvertising();
    // Serial.println("Characteristic defined! Now you can read it in your phone!");

    // delay(1000);

    // delay(1000);

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
    delay(2000);

    // mqttClient.publish(topic, "hello from esp32device");
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