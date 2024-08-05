void updateSystemState()
{
    // Nominal:
    if (heartRateNominal && respirationRateNominal && temperatureNominal)
    {
        digitalWrite(yellowLED, LOW);
        digitalWrite(redLED, LOW);
        digitalWrite(greenLED, HIGH);
    } // Alarm:
    else if (alarmState)
    {
        digitalWrite(greenLED, LOW);
        digitalWrite(yellowLED, LOW);
        digitalWrite(redLED, HIGH);
    } // Alert:
    else if (alertState)
    {
        digitalWrite(greenLED, LOW);
        digitalWrite(yellowLED, HIGH);
        digitalWrite(redLED, LOW);
    }
}

void handle ManualVitalsLED()
{
    if (heartRateNominal && respirationRateNominal && temperatureNominal)
    {
        if (millis() - manualVitalsTimer >= 10000)
        {
            digitalWrite(greenLED, LOW); // dims green LED when white light blinking
            if (!vitalsPrinted)
            {
                Serial.print(“Vital check at : “);
                Serial.println(millis());
                vitalsPrinted = true;
            }
            if (millis() - manualVitalsLEDTimer >= manualVitalsLEDInterval)
            { // interval makes it blink every 500ms
                manualVitalsLEDState = !manualVitalsLEDState;
                digitalWrite(manualVitalsLED, manualVitalsLEDState);
                manualVitalsLEDTimer = millis();
            }
        }

    }
}