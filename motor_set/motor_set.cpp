#include "motor_set.h"
//step = 0.46875
//16 step = 7.5 degree
#include <Wire.h>
#include <INA226_WE.h>
#include <SimpleKalmanFilter.h>

SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);

Motor::Motor(int step_pin,int dir_pin,int en_pin,int sleep_pin,int reset_pin,unsigned char address_id)
{


  int step_count;
  step = step_pin;
  dir = dir_pin;
  en = en_pin;
  sleep =sleep_pin;
  reset =reset_pin;
  address = address_id;
  ina226 = INA226_WE(address_id);
  pinMode(step_pin, OUTPUT); // set LED  pin as OUTPUT
  pinMode(dir_pin, OUTPUT); // set LED  pin as OUTPUT
  pinMode(en_pin, OUTPUT); // set LED  pin as OUTPUT
  pinMode(sleep_pin, OUTPUT); // set LED  pin as OUTPUT
  pinMode(reset_pin, OUTPUT); // set LED  pin as OUTPUT

  // Serial.begin(9600);
  // Wire.begin();
  ina226.init();
  digitalWrite(en, HIGH);
  // digitalWrite(reset, LOW);
  digitalWrite(dir, HIGH);
  digitalWrite(sleep, HIGH);
  delay(1);
  relaese_reset();

}

Motor::Motor(){}

void Motor::On()
{

digitalWrite(en, LOW);//active low
digitalWrite(sleep, HIGH);//active low
digitalWrite(reset, HIGH);//active low

}

void Motor::Off()
{

  digitalWrite(en, HIGH);
  digitalWrite(sleep, LOW);
  // digitalWrite(reset, LOW);

}
void Motor::relaese_reset()
{
    digitalWrite(step, LOW);
    digitalWrite(reset, HIGH);
    delay(1);
    digitalWrite(step, HIGH);
    delay(1);
    digitalWrite(step, LOW);


}
void Motor::Init(int step_pin,int dir_pin,int en_pin,int sleep_pin,int reset_pin,int topButton,int bottomButton)
{
  // Serial.println("INIT");
  step = step_pin;
  dir = dir_pin;
  en = en_pin;
  sleep =sleep_pin;
  reset =reset_pin;
  top_switch = topButton;
  buttom_switch= bottomButton;

  pinMode(step, OUTPUT); // set LED  pin as OUTPUT
  pinMode(dir, OUTPUT); // set LED  pin as OUTPUT
  pinMode(en, OUTPUT); // set LED  pin as OUTPUT
  pinMode(sleep, OUTPUT); // set LED  pin as OUTPUT
  pinMode(reset, OUTPUT); // set LED  pin as OUTPUT
  pinMode(top_switch, INPUT_PULLUP);
  pinMode(buttom_switch, INPUT_PULLUP);

  

  digitalWrite(step, LOW);
  digitalWrite(dir,HIGH);
  delayMicroseconds(1);
  int status;
  int switch_state;
  // Serial.println("init_pump");
  On();
  int Istop = digitalRead(top_switch);
  int Isbottom = digitalRead(buttom_switch);


  if(Istop == HIGH && Isbottom == HIGH)
  { 
    switch_state = 1;
    digitalWrite(dir,HIGH);
  }
  else if (Istop == LOW && Isbottom == HIGH)
  {
    switch_state = 2;
    digitalWrite(dir,HIGH);
  }
  else if (Istop == HIGH && Isbottom == LOW)
  {
    switch_state = 3;
    digitalWrite(dir,LOW);
  }
  
  while (1)
    {
      for(int x = 0; x < 300; x++)
      {
      digitalWrite(step, HIGH);
      delayMicroseconds(75);
      digitalWrite(step, LOW);
      delayMicroseconds(75);
      }
      int Istop = digitalRead(top_switch);
      int Isbottom = digitalRead(buttom_switch);
    
      if(Istop == LOW)
      {
        status = 1;
        // Serial.println("top");
        digitalWrite(dir,HIGH);
        break;
      }
      if(Isbottom == LOW)
      {
        status = 0;
        // Serial.println("buttom");
         digitalWrite(dir,LOW);
        break;
      }
      
    }
  
    int count = 0;
    while (1)
    {
      for(int x = 0; x < 300; x++)
      {
      digitalWrite(step, HIGH);
      delayMicroseconds(75);
      digitalWrite(step, LOW);
      delayMicroseconds(75);
      }
      count += 1;
      int Istop = digitalRead(top_switch);
      int Isbottom = digitalRead(buttom_switch);

      if(count > 100)
      {
      if(Istop == LOW)
      {
        status = 1;
        // Serial.println("top");
        digitalWrite(dir,HIGH);
        break;
      }
      if(Isbottom == LOW)
      {
        status = 0;
        // Serial.println("buttom");
          digitalWrite(dir,LOW);
        break;
      }
      // }
    }
    }
    count = 0;
    // Serial.println("setup");
    for(int x = 0; x < 23903; x++)
    {
      
      digitalWrite(step, HIGH);
      delayMicroseconds(75);
      digitalWrite(step, LOW);
      delayMicroseconds(75);

      
    }
    Off();
    

}



