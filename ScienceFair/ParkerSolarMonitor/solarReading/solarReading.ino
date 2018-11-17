// Read the serial port using the following program, which will timestamp each line
// grabserial -d /dev/ttyUSB7 -b 115200 -T

// VREF for Arduino Nano measured around 4.5V
// 10-bit DAC measures voltages in 1024 steps, from 0 to VREF volts ( / 1024.0 )
// Used a voltage divider on the input to cut voltage in half ( * 2.0)

float scalarValue = 4.76 * 2.0 / 1024.0;

#define CONTROL_SOLAR_PANEL_PIN   0
#define VARIABLE_SOLAR_PANEL_PIN  1

void setup()
{
   Serial.begin(115200);
   analogReference(DEFAULT);
}

void loop()
{
   int sensorRaw; // Stores the value read from the sensor (should be 0 - 1024)
   float controlVoltage, variableVoltage;   // Stores the voltage value we need to print for user

   // Do the reading for the Control analog input

   // Read the small load volatge first
   sensorRaw = 0;
   // Average 8 readings
   for(int i = 0; i < 8; i++)
   {
     sensorRaw += analogRead(CONTROL_SOLAR_PANEL_PIN);
   }
   // Divide by 8
   sensorRaw >>= 3;

   // Convert the raw sensor value into human-readable voltage
   controlVoltage = (float) sensorRaw * scalarValue;

   // Do the reading for the Variable analog input

   // We do the same steps as before, using different sensor for high load
   sensorRaw = 0;
   // Average 8 readings
   for(int i = 0; i < 8; i++)
   {
     sensorRaw += analogRead(VARIABLE_SOLAR_PANEL_PIN);
   }
   // Divide by 8
   sensorRaw >>= 3;

   // Convert the raw sensor value into human-readable voltage
   variableVoltage = (float) sensorRaw * scalarValue;

   Serial.print(", Control Panel, ");
   Serial.print(controlVoltage, 2);
   Serial.print(", Variable Panel, ");
   Serial.println(variableVoltage, 2);
   

   // Now we will wait 60s (one minute) before repeating measurement
   for(int i = 0; i < 60; i++)
   {
      delay(1000);  // Delaying 1000ms = 1 second
   }

}
