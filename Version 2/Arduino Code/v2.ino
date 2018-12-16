#include <EEPROM.h>

int i, j, k,
    RECvalue,
    LDRdirect,
    LDRmapped,
    BEFOREextLIGHT = 0,
    AVERAGEvalue = 0,
    NEWAVERAGEvalue = 0,
    PREVIOUSvalue = 0,
    VALUEbeforeON = 0,
    adjust = 0,
    calculated = 0,
    RECcal = 0,
    addr = 99,
    NIGHTswitch, //NIGHTswitch == 1 - Wholenight|| NIGHTswitch==0 - 5 hours
    LIGHTstate = 0; //LIGHTstate == 1 - ON|| LIGHTstate==0 - OFF

volatile boolean STATE = true; // STATE = True Predefined intensity | STATE = False Recorded intensity

int LED = 12,
    RELAYout = 10; 

float Freq_FullNight = 1600.0;
float Freq_SixHours = 2600.0;
float Freq_Record = 3600.0;

float Threshold_FullNight = 0.0;
float Threshold_SixHours = 0.05;
float Threshold_Record = 0.0009;

int Received_Bit_Check [3] ={0,0,0} ;
float ReceptionTime = 5000; // 5 mins = 5000
double EndTime;
double StartTime;

const int analogInPin = A1;
uint32_t startTime_uS;
uint32_t endTime_uS;

int16_t values[128];

                              // 0 , 1 
float Freq_TimeData[3][2] = { { 1100.0, 2100.0 },   // 1st bit 
                            { 3100.0, 4100.0 },   // 2nd bit
                            { 5100.0, 6100.0 } }; // 3rd bit

float Threshold_TimeData[3][2] = { { 0.5, 0.1 },
                                 { 0.22, 0.1 },
                                 { 0.22, 0.22 } };



float Single_Point_DFT(int inputArray[], int startingIndex, int endingIndex, float sampleFreq, float freqToSampleFor) 
{
  //Serial.println("entered Single_Point_DFT");
  float N = endingIndex - startingIndex + 1; //N=number of samples
  float k = freqToSampleFor * N / sampleFreq;
  float radiansPerSample = 2.0 * 3.14159 * k / N;

  float RealSum = 0;
  float ImaginarySum = 0;
  for (int n = 0; n <= endingIndex - startingIndex; n++) {
    float angle = n * radiansPerSample;
    float voltage = inputArray[startingIndex + n] * (5.0 / 1023.0);

    RealSum += voltage * cos(angle);
    ImaginarySum -= voltage * sin(angle);
  }

  return (RealSum * RealSum + ImaginarySum * ImaginarySum) / N;
}


void BLINKled(int x, int y) // x= no of blink; y speed (750 is fast 1000 is slow)
{
  digitalWrite(LED, 0);
  for (i = 0; i < x; i++) {
    for (j = 0; j < y; j++) {
      digitalWrite(LED, 1);
      delayMicroseconds(j);
      digitalWrite(LED, 0);
      delayMicroseconds(y - j);
    }
    for (j = 0; j < y; j++) {
      digitalWrite(LED, 1);
      delayMicroseconds(y - j);
      digitalWrite(LED, 0);
      delayMicroseconds(j);
    }
  }
}

void DELAYwithaudiocheck(float d) {
  for (k = 0; k < 1000; k++) {
    delay(d);
    Listen_Audio();
  }
}

// void BUTTONcheck() {
//   attachInterrupt(digitalPinToInterrupt(PUSHbutton), STATEchange, RISING);
// }

void STATEchange() {
  adjust = 0;
  digitalWrite(RELAYout, 0);
  digitalWrite(LED, 1);



  if (STATE) {
    STATE = false;
    EEPROM.write(addr + 1, 0); // Light mode (REC or PRE defined)  
  } else {
    STATE = true;
    EEPROM.write(addr + 1, 1); // Light mode (REC or PRE defined)          
  }
  //Serial.println("Interrupt"); // STATE = True Predefined intensity | STATE = False Recorded intensity  
}

void LDRvalue() {
  LDRdirect = analogRead(A0);
  LDRmapped = map(LDRdirect, 0, 1024, 0, 255);
  //Serial.print("LDR mapped ");
  //Serial.println(LDRmapped);
}

void Listen_Audio ()
{
  //Serial.println("entered listen audio");
  Check_FullNightMode();
  Check_SixHoursMode();
  Check_RecordMode();
}

