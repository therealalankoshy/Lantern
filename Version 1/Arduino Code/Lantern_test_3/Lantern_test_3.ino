long mapped_value, LDR_value, LDR_value_rec, avg_value=0, before_on, adjust, prev_avg;
int push_button = A2, Relay_out = A1, LDR_in = A0, i=0, j=0;

void setup() 
{
  pinMode(A0, INPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  Serial.begin(9600);  
}

void loop() 
{   

      if(digitalRead(full_night)==1) ////////////////////////light to be ON for whole night
      {
      Serial.println("***ON for whole night***");
      Serial.println("\n");
                   
          for(i=0;i<35;i++) // for finding adjust after light on 
          {
            LDR_value = analogRead(A0) - adjust;
            avg_value = LDR_value+avg_value;
            Serial.print("Value with light on : ");
            Serial.println(avg_value);                     
                  
            if(digitalRead(push_button) == 1)  //if button pressed break
            {
              digitalWrite(Relay_out,1);
              Serial.println("breaking after on");           
              break;
            }                 
             delay(25); // 2. make it 5000 for 3 min, 2500 for 1.5 min
          }
          
          avg_value = avg_value/36;
          adjust = avg_value - before_on;
          Serial.print("Adjust value : ");
          Serial.println(adjust); //till here    

             
          for(i=0;i<35;i++)//for finding one previous value 
          {
            LDR_value = analogRead(A0) - adjust;
            avg_value = LDR_value+avg_value;                      
                  
            if(digitalRead(push_button) == 1)  //if button pressed break
            {
              digitalWrite(Relay_out,1);
              Serial.println("breaking after on");           
              break;
            }                            
             delay(25); // 3. make it 5000 for 3 min, 2500 for 1.5 min
          }
          prev_avg = avg_value/36; //till here
          
        while(1)
        {
          Serial.println("***Inside the loop after bulb on***"); 
          Serial.println("\n");
          avg_value=0;
          
          for(i=0;i<35;i++)
          {
            LDR_value = analogRead(A0) - adjust;
            avg_value = LDR_value+avg_value;                      
                  
            if(digitalRead(push_button) == 1)  //if button pressed break
            {
              digitalWrite(Relay_out,1);
              Serial.println("breaking after ON");           
              break;
            }                                
             delay(5); // 4. make it 5000 for 3 min, 2500 for 1.5 min
          }
          
         
               
             if(digitalRead(push_button) == 1)  //if button pressed break
             {
               digitalWrite(Relay_out,1);
               Serial.println("Breaking again");
               break;  
             }
      
            
            avg_value=avg_value/36;
            Serial.print("Current Value with bulb on : ");
            Serial.println(avg_value); 
            Serial.print("Recorded Value : ");
            Serial.println(LDR_value_rec);           
            
                         
            if(avg_value > LDR_value_rec || avg_value-prev_avg > 60) 
            {                      
             digitalWrite(Relay_out, 1);
             Serial.println("Sun is up");
             break;
            }             
         }
        }
      
        
     }
      
   
      else
        digitalWrite(Relay_out, 1);
        
      if(digitalRead(push_button) == 1)  //if button pressed break with blink for normal loop
      {
        Serial.println("Breaking again");
        //Blinking 3 times
        digitalWrite(Relay_out,1);
        delay(300);
        digitalWrite(Relay_out,0);
        delay(300);
        digitalWrite(Relay_out,1);
        delay(300);
        digitalWrite(Relay_out,0);
        delay(300);
        digitalWrite(Relay_out,1);
        delay(300);
        digitalWrite(Relay_out,0);
        delay(300);
        digitalWrite(Relay_out,1);
        delay(300);   
                   
        break;        
      }
    }       
  
  }


  
else ///////////////////////////////////////////////////////////normal loop
  {   
     digitalWrite(Relay_out, 1);
     while(1)
    {
     Serial.println("***Normal loop***");     
     
     avg_value=0;
     LDR_value = analogRead(A0); 

      for(i=0;i<35;i++)
      {
        LDR_value = analogRead(A0);
        avg_value=LDR_value+avg_value;
        
        if(digitalRead(push_button) == 1)  //if button pressed break
         {
           digitalWrite(Relay_out,1);
           Serial.println("breaking");
           break;
         }
        delay(25); // 5. make it 5000 for 3 min, 2500 for 1.5 min
      }
      
     avg_value=avg_value/36;
     Serial.print("Current Value : ");
     Serial.println(avg_value);
     before_on = avg_value;
     
     if(avg_value < 100)
     {
      
        digitalWrite(Relay_out, 0);
        delay(5000); //for testing to turn on phone torch or letting the light turn on completely   
          
         for(i=0;i<35;i++) // for finding adjust after light on 
          {
            LDR_value = analogRead(A0) - adjust;
            avg_value = LDR_value+avg_value;
            Serial.print("Value with light on : ");
            Serial.println(avg_value);                     
                  
            if(digitalRead(push_button) == 1)  //if button pressed break
            {
              digitalWrite(Relay_out,1);
              Serial.println("breaking after on");           
              break;
            }            
           
             delay(25); // 2. make it 5000 for 3 min, 2500 for 1.5 min
          }
          
          avg_value = avg_value/36;
          adjust = avg_value - before_on;
          Serial.print("Adjust value : ");
          Serial.println(adjust); //till here

        
      
    
    
    if(digitalRead(full_night)==1) //light to be ON whole night
     {
      while(1)
      {
     Serial.println("***ON for Whole night***");    
     
     avg_value=0;
     LDR_value = analogRead(A0); 

      for(int i=0;i<35;i++)
      {
        LDR_value = analogRead(A0) - adjust;
        avg_value=LDR_value+avg_value;
        
        if(digitalRead(push_button) == 1)  //if button pressed break
         {
           digitalWrite(Relay_out,1);
           Serial.println("breaking");
           break;
         }
         
        delay(25); // 5. make it 5000 for 3 min, 2500 for 1.5 min
      }
      
     avg_value=avg_value/36;
     Serial.print("Current Value : ");
     Serial.println(avg_value);
     
     if(avg_value < 100)
      digitalWrite(Relay_out, 0);
     else 
      digitalWrite(Relay_out, 1);
      
     if(digitalRead(push_button) == 1)  //if button pressed break
     {
       digitalWrite(Relay_out,1);
       Serial.println("Breaking again");
       break;  
     }
          
    }
    }
    break;
    }
    if(digitalRead(push_button) == 1)  //if button pressed break
     {
       digitalWrite(Relay_out,1);
       Serial.println("Breaking again");
       break;  
     }
    
       }   
               
     }

   }
















