#include <TheThingsNetwork.h>
const char *appEui = "70B3D57ED0015F29";
const char *appKey = "6BD409A13DD083BE428710ADECACC870";

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915 or TTN_FP_AU915
#define freqPlan TTN_FP_AU915
    
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

// Atlas Scientific Conductivity K 1.0 sensors
#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it
#define rx 10                                          //define what pin rx is going to be
#define tx 11                                          //define what pin tx is going to be
SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work
String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product

void setup() {
  // The Things Uno setup
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Sensor setup
  myserial.begin(9600);
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
      
  // Initialize LED output pin
  pinMode(LED_BUILTIN, OUTPUT);
    
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);
    
  debugSerial.println("-- STATUS");
  ttn.showStatus();

// Uncomment to join the network
//  debugSerial.println("-- JOIN");
//  ttn.join(appEui, appKey);
}

void serialEvent() {
  // For the sensors
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}

void loop() {
  // Sensor reading
  if (input_string_complete) {                        //if a string from the PC has been received in its entirety
    myserial.print(inputstring);                      //send that string to the Atlas Scientific product
    myserial.print('\r');                             //add a <CR> to the end of the string
    inputstring = "";                                 //clear the string
    input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the PC
  }

  if (myserial.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
    char inchar = (char)myserial.read();              //get the char we just received
    sensorstring += inchar;                           //add the char to the var called sensorstring
    if (inchar == '\r') {                             //if the incoming character is a <CR>
      sensor_string_complete = true;                  //set the flag
    }
  }


  if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
    if (isdigit(sensorstring[0]) == false) {          //if the first character in the string is a digit
      debugSerial.println(sensorstring);                   //send that string to the PC's serial monitor
    }
    else                                              //if the first character in the string is NOT a digit
    {
      print_EC_data();                                //then call this function 
    }
    sensorstring = "";                                //clear the string
    sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
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
