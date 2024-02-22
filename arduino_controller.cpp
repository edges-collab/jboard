// Include necessary libraries
#include <Arduino.h>
#include <math.h>


// Define LED pin
const int MAX_SUBSTRINGS = 7;
const int temp_sensors=4;
int temp_sensor_pin[]={A0,A1,A2,A3};

//RFpath
const int D25 = 25;
const int D27 = 27;
const int D29 = 29;
const int D31 = 31;
const int D33 = 33;
const int D35 = 35;
const int D37 = 37;
const int D39 = 39;
const int D41 = 41;
const int D43 = 43;
const int D45 = 45;
const int D47 = 47;
//RFpath

String MEM = "MEM,nan";
String OOBSPDT = "OOBSPDT,nan";
String Atten = "Atten,nan";
String Hot = "Hot,nan";
String OOB = "OOB,nan";
String Noise = "Noise,nan";
String status;

float V1;

unsigned long this_time = 0;
unsigned long last_time =0;
unsigned long status_period = 5000; //milliseconds

//OOB attenuation
const int D22 = 22;
const int D24 = 24;
const int D26 = 26;
const int D28 = 28;
const int D30 = 30;
//OOB attenuation

//OOB_SPDT
const int D32 = 32;
const int D34 = 34;
//OOB_SPDT

const int D49 = 49;//Hot/ambient
const int D51 = 51;//OOB on/off
const int D53 = 53;//Noise on/off


