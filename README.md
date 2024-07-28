# waste-management-croduino-nova32

[![CI](https://github.com/corlukantonio/waste-management-croduino-nova32/actions/workflows/ci.yml/badge.svg)](https://github.com/corlukantonio/waste-management-croduino-nova32/actions/workflows/ci.yml)
[![Build Status](https://app.travis-ci.com/corlukantonio/waste-management-croduino-nova32.svg?token=LFYd3nvNM4EhiD43TsCb&branch=main)](https://app.travis-ci.com/corlukantonio/waste-management-croduino-nova32)
![GitHub commits since tagged version](https://img.shields.io/github/commits-since/corlukantonio/waste-management-croduino-nova32/v1.0.1/main)

## Description

**Waste Management Croduino Nova32** is a piece of software that goes to the board. It is responsible for managing board components and other external devices connected to the board. The figure below shows the interaction of the device with other components in the system.

<p align="center">
  <img src="https://user-images.githubusercontent.com/32845849/184077061-e7477185-3a82-43d2-a1b5-2192cff312c1.png" alt="waste-management-cloud-scheme-v3">
</p>

<p align="center">
  <i><b>Figure 1.</b> - Waste Management System</i>
</p>

The image below gives a closer look at what components are used and how they are wired.

<p align="center">
  <img src="https://github.com/corlukantonio/waste-management-croduino-nova32/assets/32845849/90dd7b11-67d6-452e-8128-dcc7f7c74b77" alt="waste-management-device-sketch_bb">
</p>

<p align="center">
  <i><b>Figure 2.</b> - Device sketch</i>
</p>

## Tools and technologies

Below is information about the tools and technologies used to build IoT device.

<table align="center">
  <thead>
    <tr>
      <th>Name</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Laptop</td>
      <td>
        <span>Asus GL752VW</span>
        <ul>
          <li>Operating system: Windows 10 Pro</li>
          <li>Code editor: Visual Studio Code</li>
          <ul>
            <li>PlatformIO IDE</li>
          </ul>
        </ul>
      </td>
    </tr>
    <tr>
      <td>Board</td>
      <td>
        <span>Croduino Nova32</span>
        <ul>
          <li>Microcontroller: ESP32</li>
          <li>Input voltage: 3.6V - 5.5V</li>
          <li>Battery input voltage: max. 4.2 V</li>
        </ul>
      </td>
    </tr>
    <tr>
      <td>Sensors</td>
      <td>
        <ul>
          <li>DHT11 (temperature and humidity sensor)</li>
          <li>HC-SR04 (ultrasonic distance sensor)</li>
          <li>HC-SR501 (PIR movement sensor)</li>
        </ul>
      </td>
    </tr>
  </tbody>
</table>

## Package description

### ObjectRecordBasePackage

<table align="center">
  <thead>
    <tr>
      <th>Package type</th>
      <th>Package version</th>
      <th>MAC</th>
      <th>RTC</th>
      <th>Number of values</th>
      <th>Object record value</th>
      <th>RSSI</th>
      <th>CRC</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>1 byte</td>
      <td>1 byte</td>
      <td>6 bytes</td>
      <td>6 bytes</td>
      <td>1 byte</td>
      <td>9 * Number of values bytes</td>
      <td>2 bytes</td>
      <td>1 byte</td>
    </tr>
  </tbody>
</table>

### ObjectRecordValue

<table align="center">
  <thead>
    <tr>
      <th>Type</th>
      <th>Value</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>1 byte</td>
      <td>8 bytes</td>
    </tr>
  </tbody>
</table>

## How to run it?

### Code adjusment

Before you getting down to the connection with the board and uploading the firmware onto it, there are a few tweaks to the code that need to be made. The first one is in the `WiFiHandler` where you use the `AddWiFiCredentials` method to add WiFi credentials which are then used to connect to one of the networks that can be detected by the WiFi module on the board. Below is an example of a code block where example SSIDs and passwords have been added.

```cpp
void WiFiHandler::Task()
{
  WiFi.mode(WIFI_STA);

  AddWiFiCredentials("EXAMPLE_SSID_1", "EXAMPLE_PASSWORD_1");
  AddWiFiCredentials("EXAMPLE_SSID_2", "EXAMPLE_PASSWORD_2");

  ConnectToWiFi();

  while (true)
  {
#if LOG_STACK == 1
    Serial.print("WiFiHandler: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
#endif

    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}
```

Another adjustment is in `MqttHandler` where credentials to connect to the MQTT server need to be set. Do this by completing a block of code inside the constructor as shown below.

```cpp
m_pMqttCredentials->flag = 0xFF;
strcpy(m_pMqttCredentials->server, "EXAMPLE_SERVER");
m_pMqttCredentials->port = 18850;
strcpy(m_pMqttCredentials->user, "EXAMPLE_USER");
strcpy(m_pMqttCredentials->password, "EXAMPLE_PASSWORD");
```

### Running the firmware

To establish a connection with the board via USB, it is necessary to install [CH340 driver](https://sparks.gogo.co.nz/ch340.html). Once the connection is established, you can upload the firmware to the board by opening the PlatformIO Core CLI and running the following command:

```
pio run -e esp32dev --target upload
```

Now that the firmware is uploaded and running, you can monitor what's happening on the board while it's still connected via USB by running the following command:

```
pio device monitor --baud 9600
```

Tests can be performed with:

```
pio test -e native
```
