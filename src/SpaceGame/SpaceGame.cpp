/*
TODO
- Implement player movement
- Implement way to keep track of score

OPTIONAL
- Implement collision detectin between asteroids
*/

#include <iostream>
#include <stdlib.h>
#include<unistd.h>
#include<ctime>
#include <time.h>

using namespace std;


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
                cout << "O ";
            } else if (matrix[i][j] == 6) {
                cout << "X ";
            }else {
                cout << "  ";
            }
        }
        cout << endl;
    }
}

int main () {
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

        cout << playerOne.lives << endl;
        //IMPORTANT might have to adjust the following to keep print from flickering and such
        sleep(1);
        //IMPORTANT remove the following live before trying to use with arduino
        system("clear");
    }

    cout << "Final Score: " + (time(NULL) - gameStart) + endl << "GAME OVER" << endl;
    return(0);
}