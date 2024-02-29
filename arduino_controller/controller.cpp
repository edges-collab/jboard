#include "controller.h"
#include "defines.h"


// -----------------------------------------------------------------------------
//
// Controller()
//
// -----------------------------------------------------------------------------
Controller::Controller() {

  m_ulLEDLastTime = 0;
  m_ulLEDPeriod = 500;
  m_bLEDState = false;
    
  m_ulTelemetryLastTime = 0;
  m_ulTelemetryPeriod = 5000;
  
  m_ulLastTime = 0;
  m_fUpTime = 0;
  
  m_iCommandCount = 0;
  m_iInputCount = 0;
  m_iOutputCount = 0;
  
  // Initialize our state record
  m_iMEMS = 0;
  m_iOOB = 0;
  m_iOOBAtten = 0;
  m_iOOBSPDT = 0;
  m_iHotLoad = 0;
  m_iNoise = 0;
  
  // Initialize the serial event parameters
  m_sSerialInput = "";
  m_sSerialInput.reserve(256);
  m_bSerialInputReady = false;
    
}


// -----------------------------------------------------------------------------
//
// ~Controller()
//
// -----------------------------------------------------------------------------
Controller::~Controller() {

  // Nothing to do
  
}



// -----------------------------------------------------------------------------
//
// diffTime(unsigned long, unsigned long)
//
// -----------------------------------------------------------------------------
unsigned long Controller::diffTime(unsigned long now, unsigned long before) {

  // The arduino millis() function we use for time cycles every ~50 days, so we
  // need to account for instances when now is smaller than before.
  if (now < before) {
    return now + (18446744073709551615 - before);
  } else {
    return now - before;
  }
}



// -----------------------------------------------------------------------------
//
// doInput()
//
// -----------------------------------------------------------------------------
void Controller::doInput(String sCmd, int iArg) {

  updateInputCount();
  
  if (sCmd == "MEM") {
    setMEMS(iArg);
    
  } else if (sCmd == "OOBSPDT") {
    setOOBSPDT(iArg);
    
  } else if (sCmd == "Atten") {
    setOOBAtten(iArg);

  } else if (sCmd == "Hot") {
    setHotLoad(iArg);

  } else if (sCmd == "OOB") {
    setOOB(iArg);

  } else if (sCmd == "Noise") {
    setNoise(iArg);

  } else if (sCmd == "Label") {
    sendTelemetry(0);
    updateCommandCount();
    
  } else if (sCmd == "Telem") {
    m_ulTelemetryPeriod = iArg; // milliseconds
    updateCommandCount();
  
  } else if (sCmd == "LED") { 
    m_ulLEDPeriod = iArg; // milliseconds
    updateCommandCount();
  }
}



// -----------------------------------------------------------------------------
//
// doTimers()
//
// -----------------------------------------------------------------------------
void Controller::doTimers() {

  unsigned long ulNow = millis();
  
  // Do LED timer
  if (diffTime(ulNow, m_ulLEDLastTime) >= m_ulLEDPeriod) {
  
    m_ulLEDLastTime = ulNow;
    onLEDTimer();
  }

  // Do telemetry timer
  if (diffTime(ulNow, m_ulTelemetryLastTime) >= m_ulTelemetryPeriod) {
  
    m_ulTelemetryLastTime = ulNow;
    onTelemetryTimer();
  }
  
  // Increment uptime
  m_fUpTime += ((float) diffTime(ulNow, m_ulLastTime)) / (1000.0*3600.0*24.0);
  m_ulLastTime = ulNow;
  
} 



// -----------------------------------------------------------------------------
//
// getThermistor(int)
//
// -----------------------------------------------------------------------------
float Controller::getThermistor(int iD) {

  float fVin = 4.7001;
  float fScale = 1e4;
  float fC =3.3 / 1024.00001;

  float f0 = 273.15;
  float f1 = 0.00095258922862;
  float f2 = 0.00022336130166;
  float f3 = 0.000003852721985602;
  float f4 = 0.00000002222628015940;
 
  float fADC = analogRead(iD);   
  float fV = fC * fADC;
  float fR = ((fV) / (fVin - fV)) * fScale;
  float fT = 1 / (f1 + f2 * log(fR) + f3 * pow(log(fR),2) + f4 * pow(log(fR),3)) - f0;
  
  return fT;
}



