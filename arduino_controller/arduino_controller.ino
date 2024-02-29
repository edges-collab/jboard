#include "controller.h"

// NOTE:
// Arduinos automatically reset when the USB serial is connected.  This can be
// triggered when the host closes its serial connection, sending a hangup
// signal, and then reopens its serial connection (even if the USB connector was
// never removed.  To partly avoid this behavior, after connecting the Arduino 
// USB to the host, run on the host "stty -F /dev/<my-device> -hupcl" to disable
// the hangup signal, keeping the connection alive after you close the serial
// port.  See:
// 
// https://playground.arduino.cc/Main/DisablingAutoResetOnSerialConnection/


// =============================================================================
// 
// GLOBAL VARIABLES
//
// =============================================================================

// Global object that implements all of the functionality
Controller g_ctl;



// =============================================================================
// 
// MAIN EXECUTION FUNCTIONS
//
// =============================================================================

// -----------------------------------------------------------------------------
//
// setup()
//
// -----------------------------------------------------------------------------
// Called before other functions
void setup() {

  // Start serial communication
  Serial.begin(9600);
    
  // Configure the GPIO pins to the desired modes
  g_ctl.setupPins();
  
  // Set the board to the default state
  g_ctl.setupState(0, 0, 0, 0, 0, 0);
     
}


// -----------------------------------------------------------------------------
//
// loop()
//
// -----------------------------------------------------------------------------
// Automatically called repeatedly
void loop() {
  g_ctl.onLoop();    
}


// -----------------------------------------------------------------------------
//
// serialEvent()
//
// -----------------------------------------------------------------------------
// Automatically called after each loop()
void serialEvent() {
  g_ctl.onSerialEvent();
}






