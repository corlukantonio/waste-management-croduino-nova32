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

    for (size_t i = 0; i < WIFI_MAX_CREDENTIALS; i++)
    {
        if (m_pWiFiSsids[i] == nullptr || m_pWiFiPasswords[i] == nullptr)
        {
            m_pWiFiSsids[i] = (char *)malloc(sizeof(char) * 30);
            m_pWiFiPasswords[i] = (char *)malloc(sizeof(char) * 30);

            memcpy(m_pWiFiSsids[i], pWiFiSsid, sizeof(char) * 30);
            memcpy(m_pWiFiPasswords[i], pWiFiPassword, sizeof(char) * 30);

            isMax = false;

            break;
        }
    }

    if (isMax)
    {
        Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgWiFiMaxCredentialsReached));
    }
}

void Installer::ConnectToWiFi()
{
    unsigned long startMillis;
    unsigned long currentMillis;
    size_t index = 0;
    int16_t n = 0;
    int32_t maxRssi = 0;

    if (WiFi.status() == WL_CONNECTED)
    {
        WiFi.disconnect();
    }

    n = WiFi.scanNetworks();

    if (n == 0)
    {
        Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgWiFiNoNetworksFound));
    }
    else
    {
        Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgWiFiNetworksFound, 1, n));

        for (size_t i = 0; i < n; ++i)
        {
            Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgWiFiSsidFound, 3, i, WiFi.SSID(i).c_str(), WiFi.RSSI(i)));

            if (maxRssi < WiFi.RSSI(i) || maxRssi == 0)
            {
                index = i;
                maxRssi = WiFi.RSSI(i);
            }
        }
    }

    for (size_t i = 0; i < WIFI_MAX_CREDENTIALS; i++)
    {
        if (strcmp(WiFi.SSID(index).c_str(), m_pWiFiSsids[i]) == 0)
        {
            WiFi.begin(m_pWiFiSsids[i], m_pWiFiPasswords[i]);

            Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgWiFiConnecting, 1, m_pWiFiSsids[i]));

            break;
        }
    }

    startMillis = millis();

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(5000);
        Serial.print(".");

        currentMillis = millis();

        if (currentMillis - startMillis >= WIFI_WAIT_CONNECTION_MS)
        {
            break;
        }

        WiFi.reconnect();
    }

    Serial.println("");

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgWiFiConnected, 1, WiFi.SSID(index).c_str()));
    }
}

void Installer::Setup()
{
    Serial.begin(SERIAL_BAUD);

    m_startTime = millis();

    // Pins.

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

    m_pirPinState = LOW;
    m_pirPinStatePrev = m_pirPinState;

    m_pDht = new DHT(m_tempHumiSensPin, DHT11);
    m_pRtc = new ESP32Time();
    m_pMqttClient = new MQTTClient();
    m_pWiFiClient = new WiFiClient();

    m_pDht->begin();

    // WiFi.

    m_pWiFiSsids = (char **)malloc(sizeof(char *) * 5);
    m_pWiFiPasswords = (char **)malloc(sizeof(char *) * 5);

    for (size_t i = 0; i < WIFI_MAX_CREDENTIALS; i++)
    {
        m_pWiFiSsids[i] = nullptr;
        m_pWiFiPasswords[i] = nullptr;
    }

    AddWiFiCredentials("HTEronet-NMHA85", "45803511");
    AddWiFiCredentials("MYA-L41", "03b4a12d");

    WiFi.mode(WIFI_STA);

    ConnectToWiFi();

    // MQTT.

    m_mqttServer = "hairdresser.cloudmqtt.com";
    m_mqttPort = 18972;
    m_mqttUser = "jlrwtkuh";
    m_mqttPassword = "vsHQvxEaOaIE";

    m_pMqttClient->begin(m_mqttServer, m_mqttPort, *m_pWiFiClient);

    if (!m_pMqttClient->connect("esp32wastebin", m_mqttUser, m_mqttPassword))
    {
        Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMqttConnectionFailed));
    }

    if (m_pMqttClient->connected())
    {
        Serial.println(Common::GetInstance()->GetAlertMessage(Common::eAlertMsgMqttConnected, 1, m_mqttServer));
    }

    // BLE.

    m_pCustomBLECharacteristicCallbacks = new CustomBLECharacteristicCallbacks();
    m_pCustomBLEServerCallbacks = new CustomBLEServerCallbacks();

    BLEDevice::init(DEV_NAME);

    m_pBleServer = BLEDevice::createServer();
    m_pBleServer->setCallbacks(m_pCustomBLEServerCallbacks);

    m_pBleService = m_pBleServer->createService(SERVICE_UUID);

    m_pBleCharacteristic = m_pBleService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

    m_pBleCharacteristic->setValue("Hello from ESP32 device.");
    m_pBleCharacteristic->setCallbacks(m_pCustomBLECharacteristicCallbacks);
    m_pBleService->start();

    m_pBleAdvertising = m_pBleServer->getAdvertising();
    m_pBleAdvertising->addServiceUUID(SERVICE_UUID);
    m_pBleAdvertising->setScanResponse(true);
    m_pBleAdvertising->setMinPreferred(0x06);
    m_pBleAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    Serial.println("Characteristic defined! Now you can read it in your phone!");

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

    double duration, cm;

    digitalWrite(m_ultrasonicSensTrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(m_ultrasonicSensTrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(m_ultrasonicSensTrigPin, LOW);

    duration = pulseIn(m_ultrasonicSensEchoPin, HIGH);
    cm = duration / 29 / 2;

    Serial.print("cm: ");
    Serial.println(cm);

    delay(1000);

    double humidity = m_pDht->readHumidity();
    double temperatureCelsius = m_pDht->readTemperature();

    if (isnan(humidity) || isnan(temperatureCelsius))
    {
        Serial.println("Failed to read from DHT sensor!");
    }

    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Temperature: ");
    Serial.println(temperatureCelsius);

    delay(1000);

    m_pirPinState = digitalRead(m_pirPin);

    if (m_pirPinState == HIGH)
    {
        if (m_pirPinStatePrev == LOW)
        {
            Serial.println("Hey I got you!!!");

            m_pirPinStatePrev = m_pirPinState;
        }

        analogWrite(m_ledPin, 180);
    }
    else if (m_pirPinState == LOW && m_pirPinStatePrev == HIGH)
    {
        analogWrite(m_ledPin, LOW);

        Serial.println("No movement around!");

        m_pirPinStatePrev = m_pirPinState;
    }
    else
    {
        analogWrite(m_ledPin, LOW);
    }

    m_currentTime = millis();

    if (m_pCustomBLEServerCallbacks->GetIsDeviceConnected() != 1 && m_currentTime - m_startTime >= 30000)
    {
        Serial.println("Going to deep sleep mode");

        WiFi.disconnect();
        WiFi.setSleep(WIFI_PS_MAX_MODEM);

        esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, HIGH);
        esp_deep_sleep_start();
    }
}
#endif