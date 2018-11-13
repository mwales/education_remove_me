

// VREF for Arduino Nano measured at 4.5V
// 10-bit DAC measures voltages in 1024 steps, from 0 to VREF volts

float scalarValue = 4.53 / 1024.0; // * 100.0;

void setup()
{
   Serial.begin(9600);
   analogReference(DEFAULT);
}

void loop()
{
   int sensorRaw; // Stores the value read from the sensor (should be 0 - 1024)
   float voltage;   // Stores the voltage value we need to print for user

   // Analog pin 0 = Small load
   // Analog pin 1 = High load

   Serial.println("\nPreparing measurement");

   // Read the small load volatge first
   sensorRaw = analogRead(0);

   // Convert the raw sensor value into human-readable voltage
   voltage = (float) sensorRaw * scalarValue;


   Serial.print("Small Load = ");
   Serial.print(voltage, 2);
   Serial.println(" V");

   //delay(1000);

   // We do the same steps as before, using different sensor for high load
   sensorRaw = analogRead(1);

   // Convert the raw sensor value into human-readable voltage
   voltage = (float) sensorRaw * scalarValue;

   Serial.print("Large Load = ");
   Serial.print(voltage, 2);
   Serial.println(" V");
   

   // Now we will wait 60s (one minute) before repeating measurement
   for(int i = 0; i < 10; i++)
   {
      delay(1000);  // Delaying 1000ms = 1 second
   }

}