void Motor::Init(int step_pin,int dir_pin,int en_pin,int sleep_pin,int reset_pin,unsigned char address_id)
{
  // Serial.println("INIT");
  step = step_pin;
  dir = dir_pin;
  en = en_pin;
  sleep =sleep_pin;
  reset =reset_pin;
  address = address_id;
  ina226 = INA226_WE(address_id);
  pinMode(step_pin, OUTPUT); // set LED  pin as OUTPUT
  pinMode(dir_pin, OUTPUT); // set LED  pin as OUTPUT
  pinMode(en_pin, OUTPUT); // set LED  pin as OUTPUT
  pinMode(sleep_pin, OUTPUT); // set LED  pin as OUTPUT
  pinMode(reset_pin, OUTPUT); // set LED  pin as OUTPUT

  ina226.init();
  digitalWrite(en, HIGH);
  // digitalWrite(reset, LOW);
  digitalWrite(dir, HIGH);
  digitalWrite(sleep, HIGH);
  delay(1);
  relaese_reset();
}

void Motor::Init(int step_pin,int dir_pin,int en_pin,int sleep_pin,int reset_pin)
{
  // Serial.println("INIT");
  step = step_pin;
  dir = dir_pin;
  en = en_pin;
  sleep =sleep_pin;
  reset =reset_pin;

  pinMode(step_pin, OUTPUT); // set LED  pin as OUTPUT
  pinMode(dir_pin, OUTPUT); // set LED  pin as OUTPUT
  pinMode(en_pin, OUTPUT); // set LED  pin as OUTPUT
  pinMode(sleep_pin, OUTPUT); // set LED  pin as OUTPUT
  pinMode(reset_pin, OUTPUT); // set LED  pin as OUTPUT


  digitalWrite(en, HIGH);
  // digitalWrite(reset, LOW);
  digitalWrite(dir, HIGH);
  digitalWrite(sleep, HIGH);
  delay(1);
  relaese_reset();
}
void Motor::Sleep_Off()
{
digitalWrite(sleep, HIGH);
}

void Motor::Sleep_On()
{
digitalWrite(sleep, LOW);
}

void Motor::Reset_Off()
{
digitalWrite(reset, HIGH);
}

void Motor::Reset_On()
{
digitalWrite(reset, LOW);
}


void Motor::go_front(int mm)
{
  bool status = false;
  int degree = (mm)*180;
  int stepsPerRevolution = degree/0.46875;
  // Set motor direction clockwise
  
  digitalWrite(dir,LOW);
  while(status != true){
  for(int x = 0; x < stepsPerRevolution; x++)
    {
      digitalWrite(step, HIGH);
      delayMicroseconds(1000);
      digitalWrite(step, LOW);
      delayMicroseconds(1000);
    }
    delay(1000); 
    status = true;
    }

}

