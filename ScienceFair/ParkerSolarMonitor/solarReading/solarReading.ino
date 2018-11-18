#include <EEPROM.h>

// Read the serial port using the following program, which will timestamp each line
// grabserial -d /dev/ttyUSB7 -b 115200 -T

// VREF for Arduino Nano measured around 4.5V
// 10-bit DAC measures voltages in 1024 steps, from 0 to VREF volts ( / 1024.0 )
// Used a voltage divider on the input to cut voltage in half ( * 2.0)

float scalarValue = 4.76 * 2.0 / 1024.0;
char  boardName[10];

#define CONTROL_SOLAR_PANEL_PIN   0
#define VARIABLE_SOLAR_PANEL_PIN  1

// EEPROM Locations
#define NAME_ADDR 0
#define VREF_ADDR 0x20

void setup()
{
   Serial.begin(115200);
   analogReference(DEFAULT);

   strcpy(boardName, "unnamed");

   readNameEeprom();
   readVrefEeprom();
}

int eepromChecksum(int addr,int len)
{
  int val = 0;
  for (int i = 0; i < len; i++)
  {
    val ^= EEPROM.read(addr + i);
  }

  Serial.print("CRC @ ");
  Serial.print(addr);
  Serial.print(" = ");
  Serial.println(val);
  return val;
}

int bufferChecksum(char* buffer,int len)
{
  int val = 0;
  for (int i = 0; i < len; i++)
  {
    val ^= buffer[i];
  }

  Serial.print("CRC BUF = ");
  Serial.println(val);
  return val;
}

/**
 * Stores board name in EEPROM
 */
void storeName(char* newName)
{
  int nameLen = strlen(newName);
  if (nameLen > 10)
  {
    Serial.println("Invalid name length, too long");
    return;
  }

  int checksum = bufferChecksum(newName, nameLen);

  EEPROM.write(NAME_ADDR, nameLen);
  EEPROM.write(NAME_ADDR + 1, checksum);
  for(int i = 0; i < nameLen; i++)
  {
    EEPROM.write(NAME_ADDR + 2 + i, newName[i]);
  }

  Serial.println("Name stored in EEPROM");

  strcpy(boardName, newName);
}

/**
 * Stores board name in EEPROM.  Board name helpful incase we forget which port is connected to what devices
 */
bool readNameEeprom()
{
  int numBytes = EEPROM.read(NAME_ADDR);

  if (numBytes > 10)
  {
    Serial.println("Name length in EEPROM is invalid");
    return false;
  }

  int checksum = EEPROM.read(NAME_ADDR+1);

  if (checksum != eepromChecksum(NAME_ADDR+2, numBytes))
  {
    Serial.println("Checksum failure for name");
    return false;
  }

  for(int i = 0; i < numBytes; i++)
  {
    boardName[i] = EEPROM.read(NAME_ADDR + 2 + i);
  }
  
  boardName[numBytes] = 0; // null terminator

  Serial.print("Name from EEPROM: ");
  Serial.println(boardName);
  
  return true;  
}

/**
 * Stores VREF value in EEPROM (so we can change it without recompiling and reflashing)
 */
void storeVrefEeprom(char* newVref)
{
  float vref = atof(newVref);
  
  int vrefTimes100 = vref * 100.0;

  char vrefString[15];
  itoa(vrefTimes100, vrefString, 10);

  int vrefLen = strlen(vrefString);
  if (vrefLen > 10)
  {
    Serial.println("Vref length, too long");
    return;
  }
  
  int checksum = bufferChecksum(vrefString, vrefLen);

  EEPROM.write(VREF_ADDR, vrefLen);
  EEPROM.write(VREF_ADDR + 1, checksum);
  for(int i = 0; i < vrefLen; i++)
  {
    EEPROM.write(VREF_ADDR + 2 + i, vrefString[i]);
  }

  Serial.print("VREF stored in EEPROM = ");
  Serial.println(vrefString);

  readVrefEeprom();
}

/**
 * Reads VREF from EEPROM, recomputes scalar used for voltage measurements
 */
