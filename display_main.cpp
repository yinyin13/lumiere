#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <Stepper.h>

// Define the UUIDs for the BLE service and characteristic used by the server device
static BLEUUID serviceUUID("64f4b96c-6431-4cdf-9742-972355591ef4");
static BLEUUID charUUID("2374e41a-dd58-4f9b-aaa5-58481af3228f");

// Flag variables for BLE connection
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;

// Remote characteristic to receive pred_label from the server device
static BLERemoteCharacteristic* pRemoteCharacteristic = nullptr;
static BLEAdvertisedDevice* myDevice = nullptr;

// Stepper motor configuration
const int stepsPerRevolution = 605;  // Change this to fit the number of steps per revolution for your motor
Stepper myStepper(stepsPerRevolution, 1, 2, 3, 4); // Initialize the stepper motor

// Variable to store the last received data
String lastReceivedData = "";

// Callback function for receiving notifications from the server device
static void notifyCallback(
    BLERemoteCharacteristic* pBLERemoteCharacteristic,
    uint8_t* pData,
    size_t length,
    bool isNotify
) {
    // Trim non-printable characters from the received data
    String receivedData = "";
    for (size_t i = 0; i < length; i++) {
        if (isprint(pData[i])) {
            receivedData += (char)pData[i];
        }
    }

    // Print the trimmed prediction label
    Serial.print("Received Prediction Label: ");
    Serial.println(receivedData);

    // Extract the prediction label from receivedData
    int colonIndex = receivedData.indexOf(':');
    if (colonIndex != -1) {
        String predictionLabel = receivedData.substring(0, colonIndex);

        if (predictionLabel.equals("calm")) {
            // Perform stepper motor action for "calm" prediction
            myStepper.step(stepsPerRevolution); // Rotate one revolution clockwise
        } else if (predictionLabel.equals("happy")) {
            // Perform stepper motor action for "happy" prediction
            myStepper.step(-stepsPerRevolution); // Rotate one revolution counterclockwise
        }
    }
}

// Callback function for BLE client events
class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
        connected = true;
        Serial.println("Connected to the server");
    }

    void onDisconnect(BLEClient* pclient) {
        connected = false;
        Serial.println("Disconnected from the server");
    }
};

// Function to connect to the BLE server
bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    BLEClient*  pClient = BLEDevice::createClient();
    Serial.println(" - Created client");
    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remote BLE server
    if (pClient->connect(myDevice)) {
        Serial.println(" - Connected to server");

        // Obtain a reference to the service on the remote BLE server
        BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
        if (pRemoteService == nullptr) {
            Serial.print("Failed to find service with UUID: ");
            Serial.println(serviceUUID.toString().c_str());
            pClient->disconnect();
            return false;
        }

        // Obtain a reference to the characteristic in the service
        pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
        if (pRemoteCharacteristic == nullptr) {
            Serial.print("Failed to find characteristic with UUID: ");
            Serial.println(charUUID.toString().c_str());
            pClient->disconnect();
            return false;
        }

        // Register for notifications from the remote characteristic
        if (pRemoteCharacteristic->canNotify()) {
            pRemoteCharacteristic->registerForNotify(notifyCallback);
        }

        return true;
    } else {
        Serial.println("Failed to connect to server. Retrying...");
        delay(1000); // Delay before retrying connection
        return false;
    }
}

// Callback function for BLE scan results
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
            BLEDevice::getScan()->stop();
            myDevice = new BLEAdvertisedDevice(advertisedDevice);
            doConnect = true;
            doScan = true;
        } // Found our server
    } // onResult
}; // MyAdvertisedDeviceCallbacks

void setup() {
    Serial.begin(115200);
    myStepper.setSpeed(60); // Set the motor speed in RPM
    Serial.println("Starting Arduino BLE Client application...");
    
    // Initialize BLE
    BLEDevice::init("");

    // Start BLE scan
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
}

void loop() {
    // If doConnect is true and not already connected, attempt to connect to the server
    if (doConnect && !connected) {
        connectToServer();
    }

    // Your other code can go here

    // Handle other tasks or delays here as needed
}
