/*
 * player control testing
 */
#include "ICM_20948.h"
#define AD0_VAL 1
#define SERIAL_PORT Serial
#define WIRE_PORT Wire
ICM_20948_I2C myICM;

float x_pos, y_pos;
int x_int_pos, y_int_pos;
float x_speed, y_speed;
int x_offset, y_offset, z_offset;
float sens = 0.00005;
float speedLim = 0.1;

//pins
int button = 8;

void setup() {
  // put your setup code here, to run once:
  SERIAL_PORT.begin(115200);
  while (!SERIAL_PORT);
  WIRE_PORT.begin();
  WIRE_PORT.setClock(400000);

  bool initialized = false;
  while (!initialized)
  {
    myICM.begin(WIRE_PORT, AD0_VAL);
    SERIAL_PORT.print(F("Initialization of the sensor returned: "));
    SERIAL_PORT.println(myICM.statusString());
    if (myICM.status != ICM_20948_Stat_Ok)
    {
      SERIAL_PORT.println("Trying again...");
      delay(500);
    }
    else
    {
      initialized = true;
    }
  }
  calibrate();

  pinMode(button, INPUT);

  x_pos = 4;
  y_pos = 4;
  x_int_pos = 4;
  y_int_pos = 4;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (myICM.dataReady())
  {
    if(digitalRead(button) == HIGH){
      calibrate();
    }

    float imu[3];
    getIMUData(imu);
    movePlayer(imu);
                             
    delay(30);
  }
//  else
//  {
//    SERIAL_PORT.println("Waiting for data");
//    delay(500);
//  }
}

float *getIMUData(float imu[]){
  myICM.getAGMT();
  imu[0] = myICM.gyrX();
  imu[1] = myICM.gyrY();
  imu[2] = myICM.gyrZ();
  
//  SERIAL_PORT.print("x: ");
//  SERIAL_PORT.print(imu[0] - x_offset);
//  SERIAL_PORT.print(", y: ");
//  SERIAL_PORT.print(imu[1] - y_offset);
//  SERIAL_PORT.print(", z: ");
//  SERIAL_PORT.print(imu[2] - z_offset);
//  Serial.print("\n");

  return imu;
  
}

void movePlayer(float imu[]){
  //add acceleration to speed
  x_speed -= (imu[0] * sens);
  y_speed -= (imu[1] * sens);

  //speed limit
  if(x_speed > speedLim){
    x_speed = speedLim;
  }
  else if(x_speed < -speedLim){
    x_speed = -speedLim;
  }
  if(y_speed > speedLim){
    y_speed = speedLim;
  }
  else if(y_speed < -speedLim){
    y_speed = -speedLim;
  }

  
  //Add to position only if ship is not at edge
  //x control
  if(((x_pos + x_speed > 7) || (x_pos + x_speed) < 0)){
    if(x_pos + x_speed > 7){
      x_pos = 7;
    }
    else{
      x_pos = 0;
    }
  }
  else{
    x_pos += x_speed;
  }

  //y control
  if(((y_pos + y_speed > 7) || (y_pos + y_speed) < 0)){
    if(y_pos + y_speed > 7){
      y_pos = 7;
    }
    else{
      y_pos = 0;
    }
  }
  else{
    y_pos += y_speed;
  }

  x_int_pos = x_pos;
  y_int_pos = y_pos;
  //print position
  Serial.print("x_pos: ");
  Serial.print(x_int_pos);
  Serial.print(", y_pos: ");
  Serial.print(y_int_pos);
  Serial.print(", x_speed: ");
  Serial.print(x_speed);
  Serial.print(", y_speed: ");
  Serial.println(y_speed);
}

void calibrate(){
  Serial.println("calibrating");
  while(!myICM.dataReady());
  myICM.getAGMT();
  x_offset = myICM.gyrX();
  y_offset = myICM.gyrY();
  z_offset = myICM.gyrZ();
}
