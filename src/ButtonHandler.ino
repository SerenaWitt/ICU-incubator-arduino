volatile bool buttonPressed = false;
unsigned long lastFire = 0;

void pressInterrupt()
{ // ISR
    if (millis() - lastFire < 200)
    { // Debounce
        return;
    }
    lastFire = millis();

    configureDistinct(); // differentiates between red and white buttons

    if (!digitalRead(buttonPins[0]))
    { // check vitals button press
        press(0);
    }

    if (!digitalRead(buttonPins[1]))
    { // check alarm button press
        press(1);
    }

    configureCommon(); // Return to original state
}

void configureCommon()
{
    pinMode(commonPin, INPUT_PULLUP); // sets interrupt pin to high

    for (int i = 0; i < sizeof(buttonPins) / sizeof(int); i++)
    { // sets both buttons to OUTPUT low
        pinMode(buttonPins[i], OUTPUT);
        digitalWrite(buttonPins[i], LOW);
    }
}

void configureDistinct()
{
    pinMode(commonPin, OUTPUT); // sets interrupt to output low
    digitalWrite(commonPin, LOW);

    for (int i = 0; i < sizeof(buttonPins) / sizeof(int); i++)
    { // sets buttons as input pullup (high)
        pinMode(buttonPins[i], INPUT_PULLUP);
    }
}

void press(int button)
{ // handles interrupt button press events
    if (button == 0)
    { // vitals state
        if (heartRateNominal && respirationRateNominal && temperatureNominal || alertState)
        {
            // "Clear Check Vitals State" button pressed
            manualVitalsTimer = millis();
            Serial.print("Vital check done at: ");
            Serial.println(millis());
            digitalWrite(manualVitalsLED, LOW);
            heartRateNominal = true;
            respirationRateNominal = true;
            alarmState = false;
            alertState = false;
            vitalsPrinted = false;
            respirationRateButtonPressed = false;
            rrAlarmPrinted = false;
            rrAlertPrinted = false;
        }
    }
    else if (button == 1 || alarmState)
    { // clear alarm state
        // "Clear Alarm State" button pressed
        Serial.println("Alarm cleared!");
        alarmState = false;
        heartRateButtonPressed = false;
        heartRateNominal = true;
        respirationRateNominal = true;
        respirationRateButtonPressed = false;
        hrAlarmPrinted = false;
        rrAlarmPrinted = false;
    }
}