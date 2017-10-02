#include "RGB.h"
int RED_PIN = 9;
int GREEN_PIN = 10;
int BLUE_PIN = 11;
// Flash , Delay of 25 works better, lower is too quick
int DELAY = 2;
RGB pumpkin(RED_PIN, GREEN_PIN, BLUE_PIN, DELAY);
void setup()
{
    //Set Red Pin High
    pumpkin.init();
}
void loop()
{
    // Closest Working  Non Flash
    pumpkin.fadeGreenUp(DELAY * 70);
    pumpkin.fadeRedDown(DELAY * 3);
    pumpkin.fadeBlueUp(DELAY * 9.5);
    pumpkin.fadeGreenDown(DELAY * 7.5);
    pumpkin.fadeRedUp(DELAY * 4);
    pumpkin.fadeBlueDown(DELAY * 12);

    // Flash kind sucks
    // pumpkin.fadeGreenUpFlash(DELAY);
    // pumpkin.fadeRedDownFlash(DELAY);
    // pumpkin.fadeBlueUpFlash(DELAY);
    // pumpkin.fadeGreenDownFlash(DELAY);
    // pumpkin.fadeRedUpFlash(DELAY);
    // pumpkin.fadeBlueDownFlash(DELAY);

    // Best SO FAR
    // pumpkin.fadeGreenUp(DELAY * 8);
    // pumpkin.fadeRedDown(DELAY * 2);
    // pumpkin.fadeBlueUp(DELAY * 4);
    // pumpkin.fadeGreenDown(DELAY * 3.5);
    // pumpkin.fadeRedUp(DELAY * 5);
    // pumpkin.fadeBlueDown(DELAY * 3);
}