#include <RokkitHash.h>
#include <SHA256.h>
//#include <openssl/sha.h>

const int LDR_PIN = A0; // Analog pin connected to the LDR sensor
const unsigned long SENSOR_INTERVAL = 10000; // Time interval for reading the LDR sensor (1 second)
const unsigned long SHA256_INTERVAL = 60000; // Time interval for calculating SHA256 hash (60 seconds)

unsigned long lastSensorTime = 0; // Variable to keep track of the last time sensor was read
unsigned long lastSHA256Time = 0; // Variable to keep track of the last time SHA256 hash was calculated

int sensorValue; // Variable to store the sensor value
SHA256 sha256; // SHA256 object for applying SHA256 hash to sensor value

void setup() {
    Serial.begin(9600);
}

void loop() {
    unsigned long currentTime = millis(); // Stored the current time in milliseconds once the arduino board gets started running

    // Read LDR sensor data every SENSOR_INTERVAL (10 second)
    if (currentTime - lastSensorTime >= SENSOR_INTERVAL) {
        
        sensorValue = analogRead(LDR_PIN); // Read the analog value from the LDR sensor

        unsigned long startTime = micros();
        uint32_t hashValue = rokkit(reinterpret_cast<const char*>(&sensorValue), sizeof(sensorValue)); // Calculate Rokkit hash of sensor value
        unsigned long endTime = micros();

        
        // Print sensor value and Rokkit hash to Serial Monitor
        Serial.print("Sensor Value: ");
        Serial.print(sensorValue);
        Serial.print("\tRokkit Hash: ");
        Serial.print(hashValue, HEX);
        Serial.print("\t Time: "); 
        Serial.println(endTime-startTime);
        lastSensorTime = currentTime; // Update lastSensorTime
    }

    // Calculate SHA256 hash for sensor value every SHA256_INTERVAL (60 seconds)
    if (currentTime - lastSHA256Time >= SHA256_INTERVAL) {
        // Calculate SHA256 hash of sensor value
        unsigned long startTime = micros();
        uint8_t hashBuffer[32]; // SHA256 produces a 32-byte hash
        //sha256.update(reinterpret_cast<const uint8_t*>(&lastSensorTime), sizeof(lastSensorTime)); // Include current time in hash calculation for diversity
        
        sha256.update(reinterpret_cast<const uint8_t*>(&sensorValue), sizeof(sensorValue)); // Include sensor value in hash calculation
        sha256.finalize(hashBuffer, sizeof(hashBuffer));
        unsigned long endTime = micros();

        Serial.print("Sensor Value: ");
        Serial.print(sensorValue);
        // Print SHA256 hash to Serial Monitor
        Serial.print("\tSHA256 Hash for Sensor Value: ");
        for (int i = 0; i < 32; i++) {
            Serial.print(hashBuffer[i], HEX);
        }
        Serial.print("\t Time: "); 
        Serial.println(endTime-startTime);
        Serial.println();

        // Reset SHA256 object for next calculation
        sha256.reset();

        lastSHA256Time = currentTime; // Update lastSHA256Time
    }
}
