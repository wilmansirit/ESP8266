#include <Arduino.h>

int TEMPERATURE_PIN = 0;
float previousTemp = 0;

void setup()
{
    Serial.begin(9600);
}
void loop()
{
    float mv = (analogRead(TEMPERATURE_PIN) / 1024.0) * 5000;
    float currentTemp = mv / 10;

    if ((currentTemp <= previousTemp - 0.25) || (currentTemp >= previousTemp + 0.25))
    {

        // Serial.print("TEMP=");
        Serial.println(currentTemp);
        // Serial.println("*C");
    }

    previousTemp = currentTemp;
    delay(1000);
    
}