float Find_DFT(float Test_Freq)
{
    startTime_uS = micros();
    int values = Find_128_sample_values();
    endTime_uS = micros();
    float sampleFreq = 128.0*1000000.0/(endTime_uS - startTime_uS);
    float dft = Single_Point_DFT(values, 0, 127, sampleFreq, Test_Freq);
    return dft;
}

int Find_128_sample_values()
{   
    for(uint8_t n = 0; n<128; n++)
    {
       values[n] = analogRead(analogInPin);
    }    
    return(values);
}

void Check_FullNightMode()
{
  Serial.print("entered Check_FullNightMode");
    float FullNight_DFT = Find_DFT(Freq_FullNight);
    Serial.println(FullNight_DFT);
    if (FullNight_DFT > Threshold_FullNight)
    { 
      Serial.println("FullNightMode");
        NIGHTswitch = 1;
    }
}

void Check_SixHoursMode()
{
    Serial.print("entered Check_SixHoursMode");
    float SixHours_DFT = Find_DFT(Freq_SixHours);
    Serial.println(SixHours_DFT);
    if (SixHours_DFT > Threshold_SixHours)
    {
      Serial.println("SixHours_DFT");
        NIGHTswitch = 0;
    }
}

void Check_RecordMode()
{
  Serial.print("entered Check_RecordMode");
  float Record_DFT = Find_DFT(Freq_Record);
  Serial.println(Record_DFT);
  if (Record_DFT > Threshold_Record)
  {
    STATE = true;
    BLINKled(10,750);
    int Binary = Received_value();
    int Decimal = BinaryToDecimal(Binary);
    Send_On_Time(Decimal);

  }
}

void Send_On_Time(int Time)
{
  //Serial.println("entered Send_On_Time");
  int TurnOnDelay = Time*60*60; // Time delay in seconds.
  DELAYwithaudiocheck(TurnOnDelay);
  
}

int BinaryToDecimal(int binary[])
{
  //Serial.println("entered BinaryToDecimal");
  int i=0;
  int j=3;
  int b[4];
  int decimal = 0;
  int power [4]= {1,2,4,8};
  while (j>=0)
  {
    b[i]=binary[j];
    decimal = ( power[i]*b[i] ) + decimal;      
    j--;
    i++;
  }
  return decimal;
}

int Received_value()
{
  //Serial.println("entered Received_value");
  int Received_value[3];
  StartTime = millis();
  while (Received_Bit_Check[0] == 0 || Received_Bit_Check[1] == 0 || Received_Bit_Check[2] == 0)
  {
    if ((EndTime - StartTime) < ReceptionTime)
    {
      for (int i = 0 ; i<3 ; i++)  
      { 
        if (Received_Bit_Check[i] == 0)
        {
          for (int j = 0 ; j< 2 ; j++)
          {
            int DFT = Find_DFT(Freq_TimeData[i][j]);

            if (DFT > Threshold_TimeData[i][j])
            {
              if ( j == 0 )
              {
                Received_Bit_Check[i] = 1;
                Received_value [i] = 0;
              }
              else 
              {
                Received_Bit_Check[i] = 1;
                Received_value [i] = 1;
              }
            }
          }
        }  
      }      
    EndTime = millis();
    }
    else
    {
      break;
    }
  }
  return Received_value;
}

int AVGvalue(int f) {
  AVERAGEvalue = 0;
  
  for (i = 1; i < f + 1; i++) // 60 for a minute
  {
    LDRvalue();
    Serial.print("entered AVGvalue");
    Listen_Audio();

    if (abs(LDRmapped - BEFOREextLIGHT) > 50 && LIGHTstate == 1) {
      CHECKexternalLIGHTS();
    }
    AVERAGEvalue = LDRmapped + AVERAGEvalue - adjust;
    BEFOREextLIGHT = AVERAGEvalue / i + adjust;
    //Serial.print("BEFOREextLIGHT ");
    //Serial.println(BEFOREextLIGHT);
    DELAYwithaudiocheck(1); // 1sec delay  dont adjust this use d        
  }
  //Serial.print("Adjust ");
  //Serial.println(adjust);
  AVERAGEvalue = AVERAGEvalue / f;
  //Serial.print("Average value ");
  //Serial.println(AVERAGEvalue);
  //Serial.print("BEFOREextLIGHT ");
  //Serial.println(BEFOREextLIGHT);
  return AVERAGEvalue;
}

int NEWAVGvalue(int f) {
  NEWAVERAGEvalue = 0;
  for (i = 0; i < f; i++) // 60 for a minute
  {
    LDRvalue();
    NEWAVERAGEvalue = LDRmapped + NEWAVERAGEvalue;
    (1); // 1sec delay  dont adjust this used        
  }
  NEWAVERAGEvalue = NEWAVERAGEvalue / f;
  //Serial.print("NEWAVERAGEvalue ");
  //Serial.println(NEWAVERAGEvalue);
  return NEWAVERAGEvalue;
}

