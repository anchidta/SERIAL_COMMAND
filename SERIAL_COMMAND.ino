#include <motor_set.h>
//#include<control.h>
#include <Wire.h>
#include <INA226_WE.h>


#define I2C_ADDRESS_MOTOR1 0x40
#define I2C_ADDRESS_MOTOR2 0x44
#define I2C_ADDRESS_MOTOR3 0x41
#define I2C_ADDRESS_MOTOR4 0x45

//void Motor::Init(int step_pin,int dir_pin,int en_pin,int sleep_pin,int reset_pin)
//   motor_1.Init(A4, 44, 38, 42, 40, I2C_ADDRESS_MOTOR1); /
//   motor_2.Init(A3,36,30,34,32, I2C_ADDRESS_MOTOR2);/
//   motor_3.Init(A2,28,22,26,24, I2C_ADDRESS_MOTOR3);/
//   motor_4.Init(A1,29,35,31,33, I2C_ADDRESS_MOTOR4);/
//   digitalWrite(PWM, HIGH);
//   pump.Init(A0,37,43,39,41);//1

// void Motor::Init(int step_pin,int dir_pin,int en_pin,int sleep_pin,int reset_pin)
//   motor_1.Init(pin_step_MOTOR1, pin_DIR_MOTOR1, pin_EN_MOTOR1, pin_SLP_MOTOR1, pin_RST_MOTOR1, I2C_ADDRESS_MOTOR1); /

#define pin_step_MOTOR1 A4
#define pin_DIR_MOTOR1 44
#define pin_EN_MOTOR1 38
#define pin_SLP_MOTOR1 42
#define pin_RST_MOTOR1 40

#define pin_step_MOTOR2 A3
#define pin_DIR_MOTOR2 36
#define pin_EN_MOTOR2 30
#define pin_SLP_MOTOR2 34
#define pin_RST_MOTOR2 32

#define pin_step_MOTOR3 A2
#define pin_DIR_MOTOR3 28
#define pin_EN_MOTOR3 22
#define pin_SLP_MOTOR3 26
#define pin_RST_MOTOR3 24

#define pin_step_MOTOR4 A1
#define pin_DIR_MOTOR4 29
#define pin_EN_MOTOR4 35
#define pin_SLP_MOTOR4 31
#define pin_RST_MOTOR4 33

#define pin_step_PUMP A0
#define pin_DIR_PUMP 37
#define pin_EN_PUMP 43
#define pin_SLP_PUMP 39
#define pin_RST_PUMP 41

#define pin_top_switch 51
#define pin_buttom_switch 52

int PWM = A5;//MOSFET SEND SIGNAL PIN
int MS1 = 50;
int MS2 = 48;
int MS3 = 46;

//parameter
#define current_limit 170//170mA
#define Step_pull_up_down 6000
#define Step_close_motor 10000

String inputString= "";  // a String to hold incoming data
bool stringComplete = false; 
int loop_time = 1000;
int state_command = 0;
//call motor class
Motor motor_1;
Motor motor_2;
Motor motor_3;
Motor motor_4;
Motor pump;

bool Open_all()
{
   motor_1.On();
   motor_2.On();
   motor_3.On();
   motor_4.On();
   pump.On();
   move_together(motor_1,motor_2,motor_3,motor_4,HIGH,Step_pull_up_down,current_limit);
   motor_1.Off();
   motor_2.Off();
   motor_3.Off();
   motor_4.Off();
   pump.Off();

}

int delay_ms = 150;
int step_for_pump_move = 14000;

bool Vacumn(Motor pump)
{
   pump.Move(HIGH,step_for_pump_move,delay_ms);
}

bool Pressure(Motor pump)
{
   pump.Move(LOW,step_for_pump_move,delay_ms);
}

bool Release_Ps()//Solinoid(H to L)
{
   delay(1);
   digitalWrite(PWM, HIGH);
   delay(500);
   digitalWrite(PWM, LOW);
}

