
/*
TODO
- Implement player movement
- Implement way to keep track of score

OPTIONAL
- Implement collision detectin between asteroids
*/

#include "stdlib.h"
#include "unistd.h"
//#include "ctime"
#include "time.h"

using namespace std;

//player control
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
//end player control variables

//printScreen variables
int columnList[8] = {0, 1, 2, 3, 4, 5, 6, 7};
int redRowList[8] = {22, 24, 26, 28, 30, 32, 34, 36};
int grnRowList[8] = {23, 25, 27, 29, 31, 33, 35, 37};

int printable[8][8] = {
    {0,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,1,1,1,1,1,1},
    {1,0,1,1,1,1,0,1},
    {1,1,0,0,0,0,1,1},
    {0,1,1,1,1,1,1,0}
    };
//end print screen variables

//Object used to manage the asteroids position and direction
class Asteroid {
    public:
        int x;
        int y;
        int startTime;

        bool isBlinking;
        bool isActive;

        Asteroid () {
            x = 0;
            y = 0;
            startTime = 0;
            isBlinking = false;
            isActive = false;
        }
};

//Object that keeps track of the players current position
class Player {
    public:
        int x[5];
        int y[5];
        int lives;

    Player () {
        x[0] = 3;
        x[1] = 3;
        x[2] = 3;
        x[3] = 2;
        x[4] = 4;
        y[0] = 3;
        y[1] = 4;
        y[2] = 5;
        y[3] = 4;
        y[4] = 4;
        lives = 3;
    }
};

//Should work the same as our current print matrix
void PrintMatrix (int matrix[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (matrix[i][j] == 5) {
                Serial.print("O ");
            } else if (matrix[i][j] == 6) {
                Serial.print("X ");
            }else {
                Serial.print("  ");
            }
        }
        Serial.print("\n");
    }
}