int MEMS[19][12]={
    {1,0,0,0,0,0,0,0,0,1,1,0},
    {0,0,0,1,0,0,0,0,0,1,1,0},
    {0,1,0,0,0,0,1,1,0,1,1,0},
    {0,1,0,0,1,0,1,0,0,1,1,0},
    {0,0,0,0,0,0,0,0,0,1,0,1},
    {0,0,0,0,0,1,0,0,1,1,0,0},
    {0,0,1,0,0,0,0,0,0,1,1,0},
    {1,0,0,0,0,0,0,0,0,0,1,1},
    {0,0,0,1,0,0,0,0,0,0,1,1},
    {0,1,0,0,0,0,1,1,0,0,1,1},
    {0,1,0,0,1,0,1,0,0,0,1,1},
    {0,0,0,0,0,1,0,0,1,0,0,1},
    {0,0,1,0,0,0,0,0,0,0,1,1},
    {0,0,0,0,0,1,0,0,1,0,0,1},
    {0,0,0,0,0,0,0,1,1,0,0,1},
    {0,0,0,0,1,0,0,0,1,0,0,1},
    {0,0,0,0,0,0,0,1,1,1,0,0},
    {0,0,0,0,1,0,0,0,1,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
  };
int MEMS_1[12]={0,0,0,0,0,0,0,0,0,0,0,0};
int OOB_Attn[7][5]={
  {0,0,0,0,0},
  {0,1,1,1,1},
  {1,0,1,1,1},
  {1,1,0,1,1},
  {1,1,1,0,1},
  {1,1,1,1,0},
  {1,1,1,1,1}
};
int OOB_Attn_1[5]={1,1,1,1,1};

void setup() {
  // Start serial communication
  Serial.begin(9600);
  // Initialize LED pin as an output
  pinMode(D25, OUTPUT);
  pinMode(D27, OUTPUT);
  pinMode(D29, OUTPUT);
  pinMode(D31, OUTPUT);
  pinMode(D33, OUTPUT);
  pinMode(D35, OUTPUT);
  pinMode(D37, OUTPUT);
  pinMode(D39, OUTPUT);
  pinMode(D41, OUTPUT);
  pinMode(D43, OUTPUT);
  pinMode(D45, OUTPUT);
  pinMode(D47, OUTPUT);
  pinMode(D49, OUTPUT);
  pinMode(D51, OUTPUT);
  pinMode(D53, OUTPUT);
  pinMode(D22, OUTPUT);
  pinMode(D24, OUTPUT);
  pinMode(D26, OUTPUT);
  pinMode(D28, OUTPUT);
  pinMode(D30, OUTPUT);
  pinMode(D32, OUTPUT);
  pinMode(D34, OUTPUT);
 
  
  while (!Serial) {
    // Wait for the serial port to be opened
    //Serial.println("No Command received: ");
  }

  // Print a message to indicate that the Serial Monitor is active
  //Serial.println("Serial Monitor link is active!");

}

int splitString(String inputString, char delimiter, String substrings[]) {
  // Convert the String to a character array (c-string)
  char charArray[inputString.length() + 1];
  inputString.toCharArray(charArray, sizeof(charArray));

  // Use strtok to split the string into substrings
  char *token = strtok(charArray, ",");
  int count = 0;

  while (token != NULL && count < MAX_SUBSTRINGS) {
    // Store each substring in the array
    substrings[count] = String(token);
    
    // Get the next substring
    token = strtok(NULL, ",");
    count++;
  }

  return count;
}
void activate_MEMS(String command){
int command_MEMS=command.toInt();
//Serial.println(command_MEMS);
if (command_MEMS<19){
  for (int j = 0; j < 12; j++) {
        MEMS_1[j] = MEMS[command_MEMS][j];
      }
}
switch (command_MEMS) {
    case 0:
      //Serial.println("00:LNA-Antenna");
      // Add the code to be executed when the switch is pressed
      break;
    case 1:
      //Serial.println("01:LNA-Hot/ambient");
      // Add the code to be executed when the switch is not pressed
      break;
    case 2:
      //Serial.println("02:LNA-Longcable_Open");
      // Add the code to be executed when the switch is not pressed
      break;  
    case 3:
      //Serial.println("03:LNA-Longcable_Short");
      // Add the code to be executed when the switch is not pressed
      break;
    case 4:
      //Serial.println("04:VNA-LNA");
      // Add the code to be executed when the switch is not pressed
      break;
    case 5:
      //Serial.println("05:LNA-50ohm");
      // Add the code to be executed when the switch is not pressed
      break;
    case 6:
      //Serial.println("06:LNA-Noise_source");
      // Add the code to be executed when the switch is not pressed
      break;
    case 7:
      //Serial.println("07:VNA-Antenna");
      // Add the code to be executed when the switch is pressed
      break;
    case 8:
      //Serial.println("08:VNA-Hot/ambient");
      // Add the code to be executed when the switch is not pressed
      break;
    case 9:
      //Serial.println("09:VNA-Longcable_Open");
      // Add the code to be executed when the switch is not pressed
      break;  
    case 10:
      //Serial.println("10:VNA-Longcable_Short");
      // Add the code to be executed when the switch is not pressed
      break;
    case 11:
      //Serial.println("11:VNA-50ohm");
      // Add the code to be executed when the switch is not pressed
      break;
    case 12:
      //Serial.println("12:VNA-Noise_source");
      // Add the code to be executed when the switch is not pressed
      break;
    case 13:
      //Serial.println("13:VNA-50 ohm");
      // Add the code to be executed when the switch is not pressed
      break;
    case 14:
      //Serial.println("14:VNA-Open");
      // Add the code to be executed when the switch is not pressed
      break;
    case 15:
      //Serial.println("15:VNA-Short");
      // Add the code to be executed when the switch is not pressed
      break;
    case 16:
      //Serial.println("16:LNA-Open");
      // Add the code to be executed when the switch is not pressed
      break;
    case 17:
      //Serial.println("17:LNA-Short");
      // Add the code to be executed when the switch is not pressed
      break;
    case 18:
      //Serial.println("18:all_open");
      // Add the code to be executed when the switch is not pressed
      break;



    default:
      // This block is executed if switchState is neither HIGH nor LOW
      //Serial.println("Unknown switch state");
      break;
  }
  digitalWrite(D25, MEMS_1[0]);
  digitalWrite(D27, MEMS_1[1]);
  digitalWrite(D29, MEMS_1[2]);
  digitalWrite(D31, MEMS_1[3]);
  digitalWrite(D33, MEMS_1[4]);
  digitalWrite(D35, MEMS_1[5]);
  digitalWrite(D37, MEMS_1[6]);
  digitalWrite(D39, MEMS_1[7]);
  digitalWrite(D41, MEMS_1[8]);
  digitalWrite(D43, MEMS_1[9]);
  digitalWrite(D45, MEMS_1[10]);
  digitalWrite(D47, MEMS_1[11]);
}

void OOB_Attenuation(String command){
int command_OOB_attn=command.toInt();
//Serial.println(command_OOB_attn);
if (command_OOB_attn<7){
  for (int j = 0; j < 5; j++) {
        OOB_Attn_1[j] = OOB_Attn[command_OOB_attn][j];
      }
}
switch (command_OOB_attn) {
    case 0:
      //Serial.println("00:OOB Attenuation 31dB");
      //Serial.println(OOB_Attn_1[1]);
      
      break;
    case 1:
      //Serial.println("01:OOB Attenuation 16dB");
     
      break;
    case 2:
      //Serial.println("02:OOB Attenuation 8dB");
     
      break;  
    case 3:
      //Serial.println("03:OOB Attenuation 4dB");
      
      break;
    case 4:
      //Serial.println("04:OOB Attenuation 2dB");
      break;
    case 5:
      //Serial.println("05:OOB Attenuation 1dB");
      
      break;
    case 6:
      //Serial.println("06:OOB Attenuation 0dB");
      //Serial.println(OOB_Attn_1[1]);
      
      break;

    default:
      
      //Serial.println("Unknown OOB attenuation state");
      break;
  }
  digitalWrite(D22, OOB_Attn_1[0]);
  digitalWrite(D24, OOB_Attn_1[1]);
  digitalWrite(D26, OOB_Attn_1[2]);
  digitalWrite(D28, OOB_Attn_1[3]);
  digitalWrite(D30, OOB_Attn_1[4]);
}


void activate_OOBSPDT(String command){
int command_SPDT=command.toInt();
//Serial.println(command_SPDT);
if (command_SPDT==1){
  digitalWrite(D32, HIGH);
  digitalWrite(D34, LOW);
  //Serial.println("OOB SPDT Switch ON");
} else {
  digitalWrite(D32, HIGH);
  digitalWrite(D34, HIGH);
  //Serial.println("OOB SPDT Switch OFF");
}
}

void activate_Hotload(String command){
int command_SPDT=command.toInt();
//Serial.println(command_SPDT);
if (command_SPDT==1){
  digitalWrite(D49, HIGH);
  //Serial.println("Hot load ON");
} else {
  digitalWrite(D49, LOW);
  //Serial.println("Ambient, Hot load off");
}
}

void activate_OOB(String command){
int command_SPDT=command.toInt();
//Serial.println(command_SPDT);
if (command_SPDT==1){
  digitalWrite(D51, HIGH);
  //Serial.println("OOB noise ON");
} else {
  digitalWrite(D51, LOW);
  //Serial.println("OOB noise off");
}
}

void activate_Noise(String command){
int command_SPDT=command.toInt();
//Serial.println(command_SPDT);
if (command_SPDT==1){
  digitalWrite(D53, HIGH);
  //Serial.println("Noise source ON");
} else {
  digitalWrite(D53, LOW);
  //Serial.println("Noise source off");
}
}

void Temperature_mointor(String command){
int command_SPDT=command.toInt();
//Serial.println(command_SPDT);
if (command_SPDT==1){
  
  
  float C=((float)3.3/(float)1024.00001);
  float V11=analogRead(A0);
  float V21=analogRead(A1);
  float V31=analogRead(A2);
  float V41=analogRead(A3);
  float V1=C*V11;
  float V2=C*V21;
  float V3=C*V31;
  float V4=C*V41;
  float R1= ((V1)/(4.7001-V1))*10000;
  float R2= ((V2)/(4.7001-V2))*10000;
  float R3= ((V3)/(4.7001-V3))*10000;
  float R4= ((V4)/(4.7001-V4))*10000;
  float T1=1/(0.00095258922862+0.00022336130166*(log(R1))+0.000003852721985602*(pow(log(R1),2))+0.00000002222628015940*(pow(log(R1),3)))-273.15;
  float T2=1/(0.00095258922862+0.00022336130166*(log(R2))+0.000003852721985602*(pow(log(R2),2))+0.00000002222628015940*(pow(log(R2),3)))-273.15;
  float T3=1/(0.00095258922862+0.00022336130166*(log(R3))+0.000003852721985602*(pow(log(R3),2))+0.00000002222628015940*(pow(log(R3),3)))-273.15;
  float T4=1/(0.00095258922862+0.00022336130166*(log(R4))+0.000003852721985602*(pow(log(R4),2))+0.00000002222628015940*(pow(log(R4),3)))-273.15;

} 
}

void loop() {
  
  // Check if data is available to read from serial port
  while (Serial.available() > 0) {

    // Read an incoming command
    String command = Serial.readStringUntil('\n');

    // Parse into keyword and value pair
    String substrings[MAX_SUBSTRINGS];
    int numSubstrings = splitString(command, ',', substrings);
    
    if (numSubstrings == 2) {

      if (substrings[0]=="MEM"){
        activate_MEMS(substrings[1]);
        MEM=command;  // to return the status through com port
      } else if(substrings[0]=="OOBSPDT"){
        activate_OOBSPDT(substrings[1]);
        OOBSPDT=command;  // to return the status through com port
      } else if(substrings[0]=="Atten"){
        OOB_Attenuation(substrings[1]);
        Atten=command;  // to return the status through com port
      } else if(substrings[0]=="Hot"){
        activate_Hotload(substrings[1]);
        Hot=command;  // to return the status through com port
      } else if(substrings[0]=="OOB"){
        activate_OOB(substrings[1]);
        OOB=command;  // to return the status through com port
      } else if(substrings[0]=="Noise"){
        activate_Noise(substrings[1]);
        Noise=command;  // to return the status through com port
      } else if(substrings[0]=="Temp"){
        Temperature_mointor(substrings[1]);
      }
    }
  }
    

  // Check if it is time to write a status packet
  unsigned long this_time = millis();
  bool bPrintStatus = false;

  if (this_time < last_time) {
    if (this_time + (18446744073709551615 - last_time) >= status_period) {
      bPrintStatus = true;
    }
  } else if (this_time - last_time >= status_period) {
    bPrintStatus = true;
    last_time = this_time;
  }

  // Update our time tracker
  //last_time = this_time;
  

  if (bPrintStatus) {

    String status = MEM + OOBSPDT + Atten + Hot + OOB + Noise;  // to return the status through com port
    float C=((float)3.3/(float)1024.00001);
    float V11=analogRead(A0);
    float V21=analogRead(A1);
    float V31=analogRead(A2);
    float V41=analogRead(A3);
    float V1=C*V11;
    float V2=C*V21;
    float V3=C*V31;
    float V4=C*V41;
    float R1= ((V1)/(4.7001-V1))*10000; // 4.7001 is the volatge measured at the voltage divider
    float R2= ((V2)/(4.7001-V2))*10000; // 4.7001 is the volatge measured at the voltage divider
    float R3= ((V3)/(4.7001-V3))*10000; // 4.7001 is the volatge measured at the voltage divider
    float R4= ((V4)/(4.7001-V4))*10000; // 4.7001 is the volatge measured at the voltage divider

    float T1=1/(0.00095258922862+0.00022336130166*(log(R1))+0.000003852721985602*(pow(log(R1),2))+0.00000002222628015940*(pow(log(R1),3)))-273.15;
    float T2=1/(0.00095258922862+0.00022336130166*(log(R2))+0.000003852721985602*(pow(log(R2),2))+0.00000002222628015940*(pow(log(R2),3)))-273.15;
    float T3=1/(0.00095258922862+0.00022336130166*(log(R3))+0.000003852721985602*(pow(log(R3),2))+0.00000002222628015940*(pow(log(R3),3)))-273.15;
    float T4=1/(0.00095258922862+0.00022336130166*(log(R4))+0.000003852721985602*(pow(log(R4),2))+0.00000002222628015940*(pow(log(R4),3)))-273.15;
    
    status =String(T1) + ',' + String(T2) + ',' + String(T3) + ',' + String(T4) + ',' + MEM + ',' + OOBSPDT + ',' + Atten + ',' + Hot + ',' + OOB + ',' + Noise;
    Serial.println(status);
  
  } 
    
  // Add a short delay for stability
  delay(100);
}