bool Priming()
{
   motor_3.On();
   motor_4.On();
   pump.On();

   motor_3.Open();
   Vacumn(pump);
   motor_3.Close();
   Release_Ps();

   motor_4.Open();
   Pressure(pump);
   motor_4.Close();
   Release_Ps();

   motor_3.Off();
   motor_4.Off();
   pump.Off();

}
bool Drain()
{
   motor_1.On();
   motor_4.On();
   pump.On();

   motor_1.Open();
   Vacumn(pump);
   motor_1.Close();
   Release_Ps();

   motor_4.Open();
   Pressure(pump);
   motor_4.Close();
   Release_Ps();

   motor_1.Off();
   motor_4.Off();
   pump.Off();
}
bool Fill()
{
   motor_1.On();
   motor_2.On();
   pump.On();

   motor_2.Open();
   Vacumn(pump);
   motor_2.Close();
   Release_Ps();

   motor_1.Open();
   Pressure(pump);
   motor_1.Close();
   Release_Ps();

   motor_1.Off();
   motor_2.Off();
   pump.Off();
}
void Dwell()
{
   motor_1.On();
   motor_2.On();
   motor_3.On();
   motor_4.On();

   move_together(motor_1,motor_2,motor_3,motor_4,LOW,Step_close_motor,current_limit);

   motor_1.Off();
   motor_2.Off();
   motor_3.Off();
   motor_4.Off();
}
void Close_all()
{
   motor_1.On();
   motor_2.On();
   motor_3.On();
   motor_4.On();

   move_together(motor_1,motor_2,motor_3,motor_4,LOW,Step_close_motor,current_limit);

   motor_1.Off();
   motor_2.Off();
   motor_3.Off();
   motor_4.Off();
}
void move_together(Motor motor_1,Motor motor_2,Motor motor3,Motor motor4,int dir_need,int step_check,int current_th)
{
   //if dir_need is LOW == motor pull dowm
   //if dir need is HIGH == motor pull up
  bool status_1 = false;
  bool status_2 = false;
  bool status_3 = false;
  bool status_4 = false;
  int count_time1;
  
  for(int x = 0; x < step_check; x++)
  {
  if (!status_1){

  status_1 = motor_1.signal_send(dir_need, 1,1,current_th);
  if(status_1){Serial1.println("found 1");}
  // Serial1.print(",");
  }
    if (!status_2){
  status_2 = motor_2.signal_send(dir_need, 1,2,current_th);
  if(status_2){Serial1.println("found 2");}
  // Serial1.print(",");
  }
    if (!status_3){
  status_3 =motor3.signal_send(dir_need, 1,3,current_th);
   if(status_3){Serial1.println("found 3");}
  // Serial1.print(",");
  }
    if (!status_4){
  status_4 = motor4.signal_send(dir_need, 1,4,current_th);
   if(status_4){Serial1.println("found 4");}
  }

  }
 
}
void setup_4motor()
{
motor_1.On();
motor_2.On();
motor_3.On();
motor_4.On();

move_together( motor_1, motor_2, motor_3, motor_4,LOW,720,current_limit);//delay for current sensor to init itself
move_together( motor_1, motor_2, motor_3, motor_4,LOW,25000,current_limit);//Close at 25000 step
move_together( motor_1, motor_2, motor_3, motor_4,HIGH,5000,current_limit);//open 5000 step 

motor_1.Off();
motor_2.Off();
motor_3.Off();
motor_4.Off();
}
void setup_4motor_part2()
{

motor_1.On();
motor_2.On();
motor_3.On();
motor_4.On();

move_together( motor_1, motor_2, motor_3, motor_4,LOW,720,current_limit);//delay for current sensor to init itself
move_together( motor_1, motor_2, motor_3, motor_4,LOW,20000,current_limit);//Close at 20000 step

motor_1.Off();
motor_2.Off();
motor_3.Off();
motor_4.Off();


} 
void setup() {

  Serial1.begin(115200);
  inputString.reserve(200);
  Wire.begin();

  pinMode(PWM, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  //set ms to sixttenstep
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, HIGH);
  //setup motor pin
  //void Motor::Init(int step_pin,int dir_pin,int en_pin,int sleep_pin,int reset_pin)
   motor_1.Init(pin_step_MOTOR1, pin_DIR_MOTOR1, pin_EN_MOTOR1, pin_SLP_MOTOR1, pin_RST_MOTOR1, I2C_ADDRESS_MOTOR1); /
   motor_2.Init(pin_step_MOTOR2, pin_DIR_MOTOR2, pin_EN_MOTOR2, pin_SLP_MOTOR2, pin_RST_MOTOR2, I2C_ADDRESS_MOTOR2); /
   motor_3.Init(pin_step_MOTOR3, pin_DIR_MOTOR3, pin_EN_MOTOR3, pin_SLP_MOTOR3, pin_RST_MOTOR3, I2C_ADDRESS_MOTOR3); /
   motor_4.Init(pin_step_MOTOR4, pin_DIR_MOTOR4, pin_EN_MOTOR4, pin_SLP_MOTOR4, pin_RST_MOTOR4, I2C_ADDRESS_MOTOR4); /


   digitalWrite(PWM, HIGH);
   pump.Init(pin_step_PUMP, pin_DIR_PUMP, pin_EN_PUMP, pin_SLP_PUMP, pin_RST_PUMP); /
   delay(100);

}