// -----------------------------------------------------------------------------
//
// onLEDTimer()
//
// -----------------------------------------------------------------------------
void Controller::onLEDTimer() {

  // Toggle the LED to the other state
  if (m_bLEDState) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  m_bLEDState = !m_bLEDState;

}


  
// -----------------------------------------------------------------------------
//
// onLoop()
//
// -----------------------------------------------------------------------------
void Controller::onLoop() {

  // Check if data is available from serial port
  if (m_bSerialInputReady) {
  
    // Parse into keyword and value pair
    String sCommand;
    String sArg;
    String sDelimiter = "=";
    
    int iIndex = m_sSerialInput.indexOf(sDelimiter);
    if ((iIndex > 0) && (iIndex == m_sSerialInput.lastIndexOf(sDelimiter))) {
      
      sCommand = m_sSerialInput.substring(0, iIndex);
      sCommand.replace(" ", "");

      sArg = m_sSerialInput.substring(iIndex+1);
      
      // Execute the command
      doInput(sCommand, sArg.toInt());    
    }
    
    // Reset the serial event parameters
    m_sSerialInput = "";
    m_bSerialInputReady = false;
  }
  
  // Handle timer driver events  
  doTimers();
}
  
  
  
// -----------------------------------------------------------------------------
//
// onSerialEvent()
//
// -----------------------------------------------------------------------------
void Controller::onSerialEvent() {

  while (Serial.available()) {
  
    char ch = (char) Serial.read();
    m_sSerialInput += ch;
    
    if (ch == '\n') {
      m_bSerialInputReady = true;
    }
  }
}
  
  

// -----------------------------------------------------------------------------
//
// onTelemetryTimer()
//
// -----------------------------------------------------------------------------
void Controller::onTelemetryTimer() {

  // Send a standard telemetry packet (mode=1)
  sendTelemetry(1);

}



// -----------------------------------------------------------------------------
//
// sendTelemetry(int)
//
// -----------------------------------------------------------------------------
// 0 = label packet
// 1 = standard telemetry packet
void Controller::sendTelemetry(int iMode) {
  
  // Send the type of packet specified by iMode
  if (iMode < 2) {
  
    // Allocate the telemetry string
    char chTlm[512];
  
    // Increment the packet counter
    m_iOutputCount++;

    if (iMode == 0) {

      // Build a label string packet
      sprintf(chTlm, "# Output Count, Input Count, Command Count, "
             "Uptime (Days), Temp 1 (K), Temp 2 (K), Temp 3 (K), Temp 4 (K), "
             "MEMS, OOB, OOBAtten, OOBSPDT, Hot Load, Noise, "
             "LED Period (ms), Telemetry Period (ms)");

    } else if (iMode == 1) {

      // Read thermistor temperatures
      float fT0 = getThermistor(A0);
      float fT1 = getThermistor(A1);
      float fT2 = getThermistor(A2);
      float fT3 = getThermistor(A3);

      // Build a standard telemetry packet
      sprintf(chTlm, "%d, %d, %d, "
                     "%8.3f, %6.2f, %6.2f, %6.2f, %6.2f, "
                     "%d, %d, %d, %d, %d, %d, "
                     "%d, %d", 
        m_iOutputCount, m_iInputCount, m_iCommandCount,
        m_fUpTime, fT0, fT1, fT2, fT3, 
        m_iMEMS, m_iOOB, m_iOOBAtten, m_iOOBSPDT, m_iHotLoad, m_iNoise, 
        m_ulLEDPeriod, m_ulTelemetryPeriod );
    }

    // Send the packet to serial port -- we do this one character at a time
    // since Serial.print blocks if the buffer is full.
    int iIndex = 0;
    int iLength = strlen(chTlm);
    
    while (iIndex < iLength) {
      Serial.print(chTlm[iIndex++]);
    }
    
    Serial.print('\n');
  }
}



