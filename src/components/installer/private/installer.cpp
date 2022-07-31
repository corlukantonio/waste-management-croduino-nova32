#include "components/installer/public/installer.h"

#ifdef TARGET_ESP32DEV

RTC_DATA_ATTR int bootCount = 0; //!< Boot count.
RTC_DATA_ATTR ESP32Time rtc;     //!< RTC.

Installer *Installer::ms_pInstaller{nullptr};

Installer *Installer::GetInstance()
{
    if (ms_pInstaller == nullptr)
    {
        ms_pInstaller = new Installer();
    }

    return ms_pInstaller;
}

void Installer::PrintWakeUpReason() const
{
    esp_sleep_wakeup_cause_t wakeUpReason;

    wakeUpReason = esp_sleep_get_wakeup_cause();

    Serial.println("");

    switch (wakeUpReason)
    {
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("\nWakeup caused by external signal using RTC_IO.");
        break;

    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("\nWakeup caused by timer.");
        break;

    default:
        Serial.printf("\nWakeup was not caused by deep sleep: %d\n", wakeUpReason);
        break;
    }
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
}

void Installer::Setup()
{
    Serial.begin(SERIAL_BAUD);

    delay(1000);

    PrintWakeUpReason();

    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));

    setCpuFrequencyMhz(CPU_FREQUENCY);

    Serial.print("\nBaud rate: ");
    Serial.println(Serial.baudRate());
    Serial.print("CPU frequency: ");
    Serial.print(getCpuFrequencyMhz());
    Serial.println(" MHz");

    InitPins();

    TaskHandle_t taskPwmHandler;
    TaskHandle_t taskBleHandler;
    TaskHandle_t taskWiFiHandler;
    TaskHandle_t taskMqttHandler;
    TaskHandle_t taskWasteBin;

    PwmHandler *pPwmHandler = new PwmHandler("PwmHandler", 2048, (BaseType_t)2U, &taskPwmHandler, 0);
    BleHandler *pBleHandler = new BleHandler("BleHandler", 2048, tskIDLE_PRIORITY, &taskBleHandler, 0, pPwmHandler, m_buzzerPin);
    WiFiHandler *pWiFiHandler = new WiFiHandler("WiFiHandler", 2048, tskIDLE_PRIORITY, &taskWiFiHandler, 0);
    MqttHandler *pMqttHandler = new MqttHandler("MqttHandler", 3072, (BaseType_t)1U, &taskMqttHandler, 0, pBleHandler, pWiFiHandler);
    WasteBin *pWasteBin = new WasteBin("WasteBin", 2048, (BaseType_t)1U, &taskWasteBin, 1, pBleHandler, pWiFiHandler, pMqttHandler, &rtc, bootCount, m_buzzerPin, m_ledPin, m_pirPin, m_tempHumiSensPin, m_ultrasonicSensEchoPin, m_ultrasonicSensTrigPin);

    pBleHandler->AddCallback("installer.initpins", bind(&Installer::InitPins, this, std::placeholders::_1));

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