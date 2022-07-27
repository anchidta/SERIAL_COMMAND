#include "motor_set.h"
//step = 0.46875
//16 step = 7.5 degree
#include <Wire.h>
#include <INA226_WE.h>
#include <SimpleKalmanFilter.h>
//DIR = high CLOCKWISE 1 pull
//DIR = LOW COUTERCLOCKWISE 0 push
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
    pinMode(step_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT); 
    pinMode(en_pin, OUTPUT); 
    pinMode(sleep_pin, OUTPUT); 
    pinMode(reset_pin, OUTPUT); 
    // Serial.begin(9600);
    // Wire.begin();
    ina226.init();
    digitalWrite(en, HIGH);
    digitalWrite(dir, HIGH);
    digitalWrite(sleep, HIGH);
    delay(1);
    Relaese_reset();
}

Motor::Motor(){}

void Motor::Enable()
{
    digitalWrite(en, LOW);//active low
    digitalWrite(sleep, HIGH);//active low
    digitalWrite(reset, HIGH);//active low
}

void Motor::Disable()
{
    digitalWrite(en, HIGH);
    digitalWrite(sleep, LOW);
}

void Motor::Relaese_reset()
{
    digitalWrite(step, LOW);
    digitalWrite(reset, HIGH);
    delay(1);
    digitalWrite(step, HIGH);
    delay(1);
    digitalWrite(step, LOW);
}

void Motor::Init(int step_pin,int dir_pin,int en_pin,int sleep_pin,int reset_pin,int topButtom,int bottomButtom)
{
    // Serial.println("INIT");
    step = step_pin;
    dir = dir_pin;
    en = en_pin;
    sleep =sleep_pin;
    reset =reset_pin;
    top_switch = topButtom;
    buttom_switch= bottomButtom;
    pinMode(step, OUTPUT); 
    pinMode(dir, OUTPUT); 
    pinMode(en, OUTPUT); 
    pinMode(sleep, OUTPUT); 
    pinMode(reset, OUTPUT); 
    pinMode(top_switch, INPUT_PULLUP);
    pinMode(buttom_switch, INPUT_PULLUP);
    digitalWrite(step, LOW);
    digitalWrite(dir,HIGH);
    delayMicroseconds(1);
    int status;
    int switch_state;
    // Serial.println("init_pump");
    Enable();
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
      Disable();
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
    pinMode(step_pin, OUTPUT); 
    pinMode(dir_pin, OUTPUT);
    pinMode(en_pin, OUTPUT); 
    pinMode(sleep_pin, OUTPUT); 
    pinMode(reset_pin, OUTPUT); 
    ina226.init();
    digitalWrite(en, HIGH);
    // digitalWrite(reset, LOW);
    digitalWrite(dir, HIGH);
    digitalWrite(sleep, HIGH);
    delay(1);
    Relaese_reset();
  }

  void Motor::Init(int step_pin,int dir_pin,int en_pin,int sleep_pin,int reset_pin)
  {
    // Serial.println("INIT");
    step = step_pin;
    dir = dir_pin;
    en = en_pin;
    sleep =sleep_pin;
    reset =reset_pin;
    pinMode(step_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT); 
    pinMode(en_pin, OUTPUT); 
    pinMode(sleep_pin, OUTPUT); 
    pinMode(reset_pin, OUTPUT); 
    digitalWrite(en, HIGH);
    // digitalWrite(reset, LOW);
    digitalWrite(dir, HIGH);
    digitalWrite(sleep, HIGH);
    delay(1);
    Relaese_reset();
}

void Motor::Sleep_Disable()
{
    digitalWrite(sleep, HIGH);
}

void Motor::Sleep_Enable()
{
    digitalWrite(sleep, LOW);
}

void Motor::Reset_Disable()
{
    digitalWrite(reset, HIGH);
}

void Motor::Reset_Enable()
{
    digitalWrite(reset, LOW);
}

//DIR = high CLOCKWISE 1 pull
//DIR = LOW COUTERCLOCKWISE 0 push

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
        }
        digitalWrite(2, HIGH);
        delayMicroseconds(1);
}

bool Motor::Signal_send(int dir_need, int step_check ,int motor_id, int current_th)
{  
    float current_mA = 0.0;
     digitalWrite(dir,dir_need);

      for(int x = 0; x < step_check; x++)
        {
          digitalWrite(step, HIGH);
          delayMicroseconds(delay_num_valve);
          digitalWrite(step, LOW);
          delayMicroseconds(delay_num_valve);//150
          count += 1;
          // Print(step_count);

          if (millis()- previousMillis >= 50) {
            previousMillis = millis();

          // unsigned lEnableg start = millis();
            current_mA = ina226.getCurrent_mA();
            estimated_value = simpleKalmanFilter.updateEstimate(current_mA);

            if(estimated_value >= current_th && dir_need == 0){
              num_step = count;
              return true;

            }
        // unsigned lEnableg stop = millis();
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
      Move(1,num_step,150);
}
