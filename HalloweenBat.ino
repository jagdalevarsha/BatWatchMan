//   Halloween Bat
 
//   Arduino Circuit Diagram:
//   
//   Connection for URM Proximity V3.2 to Arduino
//   Pin 1 VCC -> VCC 
//   Pin 2 GND -> GND 
//   Pin 4 PWM -> Pin 3 
//   Pin 6 COMP/TRIG -> Pin 5 
//
//   Connection for LED to Arduino
//   Positive LED terminal -> Pin 13
//   Negative LED terminal -> Resistor -> GND
//   
//   Connection for Servo Motors to Arduino
//   Servo1 -> Pin 9
//   Servo2 -> Pin 10

// Including servo library
#include <Servo.h>  

// PWM Output 0－25000US，Every 50US represent 1cm
int URPWM = 3; 
// PWM trigger pin
int URTRIG = 5; 
//Setting the proximity range to 15 cm
int RangeLow = 0;
int RangeHigh = 15;
//Distance returned by the proximity sensors
unsigned int Distance=0;
//Distance measure command 
uint8_t EnPwmCmd[4]={0x44,0x02,0xbb,0x01};

//Led Connection
int led = 13;

//Servo control objects
Servo servo1;  
Servo servo2;
 
void setup()
{
    // Setting Led to output
    pinMode(led, OUTPUT);
    
    // Serial initialization, for sending commands to processing
    // Sets the baud rate to 9600

    Serial.begin(9600);  
  
    //Setting up the servos 
    servo1.attach(9);
    servo2.attach(10);
   
    PWM_Mode_Setup();
}
 
void loop()
{
   PWM_Mode();
   
   //Read the proximity sensor input after every second
   delay(1000);
}                     
 
 
void PWM_Mode_Setup()
{ 
  pinMode(URTRIG,OUTPUT);                     // A low pull on pin COMP/TRIG
  digitalWrite(URTRIG,HIGH);                  // Set to HIGH 
  pinMode(URPWM, INPUT);                      // Sending Enable PWM mode command
}
 
void PWM_Mode()
{                              
    // a low pull on pin COMP/TRIG  triggering a sensor reading
    digitalWrite(URTRIG, LOW);    
    digitalWrite(URTRIG, HIGH);               
     
    unsigned long DistanceMeasured=pulseIn(URPWM,LOW); // reading Pin PWM will output pulses
     
    if(DistanceMeasured==50000)
    {              
      // the reading is invalid.
      //Serial.print("Invalid");    
    }
    
   else
   {
      Distance=DistanceMeasured/50;           // every 50us low level stands for 1cm
   }
  
   if((Distance > RangeLow) && (Distance < RangeHigh))
      {
         //Blink the LED, for glowing eyes
         digitalWrite(led, HIGH);
         
         //Send 1 to processing, so that processing starts playing music of bat's screeching sound
         Serial.println(1);
         
         //Rotate the servos to create the flapping arms motion
         for(int position = 30; position < 150; position += 2)
          {
            servo1.write(position);  // Move servo1 to next position
            servo2.write(position);  // Move servo2 to next position
            delay(20);               // Short pause to allow it to move
          }
      }
      else
      {
         //Send signal to processing to stop playing the music
         Serial.println(0);
         
         //Turn off the LED
         digitalWrite(led, LOW); 
      }
}
