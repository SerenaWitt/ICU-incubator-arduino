#include "ButtonHandler.ino"
#include "SensorHandler.ino"
#include "LEDManager.ino"

void setup()
{
    // Set up serial communication
    Serial.begin(9600);

    // Set up input and output pins
    configureCommon();
    attachInterrupt(digitalPinToInterrupt(commonPin), pressInterrupt, FALLING);

    // Set up other input pins
    pinMode(heartRatePin, INPUT);
    pinMode(respirationRatePin, INPUT);
    pinMode(temperaturePin, INPUT);

    // Set up output pins
    pinMode(greenLED, OUTPUT);
    pinMode(blueLED, OUTPUT);
    pinMode(yellowLED, OUTPUT);
    pinMode(redLED, OUTPUT);
    pinMode(manualVitalsLED, OUTPUT);
}

void loop()
{
    checkHeartRate();
    checkRespirationRate();
    checkTemperature();
    updateSystemState();
    handleManualVitalsLED();
}