void RECORD() {
  //Serial.println("***** RECORD *****");
  while (RECcal == 0) {
    RECvalue = AVGvalue(10);
    //Serial.print("Recorded Value ");
    //Serial.println(RECvalue);
    EEPROM.write(addr, RECvalue);
    RECcal = 1;
  }
  if (digitalRead(NIGHTswitch) == 1)
    WHOLEnight(RECvalue);
  if (digitalRead(NIGHTswitch) == 0)
    PARTIALnight(RECvalue);
}

void PREdefined(int g) {
  RECcal = 0; // when record function is broke
  Serial.println("***** PREdefined *****");
  if (NIGHTswitch == 1)
    WHOLEnight(g);
  if (NIGHTswitch == 0)
    PARTIALnight(g);
}

void WHOLEnight(int g) {
  Serial.println("___WHOLEnight___");
  PREVIOUSvalue = AVGvalue(10); //Also used to find AVERAGEvalue || use AVGvalue(58) for a minute including AVGvalue(5) in CHECKexternalLIGHTS()  
  //Serial.print("PREVIOUSvalue ");
  //Serial.println(PREVIOUSvalue);
  if (AVERAGEvalue < g || AVERAGEvalue == g) {
    //Serial.println("LIGHTS ON");
    digitalWrite(RELAYout, 1);
    LIGHTstate = 1;
    while (calculated == 0) {
      VALUEbeforeON = PREVIOUSvalue;
      //Serial.print("VALUEbeforeON ");
      //Serial.println(VALUEbeforeON);
      DELAYwithaudiocheck(5); // wait 120 sec for the intensity around the surroundings to get stable and not flicker(turn off and on)
      adjust = AVGvalue(5) - VALUEbeforeON; // 30 values to fing adjust AVGvalue(30)     
      AVERAGEvalue = AVGvalue(5) - adjust;
      calculated = 1;
    }
  }

  if (AVERAGEvalue > g) {
    //Serial.println("LIGHTS OFF");
    adjust = 0;
    digitalWrite(RELAYout, 0);
    calculated = 0;
    LIGHTstate = 0;
  }
}

void CHECKexternalLIGHTS() {
  NEWAVGvalue(5); // 5values to check external lights
  //Serial.println("External lighting");
  adjust = NEWAVERAGEvalue - VALUEbeforeON;
  //Serial.print("VALUEbeforeON ");
  //Serial.println(VALUEbeforeON);
  //Serial.print("Adjust modified ");
  //Serial.println(adjust);
}

void PARTIALnight(int g) {
  //Serial.println("___PARTIALnight___");
  AVGvalue(10);
  if (AVERAGEvalue < g) {
    //Serial.println("LIGHTS ON");
    digitalWrite(RELAYout, 1);
    delay(5000);
    DELAYwithaudiocheck(300); //18000 for 5 hours (5*60*60)

    //Serial.println("LIGHTS OFF");
    digitalWrite(RELAYout, 0);
    DELAYwithaudiocheck(600); //50400 for 14hours off time    
  }
}

void setup() {
  pinMode(A0, INPUT);
  pinMode(9, INPUT);
  pinMode(10, OUTPUT);
  pinMode(11, INPUT);
  pinMode(12, OUTPUT);
  
  digitalWrite(RELAYout,0);
  
  NIGHTswitch = 1;
  
  double EndTime = StartTime = 0;
  
  Serial.begin(9600);

  BLINKled(3, 700);

  LDRvalue();
  BEFOREextLIGHT = LDRmapped;

  STATE = true;
  
  
  if (EEPROM.read(addr + 1) == 0) // Light mode (REC or PRE defined)  
  {
    RECcal = 1;
    RECvalue = EEPROM.read(addr);
    //Serial.print("------Reading EEPROM------");
    //Serial.print("Recorded Value (After supply interrupted) ");
    //Serial.println(RECvalue);
    STATE = false;
  }
}

void loop() {
  Serial.println("               ");
  Serial.println("##########MAIN LOOP##########");
  if (STATE == true) {
    BLINKled(2, 600);
    Serial.println("Predefined Value state");
    PREdefined(80); // predefined value 80 for light to turn on
  }
  if (STATE == false) {
    //Serial.println("recorded Value state");
    BLINKled(4, 600);
    RECORD();
  }

}
