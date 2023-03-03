int columnList[8] = {0, 1, 2, 3, 4, 5, 6, 7};
int redRowList[8] = {22, 24, 26, 28, 30, 32, 34, 36};
int grnRowList[8] = {23, 25, 27, 29, 31, 33, 35, 37};

int printable[8][8] = {
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,1,1,1,1,1,1},
    {1,0,1,1,1,1,0,1},
    {1,1,0,0,0,0,1,1},
    {1,1,1,1,1,1,1,1}
    };

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 8; i++) {
    pinMode(columnList[i], OUTPUT);
    pinMode(redRowList[i], OUTPUT);
    pinMode(grnRowList[i], OUTPUT);
//    digitalWrite(redRowList[i], HIGH);
//    digitalWrite(grnRowList[i], LOW);
//    digitalWrite(columnList[i], HIGH);
  }
  
//  for (int i = 0; i < 8; i++) {
//    for (int j = 0; j < 8; j++) {
//      printable[i][j] = 2;
//    }
//  }
}

void loop() {
  // put your main code here, to run repeatedly:
  PrintToScreen(printable);
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
                digitalWrite(redRowList[i], HIGH);
                digitalWrite(grnRowList[i], HIGH);
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
