#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

//BLE INIT
BLECharacteristic *pCharacteristic;
bool device_connected = false;
bool device_enabled = false;
int txTime;
char txTemp[20];
char txString[20]; //payload limited to 20bytes to keep ble packet valid
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

//GPIO INIT
const int pin_input = A6; //gpio34 on doITdev Board
const int pin_LED = 2; //blue onboard pin_LED pin

//DRUM INIT
const int threshold = 50;//input threshold
const int hitDelay = 20;//milliseconds
const int sampleCount = 10;
int hitCount = 0;//keep track of hits
int hitVelocity;
int peakVelocity;
int sampleSum;

int previousMillis;
int currentMillis;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      device_connected = true;
      Serial.println("device connected");
    };

    void onDisconnect(BLEServer* pServer) {
      device_connected = false;
      Serial.println("device disconnected");
        device_enabled = false;
        digitalWrite(pin_LED, LOW);
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {

      std::string enableValue = pCharacteristic->getValue();

      if (enableValue.find("1") != -1) {
        Serial.println("device enabled");
        device_enabled = true;
        digitalWrite(pin_LED, HIGH);
      }
      else if (enableValue.find("0") != -1) {
        Serial.println("device disabled");
        device_enabled = false;
        digitalWrite(pin_LED, LOW);
      }
    }
};

void setup() {
  Serial.begin(115200);

  pinMode(pin_LED, OUTPUT);

  // Create the BLE Device
  BLEDevice::init("BLE drumPad");

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
  if (device_connected/*&& device_enabled*/) {
    
    currentMillis=millis();
    
    if(currentMillis >= previousMillis+hitDelay) {
      
      sampleSum = analogRead(pin_input);
        
        if(sampleSum>=threshold){
          previousMillis=currentMillis;
          digitalWrite(pin_LED, LOW);
          for(int sampleIndex=0; sampleIndex<(sampleCount-1); sampleIndex++) {
            //sampleSum = sampleSum + analogRead(pin_input);
            int adc=analogRead(pin_input);
            if(adc>sampleSum)
              sampleSum=adc;
          }
          digitalWrite(pin_LED, HIGH);
          //sampleSum = sampleSum/sampleCount;
          
          hitCount=hitCount+1;
          
          itoa(sampleSum, txTemp, 10);
          strcpy(txString, txTemp);
          strcat(txString, " ");
          itoa(currentMillis, txTemp, 10);
          strcat(txString, txTemp);
          strcat(txString, " ");
          itoa(hitCount, txTemp, 10);
          strcat(txString, txTemp);
          pCharacteristic->setValue(txString);
          pCharacteristic->notify(); // Send the payload
          Serial.println("***BLE Payload***");
          Serial.println(txString);
          Serial.println("***"); 
        } 
    }    
  }
}