void Motor::go_back(int mm)
{
  bool status = false;
  int degree = (mm)*180;
  int stepsPerRevolution = degree/0.46875;
  // Set motor direction clockwise
  
  digitalWrite(dir,HIGH);
  while(status != true){
  for(int x = 0; x < stepsPerRevolution; x++)
    {
      digitalWrite(step, HIGH);
      delayMicroseconds(1000);
      digitalWrite(step, LOW);
      delayMicroseconds(1000);
    }
    delay(1000);
  status = true; 
  }
}

void Motor::Move2degree(int degree,int count)
{
  
  int time = 0;
  int stepsPerRevolution = degree;//0.46875;
  // Set motor direction clockwise
  
  digitalWrite(dir,HIGH);
  while(time < count){ 
    for(int x = 0; x < stepsPerRevolution; x++)
      {
        digitalWrite(step, HIGH);
        delayMicroseconds(1000);
        digitalWrite(step, LOW);
        delayMicroseconds(1000);
      }
      delay(1000); // Wait a second
      
      // Set motor direction counterclockwise
      digitalWrite(dir, LOW);

    // Spin motor quickly-
    for(int x = 0; x < stepsPerRevolution; x++)
    {
      digitalWrite(step, HIGH);
      delayMicroseconds(1000);
      digitalWrite(step, LOW);
      delayMicroseconds(1000);
    }
    delay(1000); // Wait a second
  count = count + 1;

}
}
//DIR = high CLOCKWISE 1
//dir = LOW COUTERCLOCKWISE 0 
void Motor::Move(uint8_t dir_need,int step_check,int delay_ms)
{
  digitalWrite(step, LOW);
  digitalWrite(dir,dir_need);
  delayMicroseconds(1);
  // Serial.println("START MOVE");
  float current_mA = 0.0;
    for(int x = 0; x < step_check; x++)
      {
        digitalWrite(step, HIGH);
        delayMicroseconds(delay_ms);
        digitalWrite(step, LOW);
        delayMicroseconds(delay_ms);
      
        // 


        // delayMicroseconds(1);
        // Print(step_count);
      }
      digitalWrite(2, HIGH);
      delayMicroseconds(1);
      // delay(1000); 
      // step_count = 0;
      // Print("move complete");// Wait a second


     
      // Set motor direction counterclockwis

}
// void Motor::Movetogeter(Motor motor1,int dir_need ,int step_check)
// {


// digitalWrite(motor1.dir,dir_need);
// digitalWrite(dir,dir_need);
// for(int x = 0; x < step_check; x++)
// {
//   motor1.signal_send(1);
//   for(int x = 0; x < 1; x++)
//       {
//         digitalWrite(step, HIGH);
//         delayMicroseconds(1000);
//         digitalWrite(step, LOW);
//         delayMicroseconds(1000);
//         // step_count += 1;
//         // Print(step_count);
//       }
// }
// }

// void Motor::get_current()
bool Motor::signal_send(int dir_need, int step_check ,int motor_id, int current_th)
{  float current_mA = 0.0;
   digitalWrite(dir,dir_need);
   
    for(int x = 0; x < step_check; x++)
      {
        digitalWrite(step, HIGH);
        delayMicroseconds(100);
        digitalWrite(step, LOW);
        delayMicroseconds(100);//150
        count += 1;
        // Print(step_count);

        if (millis()- previousMillis >= 50) {
          previousMillis = millis();
  
        // unsigned long start = millis();
          current_mA = ina226.getCurrent_mA();
          estimated_value = simpleKalmanFilter.updateEstimate(current_mA);
          // Serial.print(current_mA);
          
          
          // Serial.print(motor_id);
          
          // Serial.print(",");
          // Serial.println(estimated_value);
          if(estimated_value >= current_th && dir_need == 0){
            num_step = count;
            return true;

          }
      // unsigned long stop = millis();
      // Serial.print(",");
      // Serial.println(stop-start);
      }
      return false;
  }
}

bool Motor::Close()
{
  Move(0,num_step,150);


}
bool Motor::Open()
{ 
  Move(1,6000,150);
}