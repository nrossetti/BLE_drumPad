#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool device_connected = false;
bool device_enabled = false;
int txValue = 0;
int txTime;
char txTemp[20];
char txString[20]; //payload limited to 20bytes to keep ble packet valid

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

const int readPin = A6; //gpio34 on doITdev Board
const int LED = 2; //blue onboard led pin

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      device_connected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      device_connected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      
      std::string enableValue = pCharacteristic->getValue();

      if (enableValue.find("1") != -1) {
        Serial.println("device enabled");
        device_enabled = true;
        digitalWrite(LED, HIGH);
      }
      else if (enableValue.find("0") != -1) {
        Serial.println("device disabled");
        device_enabled = false;
        digitalWrite(LED, LOW);
      }
    }
};

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  // Create the BLE Device
  BLEDevice::init("ESP32 UART Test");

  //BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  //BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  //BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection");
}

void loop() {
  if (device_connected&&device_enabled) {
    txValue = analogRead(readPin);
    txTime = millis();
    
    itoa(txValue, txTemp, 10);
    strcpy(txString,txTemp);
    strcat(txString,",");
    itoa(txTime, txTemp, 10);
    strcat(txString,txTemp);
    strcat(txString," ");
    
    pCharacteristic->setValue(txString);

    pCharacteristic->notify(); // Send the payload
    Serial.print("***Payload***");
    Serial.print(txString);
    Serial.println(" ***");
  }
  delay(10);
}

//if ((unsigned long)(millis() - previousMillis) >= interval)