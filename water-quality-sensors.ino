// ==========================
// Water quality conductivity
// Sent via The Things Uno
// By Simon Loffler 2019
// ==========================

#include <TheThingsNetwork.h>
#include "secrets.h"
// OTAA authentication
const char *appEui = APPEUI;
const char *appKey = APPKEY;

// ABP authentication
const char *devAddr = DEVADDR;
const char *nwkSKey = NWKSKEY;
const char *appSKey = APPSKEY;

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915 or TTN_FP_AU915
#define freqPlan TTN_FP_AU915
    
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

// Atlas Scientific Conductivity K 1.0 sensors
// Use SoftwareSerial to send/receive from the sensors so we can still use debugSerial
#include <SoftwareSerial.h>
#define rx 10
#define tx 11
SoftwareSerial myserial(rx, tx);
// The incoming data string to communicate from serial monitor to the sensors
String inputstring = "";
boolean input_string_complete = false;
// The data string from the Atlas Scientific sensors
String sensorstring = "";
boolean sensor_string_complete = false;

void setup() {
  // The Things Uno setup
  loraSerial.begin(57600);
  debugSerial.begin(9600);  // Try 57600

  // Sensor setup
  myserial.begin(9600);  // Try 57600

  // Reserve some bytes for strings sent from the serial monitor
  inputstring.reserve(10);
  // Reserve some bytes for data from the sensors
  sensorstring.reserve(30);
      
  // Initialize LED output pin
  pinMode(LED_BUILTIN, OUTPUT);
    
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);
    
  debugSerial.println("-- STATUS");
  ttn.showStatus();

  // Uncomment to join the network using OTAA
//   debugSerial.println("-- JOIN");
//   ttn.join(appEui, appKey);

  // Uncomment to join the network using ABP
//  debugSerial.println("-- PERSONALIZE");
//  ttn.personalize(devAddr, nwkSKey, appSKey);

}

void loop() {

  // If a complete string has been received from the serial monitor, send it to the sensors
  if (input_string_complete) {
    myserial.print(inputstring);
    // Add a <CR> to the end of the string
    myserial.print('\r');
    // Clear and reset the flag to say we've completed sending
    inputstring = "";
    input_string_complete = false;
  }

  // If we receive a character from the sensors, add it to the sensorstring until we see a <CR>
  if (myserial.available() > 0) {
    char inchar = (char)myserial.read();
    sensorstring += inchar;
    if (inchar == '\r') {
      // It's the end of the sensor reading!
      sensor_string_complete = true;
    }
  }


  // If we're at the end of the sensor string, send it to the serial monitor and The Things Network
  if (sensor_string_complete == true) {
    if (isdigit(sensorstring[0]) == false) {
      // If the first character in the string is a digit send that to the serial monitor
      debugSerial.println(sensorstring);
    }
    else {
      // Print the sensor data
      print_EC_data();

      // Send sensor data to The Things Network

      // Convert sensorstring to a byte array
      byte data[sensorstring.length()];
      sensorstring.getBytes(data, sizeof(data));

      debugSerial.println("-- BYTE ARRAY TO SEND");
      for (int i=0; i < sizeof(data); i++){
         printHex(data[i]);
      }
      debugSerial.println();
      debugSerial.println();

      // Send the data
      ttn.sendBytes(data, sizeof(data));
    }
    // Clear and reset the sensorstring
    sensorstring = "";
    sensor_string_complete = false;
  }
}

void print_EC_data(void) {
  // Parse the sensor data and print it.
  char sensorstring_array[30];                        //we make a char array
  char *EC;                                           //char pointer used in string parsing
  char *TDS;                                          //char pointer used in string parsing
  char *SAL;                                          //char pointer used in string parsing
  char *GRAV;                                         //char pointer used in string parsing
  float f_ec;                                         //used to hold a floating point number that is the EC
  
  sensorstring.toCharArray(sensorstring_array, 30);   //convert the string to a char array 
  EC = strtok(sensorstring_array, ",");               //let's pars the array at each comma
  TDS = strtok(NULL, ",");                            //let's pars the array at each comma
  SAL = strtok(NULL, ",");                            //let's pars the array at each comma
  GRAV = strtok(NULL, ",");                           //let's pars the array at each comma

//  debugSerial.println("-- RAW SENSOR READING");
//  debugSerial.println(sensorstring);
//  debugSerial.println();

  debugSerial.println();
  debugSerial.println("-- SENSOR READING");
  debugSerial.print("EC:");                                //we now print each value we parsed separately
  debugSerial.println(EC);                                 //this is the EC value

  debugSerial.print("TDS:");                               //we now print each value we parsed separately
  debugSerial.println(TDS);                                //this is the TDS value

  debugSerial.print("SAL:");                               //we now print each value we parsed separately
  debugSerial.println(SAL);                                //this is the salinity value

  debugSerial.print("GRAV:");                              //we now print each value we parsed separately
  debugSerial.println(GRAV);                               //this is the specific gravity
  debugSerial.println();                                   //this just makes the output easier to read
  
//f_ec= atof(EC);                                     //uncomment this line to convert the char to a float
}

void printHex(char X) {
  // For debugging, print a byte to the console as a hex string

  if (X < 16) {debugSerial.print("0");}

  debugSerial.print(X, HEX);
  debugSerial.print(" ");

}