void serialEvent1() {
  while (Serial1.available())
  {
    char inChar = (char)Serial1.read();
    if (inChar != '\n'  && inChar != '\r') {
      inputString += inChar;
    }
    if (inChar == '\n') {
      Serial1.println(inputString);
      stringComplete = true;
    }
  }
}


// void handleSerial1() {
//  while (Serial1.available() > 0) {
//    char incomingCharacter = Serial1.read();
//    switch (incomingCharacter) {
//       case 'i'://Init value+pump
//          state_command = 1;
//          break;

//       case 't'://Init value+tube
//          state_command = 2;
//          break;

//       case 'p':
//          state_command = 3;

//          break;

//       case 'd':
//          state_command = 4;
//          break;

//       case 'f':
//          state_command = 5;
         
//          break;

//       case 'w':
//          state_command = 6;
         

//          break;

//       case 'o':
//          state_command = 7;
   
//          break;
      
//        case 'c':
//          state_command = 8;
   
//          break;
      


//     }
//  }
// }

void loop() {
   // handleSerial1();
 if (stringComplete)
  {
   //  if (inputString.equals("scan"))
   //  {
   //    // Serial1.println("scan");
   //  }
    if(inputString.equals("A+i"))
      {
         state_command = 1;
    

      }
   else if(inputString.equals("A+t"))
      {
         state_command = 2;
   

      }
   else if(inputString.equals("A+p"))
      {
         state_command = 3;
    

      }
   else if(inputString.equals("A+d"))
      {
         state_command = 4;
         
      }
   else if(inputString.equals("A+f"))
      {
         state_command = 5;
         

      }
   else if(inputString.equals("A+w"))
      {
         state_command = 6;
         

      }
   else if(inputString.equals("A+o"))
      {
         state_command = 7;
         

      }
   else if(inputString.equals("A+c"))
      {
         state_command = 8;
         

      }
   // else
   //    {
   //       break;
   //    }
    stringComplete = false;
    inputString = "";
  }


   switch(state_command)
   {
      case 0:  break;//idie

      case 1:  pump.Init(pin_step_PUMP, pin_DIR_PUMP, pin_EN_PUMP, pin_SLP_PUMP, pin_RST_PUMP, pin_top_switch ,pin_buttom_switch); ///1
               delay(100);
               setup_4motor();

               state_command = 0;
               break;

      case 2:  setup_4motor_part2();
               state_command = 0;
               break;

      case 3:  Priming();
               
               break;

      case 4:  Drain();
               
               break;

      case 5:  Fill();
               
               break;

      case 6:  Dwell();
               state_command = 0;
               break;

      case 7:  Open_all();
               state_command = 0;
               break;

      case 8:  Close_all();
               state_command = 0;
               break;


   default: break;
   
   }
}