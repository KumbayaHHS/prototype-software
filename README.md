# prototype-software

## Table of contents

- [Description of the software](#I-Description-of-the-software)
- [Description of the device](#II-Description-of-the-device)
- [Setup your development environment](#III-setup-your-development-environment)
    - [1. Setup Azure IoT Hub](#1-setup-azure-iot-hub)
    - [2. Add a device to Azure IoT Hub](#2-add-a-device-to-azure-iot-hub)
    - [3. Setup the device](#3-setup-the-device)

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

## III. Setup your development environment

### 1. Setup Azure IoT Hub

**If you already have an azure IoT hub, go to the step of [adding a device to the hub](#2-add-a-device-to-azure-iot-hub).**

- In the azure interface, click on the button "Create a resource".
- Then click "Internet of Things" and "IoT Hub".
- Fill in the form (Name of the hub, etc...) and click on "Create". **You will have to wait a few minutes for the IoT Hub to be created and deployed.**

### 2. Add a device to Azure IoT Hub

- Click the button "Go to resource".
- Now we can create a new IoT device, click "IoT Devices" under the "Explorers" heading.
- Click the "New" button to add a new device.
- Fill the name with the device name in the "Device ID" field, then click the "Save" button to create the device.
- You will now see a new device on the IoT Devices page.

### 3. Setup the device

For this step, you will need the following information:
- **HUB_NAME**: The name of the Azure IoT Hub
- **DEVICE_ID**: The name of the device created in the previous step
- **USER_ID**: The id of the user

You will need the Azure CLI: https://docs.microsoft.com/en-us/cli/azure/install-azure-cli?view=azure-cli-latest

In the secrets.h file, it will be necessary to modify the definitions of the following values:
- **WIFI_NAME**: This is the name of the Wi-Fi.
- **WIFI_PASSWORD**: This is the Wi-Fi password.
- **DEVICE_ID**: This is the name of the device you created in the Azure IoT Hub portal.
- **AZURE_HOSTNAME**: This is the hostname of your Azure IoT Hub. *It should look like this **"<HUB_NAME>.azure-devices.net"**.*
- **AZURE_USERNAME**: This is the username for the Azure IoT Hub. *It should look like this **"<HUB_NAME>.azure-devices.net/<DEVICE_ID>/?api-version=2018-06-30"**.*
- **AZURE_PASSWORD**: This is the password for the Azure IoT Hub. *You can retrieve it with the following command:   
``
az iot hub generate-sas-token -d <DEVICE_ID> -n <HUB_NAME> --du 31556952
``*
- **AZURE_FEEDBACK_TOPIC**: This is the address where the data are send. *It should look like this **"devices/<DEVICE_ID>/messages/events/"**.*
- **AZURE_LED_TOPIC**: This is the address where the prototype can retrieve a message sent by another device. *It should look like this **"devices/<DEVICE_ID>/messages/devicebound/#"**.*
- **USER_ID**: This is the user id.


