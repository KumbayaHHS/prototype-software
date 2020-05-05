# prototype-software

## Table of content

- [Description of the software](#I.-Description-of-the-software)
- [Description of the device](#II.-Description-of-the-device)
- [Development configuration](#III.-Development-configuration)
    - [1. Setup the X.509 certificates for authentication](#1.-Setup-the-X.509-certificates-for-authentication)
    - [2. Setup Azure IoT Hub](#2.-Setup-Azure-IoT-Hub)
    - [3. Add a device to Azure IoT Hub](#3.-Add-a-device-to-Azure-IoT-Hub)
    - [4. Setup the device](#4.-Setup-the-device)

## I. Description of the software

This software will be used by the device. It will meet the following conditions:
- connect to the Wi-Fi.
- collect soil information (Phosphorus, nitrogen and potassium) as well as pH. *All the information will be collected at a pre-configured interval.*
- send this information to the Azure IoT Hub.

## II. Description of the device

![Electrical diagram of the prototype](./img/prototype%20diagram.png)

Our prototype is composed of:
- an arduino UNO WiFi rev2 card.
- a 9V battery
- a pH sensor with a BNC connector
- an NPK sensor with a RS485 shield as a bridge

## III. Development configuration

### 1. Setup the X.509 certificates for authentication

- Open the sketch in the Arduino IDE using the `File -> Examples -> ArduinoECCX08 -> Tools -> ECCX08SelfSignedCert`.
- Click the "Upload" button to build and upload the sketch to your board, then open the Serial Monitor. **Make sure the line ending configuration is set to "Both NL & CR."**

**This sketch will prompt you to permanently configure your ATECC508A to ECC608A crypto element if it is not configured and locked.**

- You will be prompted for information to include in the self signed certificate, including the issue year, month, day and hour of the certificate and the expiry period in years. *We'll be using slot 0 to generate and store the private key used to sign the self signed certificate (slots 1 to 4 can be used to generate and store additional private keys if needed) - then slot 8 will be used to store the issue and expiry date of the certificate along with it's signature.*   
**Note: Since the private key is generated inside the crypto element it never leaves the device and is stored securely and cannot be read.**

- Copy the generated SHA1 value

### 2. Setup Azure IoT Hub

**If you already have an azure IoT hub, go to the step of [adding a device to the hub](#Add-a-device-to-Azure-IoT-Hub).**

- In the azure interface, click on the button "Create a resource".
- Then click "Internet of Things" and "IoT Hub".
- Fill in the form (Name of the hub, etc...) and click on "Create". **You will have to wait a few minutes for the IoT Hub to be created and deployed.**

### 3. Add a device to Azure IoT Hub

- Click the button "Go to resource".
- Now we can create a new IoT device, click "IoT Devices" under the "Explore" heading.
- Click the "Add" button to add a new device.
- Enter a name for the device then click the "X.509 Self-Signed" tab. Paste the SHA1 from the Arduino IDE's serial monitor for both the Primary and Secondary Thumbprint. Then click the "Save" button to create the device.
- You will now see a new entry on the IoT Devices page.

### 4. Setup the device

In the arduino_secrets.h file, it will be necessary to modify the definitions of the following values:
- **BROKER**: This is the hostname of your Azure IoT Hub broker. *It should look like this **"hub-name.azure-devices.net"**.*
- **DEVICE_ID**: This is the name of the device you created in the Azure IoT Hub portal.
- **SECRET_SSID**: This is the name of the Wi-Fi.
- **SECRET_PASS**: This is the Wi-Fi password.

