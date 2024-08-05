void checkHeartRate()
{
    if (digitalRead(heartRatePin) == HIGH)
    { // HR button is pressed
        if (!heartRateButtonPressed)
        { // If button not already pressed, record the press time
            heartRateButtonPressed = true;
            heartRatePressTime = millis();
            Serial.print("HR button pressed at: ");
            Serial.println(millis());
            hrAlarmPrinted = false;
        }
    }
    // Check if HR button was pressed more than 3 seconds ago
    if (heartRateButtonPressed && (millis() - heartRatePressTime >= 3000))
    {
        if (!hrAlarmPrinted)
        { // Check if alarm state for HR hasn't been printed yet
            Serial.print("HR LED turned on at: ");
            Serial.println(millis());
            Serial.println("Alarm State");
            hrAlarmPrinted = true; // Set flag to indicate that alarm state for HR has been printed
        }
        alarmState = true;        // alarm state true after 3 seconds
        heartRateNominal = false; // HR abnormal
    }
}

void checkRespirationRate()
{
    if (!alarmState)
    {
        if (digitalRead(respirationRatePin) == HIGH)
        { // RR button is pressed
            if (!respirationRateButtonPressed)
            { // If button not already pressed, record the press time
                respirationRateButtonPressed = true;
                respirationRatePressTime = millis();
                Serial.print("RR button pressed at: ");
                Serial.println(millis());
            }
        }
        // Check if RR button was pressed more than 3 seconds ago
        if (respirationRateButtonPressed && (millis() - respirationRatePressTime >= 3000))
        {
            if (!rrAlarmPrinted)
            {
                Serial.print("Alert state turned on at: ");
                Serial.println(millis());
                Serial.println("Alert State");
                rrAlarmPrinted = true; // enters alert state
            }
            alertState = true;
            respirationRateNominal = false;
        }
        // Check if RR is in alert state for 6 seconds
        if (alertState && (millis() - respirationRatePressTime >= 9000))
        {
            if (!rrAlertPrinted)
            {
                Serial.print("RR alarm state at: ");
                Serial.println(millis());
                Serial.println("Alarm State");
                rrAlertPrinted = true; // enters alarm state
            }
            alarmState = true;
            alertState = false;
        }
    }
}

void checkTemperature()
{
    if (!alarmState && !alertState)
    { // don't allow temp check if alert or alarm state is true
        if (digitalRead(temperatureHighPin) == HIGH)
        {
            if (!temperatureHighPressed)
            { // Check if it's the first press
                temperatureHighPressed = true;
                temperatureNominal = false;
                temperatureHigh = true;
                temperatureLow = false;
            }
        }
        else if (digitalRead(temperatureLowPin) == HIGH)
        {
            if (!temperatureLowPressed)
            { // Check if it's the first press
                temperatureLowPressed = true;
                temperatureNominal = false;
                temperatureHigh = false;
                temperatureLow = true;
            }
        }
        else
        {
            // If neither button is currently pressed, disable the pressed flags
            temperatureHighPressed = false;
            temperatureLowPressed = false;
        }

        // Condition to prevent printing during the adjustment period
        if (!temperatureAdjusted)
        {
            if (temperatureHigh)
            {
                Serial.println("Temperature is too high!");
            }
            else if (temperatureLow)
            {
                Serial.println("Temperature is too low!");
            }
        }
    }

    // Check for temperature adjustment
    if ((temperatureHigh || temperatureLow) && digitalRead(temperaturePin) == HIGH && !temperatureAdjusted)
    {
        digitalWrite(blueLED, HIGH);
        temperatureAdjustmentTimer = millis(); // Start adjustment timer
        temperatureAdjusted = true;            // Mark temperature as adjusted
        Serial.print("Temperature adjustment started at: ");
        Serial.println(millis());
    }
    if (temperatureAdjusted && (millis() - temperatureAdjustmentTimer >= 5000))
    {
        temperatureNominal = true; // Reset temperature to normal after 5 seconds
        digitalWrite(blueLED, LOW);
        temperatureAdjusted = false; // Reset temperature adjustment flag
        temperatureHigh = false;     // Reset high temperature flag
        temperatureLow = false;      // Reset low temperature flag
        Serial.print("Temperature adjustment completed at: ");
        Serial.println(millis());
    }
}