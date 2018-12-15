int LDR_value,LDR_value_rec;
int push_button = 12, LED_out = 11, LDR_in = A0;

void setup() 
{
  pinMode(A0, INPUT);
  pinMode(11, OUTPUT);
  pinMode(12,INPUT);
  Serial.begin(9600);
  
}

void loop() 
{
  LDR_value = analogRead(A0); 
  Serial.println(LDR_value);
  analogWrite(11,0);
//  delay(500);
  
if(digitalRead(push_button) == 1)
  {
    delay(2000);
    LDR_value_rec = analogRead(LDR_in); 
      while(1)
    {
      LDR_value = analogRead(A0); 
      Serial.println(LDR_value_rec);
      if(LDR_value < LDR_value_rec)
        digitalWrite(LED_out, 1);
      else
        digitalWrite(LED_out, 0);
        delay(100);
        Serial.println("still in rec loop");
      if(digitalRead(push_button) == 1)
      {
        Serial.println("breaking");
        delay(1000);
         break;
      }
    }
  }
  
else
  { 
     while(1)
    {
     LDR_value = analogRead(A0); 
     Serial.println(LDR_value);
     if(LDR_value < 200)
      digitalWrite(LED_out, 1);
     else 
      digitalWrite(LED_out, 0);      
     if(digitalRead(push_button) == 1)
     {
       Serial.println("jumped loop");
       break;
     }
      
     }

   }
}

