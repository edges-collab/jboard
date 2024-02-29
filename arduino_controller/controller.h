#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "Arduino.h"

class Controller {

  private:

    // Member variables
    unsigned long m_ulLEDLastTime;
    unsigned long m_ulLEDPeriod;
    bool          m_bLEDState;
    
    unsigned long m_ulTelemetryLastTime;
    unsigned long m_ulTelemetryPeriod;
    
    unsigned long m_ulLastTime;
    float         m_fUpTime;
    
    int           m_iInputCount;
    int           m_iCommandCount;
    int           m_iOutputCount;
    
    int           m_iMEMS;
    int           m_iOOB;
    int           m_iOOBAtten;
    int           m_iOOBSPDT;
    int           m_iHotLoad;
    int           m_iNoise;
    
    String        m_sSerialInput;
    bool          m_bSerialInputReady;
    
    // Private helper functions   
    
    unsigned long diffTime(unsigned long, unsigned long);
    float         getThermistor(int);
    void          onLEDTimer();
    void          onTelemetryTimer(); 
    void          sendTelemetry(int);
    void          setMEMS(int);
    void          setOOB(int);
    void          setOOBAtten(int);
    void          setOOBSPDT(int);
    void          setHotLoad(int);
    void          setNoise(int);
    void          updateCommandCount();
    void          updateInputCount();
    

  public:

    Controller();
    ~Controller();

    void          doInput(String, int);
    void          doTimers();
    void          onLoop();
    void          onSerialEvent();
    void          setupPins();
    void          setupState(int, int, int, int, int, int);

};



#endif // _CONTROLLER_H_





