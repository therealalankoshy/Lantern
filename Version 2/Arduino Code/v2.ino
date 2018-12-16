int Received_Bit_Check [3] ={0,0,0} ;
float ReceptionTime = 5000; // 5 mins = 5000
double EndTime;
double StartTime;

const int analogInPin = A0;
uint32_t startTime_uS;
uint32_t endTime_uS;

int16_t values[128];
int i,j;

                                // 0 , 1 
float Freq_TimeData[3][2] = { { 1100.0, 2100.0 },   // 1st bit 
                            { 3100.0, 4100.0 },   // 2nd bit
                            { 5100.0, 6100.0 } }; // 3rd bit

float Threshold_TimeData[3][2] = { { 0.5, 0.1 },
                                 { 0.22, 0.1 },
                                 { 0.22, 0.22 } };

  
float Single_Point_DFT(int16_t inputArray[], int startingIndex, int endingIndex, float sampleFreq, float freqToSampleFor) 
{
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

int BinaryToDecimal(int binary[])
{
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


int Received_value()
{
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


void setup() 
{
  Serial.begin(9600);
  double EndTime = StartTime = 0;
}


void loop() 
{
    int Binary = Received_value();
    int Decimal = BinaryToDecimal(Binary);
}