void setup () {

  //player control setup
  ERIAL_PORT.begin(115200);
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
  //end player control setup

  //printScreen setup
  for (int i = 0; i < 8; i++) {
  pinMode(columnList[i], OUTPUT);
  pinMode(redRowList[i], OUTPUT);
  pinMode(grnRowList[i], OUTPUT);
  //end print screen setup

    int matrix[8][8];
    int maxAsteroids = 2;
    int isGameAlive = true;

    Asteroid objectList[maxAsteroids];
    Player playerOne;

    int gameStart = time(NULL);
    srand(time(NULL));

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            matrix[i][j] = 0;
        }
    }

    //Initialize first asteroid
    objectList[0].x = rand() % 6 + 1;;
    objectList[0].y = rand() % 6 + 1;;
    objectList[0].startTime = time(NULL);
    objectList[0].isActive = true;

    int spawnTimer = time(NULL);

    while(isGameAlive) {
        PrintMatrix(matrix);

        //Spawns asteroid every 5 seconds if there is an asteroid to spawn
        //Change max asteroids to enable for asteroids to spawn at one time
        if ((time(NULL) - spawnTimer) > 5) {
            for (int i = 0; i < maxAsteroids; i++) {
                if (objectList[i].isActive == false) {
                    objectList[i].x = rand() % 6 + 1;
                    objectList[i].y = rand() % 6 + 1;
                    objectList[i].startTime = time(NULL);
                    objectList[i].isActive = true;

                    spawnTimer = time(NULL);
                    break;
                }
            }
        }

        //Prints out the asteroids
        for (int i = 0; i < maxAsteroids; i++) {

            if (objectList[i].isActive == true) {
                if ((time(NULL) - objectList[i].startTime) < 10 && objectList[i].isBlinking == false) {
                    //Tells print function to print the asteroids as 0
                    //IMPORTANT Just set to either 1 or 2 to work with our print screen function
                    matrix[objectList[i].x][objectList[i].y] = 5;
                    matrix[objectList[i].x+1][objectList[i].y+1] = 5;
                    matrix[objectList[i].x][objectList[i].y+1] = 5;
                    matrix[objectList[i].x-1][objectList[i].y+1] = 5;
                    matrix[objectList[i].x-1][objectList[i].y] = 5;
                    matrix[objectList[i].x-1][objectList[i].y-1] = 5;
                    matrix[objectList[i].x][objectList[i].y-1] = 5;
                    matrix[objectList[i].x+1][objectList[i].y-1] = 5;
                    matrix[objectList[i].x+1][objectList[i].y] = 5;
                    if ((time(NULL) - objectList[i].startTime) < 5) {
                        objectList[i].isBlinking = true;
                    }
                }
                else {
                    matrix[objectList[i].x][objectList[i].y] = 0;
                    matrix[objectList[i].x+1][objectList[i].y+1] = 0;
                    matrix[objectList[i].x][objectList[i].y+1] = 0;
                    matrix[objectList[i].x-1][objectList[i].y+1] = 0;
                    matrix[objectList[i].x-1][objectList[i].y] = 0;
                    matrix[objectList[i].x-1][objectList[i].y-1] = 0;
                    matrix[objectList[i].x][objectList[i].y-1] = 0;
                    matrix[objectList[i].x+1][objectList[i].y-1] = 0;
                    matrix[objectList[i].x+1][objectList[i].y] = 0;
                    objectList[i].isBlinking = false;

                    if ((time(NULL) - objectList[i].startTime) > 10) {
                        objectList[i].isActive = false;
                    }
                }
            }
        }

        //Implement player input here
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
        

        for (int i = 0; i < 5; i++) {
            //Player is colliding with an asteroid
            if (matrix[playerOne.x[i]][playerOne.y[i]] == 5) {

                //Destroy asteroid that collided with player
                for (int j = 0; j < maxAsteroids; j++) {
                    for (int k = 0; k < 5; k++) {
                        if ((objectList[j].x == playerOne.x[k] || objectList[j].x+1 == playerOne.x[k] || objectList[j].x-1 == playerOne.x[k]) && (objectList[j].y == playerOne.y[k] || objectList[j].y+1 == playerOne.y[k] || objectList[j].y-1 == playerOne.y[k])) {
                            //Found which asteroid the player has collided with or at least an asteroid
                            if ((time(NULL) - objectList[j].startTime) > 5 && (time(NULL) - objectList[j].startTime) < 10) {
                                //The asteroid found can damage the player
                                playerOne.lives--;
                                objectList[j].isActive = false;

                                //Make sure all reminents of the destroyed asteroid dont show
                                matrix[objectList[j].x][objectList[j].y] = 0;
                                matrix[objectList[j].x+1][objectList[j].y+1] = 0;
                                matrix[objectList[j].x][objectList[j].y+1] = 0;
                                matrix[objectList[j].x-1][objectList[j].y+1] = 0;
                                matrix[objectList[j].x-1][objectList[j].y] = 0;
                                matrix[objectList[j].x-1][objectList[j].y-1] = 0;
                                matrix[objectList[j].x][objectList[j].y-1] = 0;
                                matrix[objectList[j].x+1][objectList[j].y-1] = 0;
                                matrix[objectList[j].x+1][objectList[j].y] = 0;

                                j = 100;
                                break;
                            }
                        }
                    }
                }

                //Check for gameover
                if (playerOne.lives <= 0) {
                    isGameAlive = false;
                }
            }
            //Tells print function to print the player as +
            //IMPORTANT Just set to either 1 or 2 to work with our print screen function
            matrix[playerOne.x[i]][playerOne.y[i]] = 6;
        }

        Serial.println(playerOne.lives);
        //IMPORTANT might have to adjust the following to keep print from flickering and such
        delay(1);
        //IMPORTANT remove the following live before trying to use with arduino
        system("clear");
    }

    //cout << "Final Score: " + (time(NULL) - gameStart) + endl << "GAME OVER" << endl;
    Serial.print("Final Score: ");
    Serial.println((time(NULL) - gameStart));
    Serial.println("GAME OVER");


    return(0);
}

void loop(){
  
}

void PrintToScreen (int printMatrix[8][8]) {
    for (int i = 0; i < 8; i++) {
      //            digitalWrite(columnList[j], LOW);
        for (int j = 0; j < 8; j++) {

            //NOTE going to have to experiment with what happens when the row values are LOW and the COLUMN values are high
            //Determines which row will be lighting up
            if (printMatrix[i][j] == 1) {
                digitalWrite(grnRowList[i], HIGH);
                digitalWrite(redRowList[i], LOW);
                digitalWrite(columnList[j], HIGH);
            }
            else if (printMatrix[i][j] == 2) {
                digitalWrite(redRowList[i], HIGH);
                digitalWrite(grnRowList[i], LOW);
                digitalWrite(columnList[j], HIGH);
            }
            else {
                digitalWrite(columnList[j], LOW);                
            }
            //Wherever a HIGH row and a LOW column meet is where the light will light up
//            digitalWrite(columnList[j], LOW);
          
        }
        delay(100/30);
        //Sets back to values where nothing should light up when a row value is changed;
        digitalWrite(redRowList[i], HIGH);
        digitalWrite(grnRowList[i], HIGH);
//        digitalWrite(columnList[i], LOW);
    }
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
