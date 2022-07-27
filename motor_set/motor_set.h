#ifndef MOTOR_SET_H
#define MOTOR_SET_H

#include <Arduino.h>
#include <Wire.h>
#include <INA226_WE.h>
class Motor {
  
  public:
    int step_count = 0;
    int step;
    int dir;
    int en;//active low
    int sleep;//active low
    int reset;//active low ,step inprt will ignored,pull sleep & reset high to enable pin
    unsigned char address;
    INA226_WE ina226;
    unsigned long previousMillis;
    float estimated_value;
    int motor_id;
    int top_switch;
    int buttom_switch;
    int num_step = 6000;
    int count = 0;
    int delay_num_valve = 100;
    // int nema17 = 3200;
    // int motor_test = 768;


    Motor(int step_pin, int dir_pin, int en_pin,int sleep_pin,int reset_pin, unsigned char address_id);
    // Motor(int interface = Motor::NEMA17,int step_pin, int dir_pin, int en_pin);
    Motor();
    void Init(int step_pin, int dir_pin, int en_pin,int sleep_pin,int reset_pin,unsigned char address_id);//init motor with current sensor
    void Init(int step_pin, int dir_pin, int en_pin,int sleep_pin,int reset_pin,int topButton,int bottomButton);//init pump
    void Init(int step_pin, int dir_pin, int en_pin,int sleep_pin,int reset_pin);//setup pin only
    // void Move2degree(int position, int count);
    void Move(uint8_t dir_need, int step_check,int delay_ms);
    void Disable();
    void Enable();
    void Relaese_reset();
    void Sleep_Enable();
    void Sleep_Disable();
    void Reset_Enable();
    void Reset_Disable();
    // void go_back(int mm);
    // void go_front(int mm);
    bool Close();
    bool Open();
    // void Movetogeter(Motor motor1,int dir_need ,int step_check);
    bool Signal_send(int dir_need,int step_check,int motor_id,int current_th);
    // void 2Move2degree(Motor motor1, Motor motor2, int posit);
    
};

#endif