bool readVrefEeprom()
{
  int numBytes = EEPROM.read(VREF_ADDR);

  if (numBytes > 10)
  {
    Serial.println("VREF length in EEPROM is invalid");
    return false;
  }

  int checksum = EEPROM.read(VREF_ADDR+1);

  if (checksum != eepromChecksum(VREF_ADDR+2, numBytes))
  {
    Serial.println("Checksum failure for VREF");
    return false;
  }

  char vrefTime100[11];
  for(int i = 0; i < numBytes; i++)
  {
    vrefTime100[i] = EEPROM.read(VREF_ADDR + 2 + i);
  }
  
  vrefTime100[numBytes] = 0; // null terminator

  Serial.print("VREF from EEPROM: ");
  Serial.print(boardName);

  float vref = (float) atoi(vrefTime100) / 100.0;

  Serial.print(" = ");
  Serial.print(vref, 3);

   scalarValue = vref * 2.0 / 1024.0;

   Serial.print(", scaler = ");
   Serial.println(scalarValue, 3);
  
  return true;  
}

char inputBuffer[15];
int bufferPos;

bool readIntoBufferWithTimeout()
{
  if (bufferPos == 14)
  {
    Serial.println("Buffer overrun, clearing buffer");
    bufferPos = 0;
  }

  int numBytes = Serial.readBytes(inputBuffer + bufferPos, 1);
  if (numBytes == 0)
    return false;
  
  if ( (inputBuffer[bufferPos] == '\n') || (inputBuffer[bufferPos] == '\r') )
  {
    // Buffer complete!
    inputBuffer[bufferPos] = 0; // null terminate
    bufferPos = 0;
    return true;
  }
  else
  {
    bufferPos++;
    return false;
  }
  
}

/**
 * Returns true if a user has entered a command.  Command string is in global variable
 */
bool handleInputAndDelay(int numSeconds)
{
    for(int i = 0; i < 60; i++)
    {
      if (readIntoBufferWithTimeout())
      {
        // User gave us data
        //Serial.print("User data: ");
        //Serial.println(inputBuffer);
        return true;
      }
      else
      {
        // Do nothing
      }
    }

    return false;
}

/**
 * Prints help, handles commands from user to update EEPROM values
 */
void processInput()
{
  if (strcmp(inputBuffer, "") == 0)
  {
    // Empty string
    return;
  }
  
  if ( strcmp(inputBuffer, "?") == 0)
  {
    Serial.println("Help (Solar Panel Monitor:");
    Serial.println("  Var=Value");
    Serial.println("name=String  ex: name=Alkaline");
    Serial.println("vref=Float   ex: vref=4.5");
    
    Serial.println("\nStatus:\n");
    Serial.print("Scalar = ");
    Serial.println(scalarValue, 3);
    Serial.print("Board Name = ");
    Serial.println(boardName);

    readNameEeprom();
    readVrefEeprom();
    
    return;
  }

  if (inputBuffer[4] == '=')
  {
    // We have some kind of command
    inputBuffer[4] = 0;
    if (strcmp(inputBuffer, "name") == 0)
    {
      // User wants to change the name
      Serial.print("Name command with name: ");
      Serial.println(inputBuffer + 5);

      storeName(inputBuffer + 5);
      return;
    }

    if (strcmp(inputBuffer, "vref") == 0)
    {
      // User wants to change the name
      Serial.print("vref command with value: ");
      Serial.println(inputBuffer + 5);

      storeVrefEeprom(inputBuffer+5);
      return;
    }
  }

  Serial.print("Unknown command: ");
  Serial.println(inputBuffer);
}

void loop()
{
   int sensorRaw; // Stores the value read from the sensor (should be 0 - 1024)
   float controlVoltage, variableVoltage;   // Stores the voltage value we need to print for user

   // Do the reading for the Control analog input

   // Read the control voltage first
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

   // We do the same steps as before, using different sensor for variable panel voltage
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
   if (handleInputAndDelay(60))
   {
     processInput();
   }

}
