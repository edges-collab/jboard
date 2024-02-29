#ifndef _DEFINES_H_
#define _DEFINES_H_

// Pins for MEMS switch path
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

// Pins for OOB attenuation
const int D22 = 22;
const int D24 = 24;
const int D26 = 26;
const int D28 = 28;
const int D30 = 30;

// Pins for OOB_SPDT
const int D32 = 32;
const int D34 = 34;

// Pins for calibration
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


#endif _DEFINES_H_





