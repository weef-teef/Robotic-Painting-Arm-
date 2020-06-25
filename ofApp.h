#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    
    //Math............
    float theta1;//base centered -> hand position (test = 90)
    float theta2;//Base to l1 (lower arm)
    float theta3;//l1 to l2(lower arm to upper arm)
    float theta4;//l1 to first joint
    float theta5;//l2 to first joint
    
    const float l1 = 24;//l1: lower arm
    const float l2 = 32; // l2:upper arm, l3:length of brush
    const float adjForBrush = 25.0;// length of brush
    
    
    float d1; // D0:dist from base to hand (!account the adj for brush)(test = 40cm)
    const float d2 = 10.5;//dist. from floor to pivot on Base
    float d3; // dist. from BasePivot to first arm joint
    float d4;//dist. Basepivot to hand
    float d5;//dist from 1st arm joint to hand
    float d6; //dist of offset cause by hand position
    
    
    
    float z;//dist from floor to hand (provided by posY)
    float xDist; // offset calculation of template triangle
    float hDist; //hypotenuse of template triangle
    float bDist; //distance of base to target
    const float theta0 = 90; //for calculations
    
    float thetaE, thetaF;
    float tempThetaE, tempThetaF;
    float floorToCanvasOffset;
    
    
    //Drawing Mode for Testing........
    deque <int> posX, posY;
    
    ofColor bgtColor;
    
    
    //Activation...............
    bool alphaRun, run, run2, drawMode, drawModeStart;
    
    //Servo's..................
    int Base,lArm,uArm,rHand,fHand; //Servo number on Arduino 
    float turnBase;
    
    float spLarm, spUarm, spRHand, spFHand; //servo positions
    
    
    //Notifications.............
    ofTrueTypeFont        font;
    ofTrueTypeFont      smallFont;
    string echoAnalog, echoDigital;
    
    
    //Arduino Functions........
    
private:
    
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
    void updateArduino();
    
    bool        bSetupArduino;            // flag variable for setting up arduino once
    
    int forwardDir, forwardYDir, reverseDir, YaxisDir, XaxisDir;
    int autoLevel;
    
    ofArduino    ard;
    ofSerial serial;
    
    
    
};
