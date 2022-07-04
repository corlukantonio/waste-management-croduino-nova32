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

    TaskHandle_t taskBleHandler;
    TaskHandle_t taskWiFiHandler;
    TaskHandle_t taskMqttHandler;
    TaskHandle_t taskWasteBin;

    BleHandler *pBleHandler = new BleHandler("BleHandler", 4096, &taskBleHandler);
    WiFiHandler *pWiFiHandler = new WiFiHandler("WiFiHandler", 2048, &taskWiFiHandler);
    MqttHandler *pMqttHandler = new MqttHandler("MqttHandler", 2048, &taskMqttHandler, pWiFiHandler);
    WasteBin *pWasteBin = new WasteBin("WasteBin", 2048, &taskWasteBin, pBleHandler, pWiFiHandler, pMqttHandler, m_buzzerPin, m_ledPin, m_pirPin, m_tempHumiSensPin, m_ultrasonicSensEchoPin, m_ultrasonicSensTrigPin);

    pBleHandler->AddCallback("installer.initpins", std::bind(&Installer::InitPins, this, std::placeholders::_1));

    while (true)
    {
        delay(10000);
    }
}

void Installer::Loop()
{
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
}

#endif