// -----------------------------------------------------------------------------
//
// setMEMS(int)
//
// -----------------------------------------------------------------------------
void Controller::setMEMS(int iArg) {

  if (iArg < 19) {
   
    digitalWrite(D25, MEMS[iArg][0]);
    digitalWrite(D27, MEMS[iArg][1]);
    digitalWrite(D29, MEMS[iArg][2]);
    digitalWrite(D31, MEMS[iArg][3]);
    digitalWrite(D33, MEMS[iArg][4]);
    digitalWrite(D35, MEMS[iArg][5]);
    digitalWrite(D37, MEMS[iArg][6]);
    digitalWrite(D39, MEMS[iArg][7]);
    digitalWrite(D41, MEMS[iArg][8]);
    digitalWrite(D43, MEMS[iArg][9]);
    digitalWrite(D45, MEMS[iArg][10]);
    digitalWrite(D47, MEMS[iArg][11]);
   
    // Store the current state
    m_iMEMS = iArg;
       
    updateCommandCount();
  }
}



// -----------------------------------------------------------------------------
//
// setOOBAtten(int)
//
// -----------------------------------------------------------------------------
void Controller::setOOBAtten(int iArg) {

  if (iArg < 5) {

    digitalWrite(D22, OOB_Attn[iArg][0]);
    digitalWrite(D24, OOB_Attn[iArg][1]);
    digitalWrite(D26, OOB_Attn[iArg][2]);
    digitalWrite(D28, OOB_Attn[iArg][3]);
    digitalWrite(D30, OOB_Attn[iArg][4]);
    
    // Store the current state
    m_iOOBAtten = iArg;
        
    updateCommandCount();
  }
}



// -----------------------------------------------------------------------------
//
// setOOBSPDT(int)
//
// -----------------------------------------------------------------------------
void Controller::setOOBSPDT(int iArg) {

  if (iArg < 2) {
  
    if (iArg == 1) {
      digitalWrite(D32, HIGH);
      digitalWrite(D34, LOW);
    } else {
      digitalWrite(D32, HIGH);
      digitalWrite(D34, HIGH);
    }
    
    // Store the current state
    m_iOOBSPDT = iArg;
        
    updateCommandCount();
  }
}



// -----------------------------------------------------------------------------
//
// setHotLoad(int)
//
// -----------------------------------------------------------------------------
// 0: Ambient load (hot load off)
// 1: Hot load
void Controller::setHotLoad(int iArg) {

  if (iArg < 2) {
 
    if (iArg == 1) {
      digitalWrite(D49, HIGH);
    } else {
      digitalWrite(D49, LOW);
    }
   
    // Store the current state
    m_iHotLoad = iArg;
        
    updateCommandCount();
  }
}



// -----------------------------------------------------------------------------
//
// setOOB(int)
//
// -----------------------------------------------------------------------------
// 0: OOB noise off
// 1: OOB noise on
void Controller::setOOB(int iArg) {

  if (iArg < 2) {
  
    if (iArg == 1) {
      digitalWrite(D51, HIGH);
    } else {
      digitalWrite(D51, LOW);
    }
    
    // Store the current state
    m_iOOB = iArg;
     
    updateCommandCount();
  }  
}



// -----------------------------------------------------------------------------
//
// setNoise(int)
//
// -----------------------------------------------------------------------------
// 1: Noise source on
// 0: Noise source off
void Controller::setNoise(int iArg) {

  if (iArg < 2) {
  
    if (iArg == 1) {
      digitalWrite(D53, HIGH);
    } else {
      digitalWrite(D53, LOW);
    }
   
    // Store the current state
    m_iNoise = iArg;    
    updateCommandCount();
  }      
}



// -----------------------------------------------------------------------------
//
// setupPins()
//
// -----------------------------------------------------------------------------
void Controller::setupPins() {

  // Enable LED control
  pinMode(LED_BUILTIN, OUTPUT);
   
  // Initialize other control pins
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
  
}



// -----------------------------------------------------------------------------
//
// setupState()
//
// -----------------------------------------------------------------------------
void Controller::setupState(int iMEMS, int iOOB, int iOOBAtten, int iOOBSPDT,
  int iHotLoad, int iNoise) {
  
  setMEMS(iMEMS);
  setOOB(iOOB);
  setOOBAtten(iOOBAtten);
  setOOBSPDT(iOOBSPDT);
  setHotLoad(iHotLoad);
  setNoise(iNoise);
}



// -----------------------------------------------------------------------------
//
// updateCommandCount()
//
// -----------------------------------------------------------------------------

void Controller::updateCommandCount() {

  m_iCommandCount++;
}


// -----------------------------------------------------------------------------
//
// updateInputCount()
//
// -----------------------------------------------------------------------------

void Controller::updateInputCount() {

  m_iInputCount++;
}
  

