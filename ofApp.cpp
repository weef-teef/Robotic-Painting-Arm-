#include "ofApp.h"
#include <cmath>
#include <math.h>


/*
 TO DO:

  1.
        i. On motion/ off motion
        ii. Several paint motions
        iii. dipping into paint motion
        iiii. Mixing paint?
 
 2.
        i. change bDist to echolocation position
        ii.acount for paint brush distance
 
 
 
 */

//--------------------------------------------------------------
void ofApp::setup(){
    
    //Check everytime if usb.modem##### is the same....
    ard.connect("/dev/tty.usbmodem14101", 57600);
    ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
    bSetupArduino    = false;
    ofSetVerticalSync(true);
    
    //Arduino Pin Numbers.......
    Base = 10;
    lArm = 9;
    uArm = 6;
    rHand = 5;
    fHand = 3;
    
    floorToCanvasOffset = 15;//Offset from floor to bottom of canvas , this can be set to anything
    
    
    //Activations........
    run = false; //base and upper arm movements
    run2 = false; // Hand rotations
    alphaRun = false; // starts possible functions
    
    drawMode = false;
    drawModeStart = false;
    
    
    
    turnBase = 90;
    
    //Notifications.......
    font.load("franklinGothic.otf", 14);
    smallFont.load("franklinGothic.otf", 14);
    


}

//--------------------------------------------------------------
void ofApp::update(){
    
    updateArduino();
    

    
    
    
    //Manual Servo Movements..................
    
    if (alphaRun){
    if (run ){
        
        forwardDir = ofMap(mouseX, 0, ofGetWidth(), 0, 150); //lower arm
        forwardYDir = ofMap(mouseY, 0, ofGetHeight(), 0, 180); //upperarm
        reverseDir = ofMap(forwardYDir, 0, 180, 180, 0);
        
 
        ard.sendServo(uArm , reverseDir);
        ard.sendServo(lArm , forwardDir);
        ard.sendServo(Base , abs(turnBase));
        
        spLarm = forwardDir;
        spUarm = reverseDir;
        
        
        
        //test.....
//        autoLevel = ((forwardDir + reverseDir)/2);
//        ard.sendServo(rHand ,autoLevel * 1.5); //rotational hand control
//        
        
        //test 2.......
//
//        autoLevel = ofMap(forwardDir, 0, 150, 90, 180);
//
//        ard.sendServo(rHand ,autoLevel); //rotational hand control

    }
    
    if (run2){
        
        YaxisDir = (ofMap(mouseY, 0, ofGetHeight(), 0, 180)); //Hand controls
        XaxisDir = (ofMap(mouseX, 0, ofGetWidth(), 0, 180)); // Hand controls
        
        ard.sendServo(fHand , XaxisDir);
        ard.sendServo(rHand , XaxisDir);
        
        spFHand = XaxisDir;
        spRHand = YaxisDir;
        
            }
                        }
 
    
    if (drawMode){
        if(drawModeStart){
         
            for (int i = 0 ; i < posX.size(); i++){
                  if (posX[i] > 10 && posX[i] < 260 && posY[i] > 10 && posY[i] < 360){
         
         
          z = ((ofMap(posY[i],10, 350, 350,10 ))/10) +floorToCanvasOffset;
                    
  
            std::cout << "z:......" << z << endl;
            
            bDist = 40; //this will need to be changed to echo position later
                      
                      
            theta1 = 90; //For testing purposes alone CHANGE TO VAR. LATER
            
            xDist = z - d2;
            hDist =sqrt((xDist * xDist)+ (bDist * bDist));
                      std::cout << "xDist:......" << xDist << endl;
                      std::cout << "hDist:......" << hDist << endl;
            
            tempThetaE = ((bDist * bDist) + (hDist * hDist) - (xDist * xDist))/(2 * hDist * bDist);
            thetaE = ofRadToDeg(acos(tempThetaE));
            
            tempThetaF = ((l1*l1) + (hDist*hDist)-(l2 *l2))/(2 * l1 * hDist);
            thetaF = ofRadToDeg(acos(tempThetaF));
                      std::cout << "thetaE:......" << thetaE << endl;
                      std::cout << "thetaF:......" << thetaF << endl;
                      
            
            theta2 = 180 - (thetaE + thetaF);
            
            float tempTheta4 = ((l2*l2)+(l1*l1)-(hDist*hDist))/(2*l2*l1);
            theta4 = ofRadToDeg(acos(tempTheta4));  //Map backwards? ...yes?
            theta5 = ofMap(theta4, 0, 180, 180, 0); //mapping 4 backwards
                      
                     
            
                      std::cout << "theta4:......" << theta4 << endl;
                      std::cout << "theta5:......" << theta5 << endl;
                      std::cout << "theta2:......" << theta2 << endl;
            
            ard.sendServo(uArm , theta5);
            ard.sendServo(lArm , theta2);
            ard.sendServo(Base , theta1); //90 is facing forward (for test it is static)
            
                      posY.clear();
            }
            }
            
        }

        
    }
    
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    bgtColor = (255);
    ofSetBackgroundColor(bgtColor);
    
    ofNoFill();
    ofSetColor(0);
    ofDrawRectangle(10, 10, 250, 350);
    ofFill();
    
    ofSetColor(64, 64, 64);
    smallFont.drawString("This is a calibration test system:\nTo run a visual test of the math press 'd' \n then press 's'  then click inside the box\n  (right now the x axis is turned off to simplify my calculations) \nTo run manual movements:\n wait for screen to turn green before pressing 'r' to start\n (you might also need to click the mouse once).\nPress LEFT or RIGHT to move the Base. \nMove the mouse to control the arms (x = lower arm movements \ny = upper arm movements), hold 'mouse-click' to move hand.", 200, 500);
    
 
    if (bSetupArduino){
        bgtColor = ofColor(15, 184, 79);
        ofSetBackgroundColor(bgtColor);
        
        font.drawString(echoAnalog, 515, 30);
       // font.drawString(echoDigital, 200, 30);
        font.drawString("Forward Hand: " + ofToString(spFHand) , 515, 70);
        font.drawString("Rotation Hand: " + ofToString(spRHand) , 515, 90);
        font.drawString("Lower Arm: " + ofToString(spLarm) , 515, 110);
        font.drawString("Upper Arm: " + ofToString(spUarm) , 515, 130);
        font.drawString("Base: " + ofToString(turnBase) , 515, 150);
        font.drawString("AutoLevel: " + ofToString(autoLevel) , 515, 170);
        font.drawString("Run: " + ofToString(alphaRun) , 515, 190);
        
      
        
        
        
        
        ofSetColor(64, 64, 64);
        smallFont.drawString("This is a calibration test system: Press 'r' to start. "
                             "\nPress LEFT or RIGHT to move the Base. \nMove the mouse to control the arms, hold 'mouse-click' to move hand.", 200, 500);
        ofSetColor(255, 255, 255);
        
    }
    
   //Drawing Mode...............
    
    if (drawMode){
        
        alphaRun = false;
        
        ofPushStyle();
        
        ofSetColor(0);
        font.drawString(echoAnalog, 515, 30);
        // font.drawString(echoDigital, 200, 30);
        font.drawString("Forward Hand: " + ofToString(spFHand) , 515, 70);
        font.drawString("Rotation Hand: " + ofToString(spRHand) , 515, 90);
        ofSetColor(255,0,0);
        font.drawString("Lower Arm: " + ofToString(spLarm) , 515, 110);
        font.drawString("Upper Arm: " + ofToString(spUarm) , 515, 130);
        ofSetColor(0);
        font.drawString("Base: " + ofToString(turnBase) , 515, 150);
        font.drawString("AutoLevel: " + ofToString(autoLevel) , 515, 170);
        font.drawString("Run: " + ofToString(alphaRun) , 515, 190);
        ofPopStyle();
       
        for (int i = 0 ; i < posX.size(); i++){
    ofSetColor(255, 0, 0);
            if (posX[i] > 10 && posX[i] < 260 && posY[i] > 10 && posY[i] < 360){
        
            ofDrawCircle(posX[i], posY[i], 4);
            
            ofSetColor(bgtColor);
            ofDrawRectangle(0, 370, 150, 500);
           
                int pH = 400;
            ofSetColor(0);
             font.drawString("posY: " + ofToString((ofMap(posY[i],10, 350, 350,10 ))/10) + "cm", 0, pH +20);
                font.drawString("Z......" + ofToString(z), 10, pH +40);
                font.drawString("bDist......" + ofToString(bDist), 10, pH+60);
                font.drawString("xDist:......" + ofToString(xDist), 10, pH+80);
                font.drawString("hDist:......" + ofToString(hDist), 10, pH+100);
                font.drawString("d2:......" + ofToString(2), 10, pH+120);
                font.drawString("L1:......" + ofToString(l1), 10, pH+160);
                font.drawString("L2:......" + ofToString(l2), 10, pH+180);
                font.drawString("thetaE:......" + ofToString(thetaE), 10, pH + 200);
                font.drawString("thetaF:......" + ofToString(thetaF), 10, pH + 220);
                  ofSetColor(255,0,0);
                font.drawString("UPPER ARM:......" + ofToString(theta2), 10, pH + 240);
                  ofSetColor(0);
                font.drawString("theta4:......" + ofToString(theta4), 10, pH+260);
                  ofSetColor(255,0,0);
                font.drawString("LOWER ARM:......" + ofToString(theta5), 10, pH+280);
                
                
        
        //math TEST #1.............
        
        
        

        
            }
        }
                
                
                
    }

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    if (key == 'r' || key == 'R'){
        alphaRun = !alphaRun;
        
        
    }
    if (key == 'd' || key == 'D'){
        drawMode = !drawMode;
        
        
    }
    
    if (key == 's' || key == 'S'){
        drawModeStart = !drawModeStart;
        
        
    }
    
    if (key == OF_KEY_RIGHT){
       
        turnBase += 1;
        
    }
    if (key == OF_KEY_LEFT){
        
        
        turnBase -= 1;
    }
    
 
    
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
  
     if (alphaRun){
         run2 = true;
         run = false;
     }
    
     if (drawMode){
    posX.push_back(mouseX);
    posY.push_back(mouseY);
     }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    run = true;
    run2 = false;
    
}
//--------------------------------------------------------------
void ofApp::setupArduino(const int & version) {
    
    // remove listener because we don't need it anymore
    ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    ofLogNotice() << ard.getFirmwareName();
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
    
    //     set pin D11 as PWM (analog output)
   
    //Need one for light.....
  //  ard.sendDigitalPinMode(1, ARD_PWM);
    
    // attach a servo to pin
        ard.sendServoAttach(3);
        ard.sendServoAttach(5);
        ard.sendServoAttach(6);
        ard.sendServoAttach(9);
        ard.sendServoAttach(10);
    
    ard.sendDigitalPinMode(11, ARD_PWM);
 
    
ard.sendAnalogPinReporting(0, ARD_ANALOG); // Sets A0 as input
ard.sendDigitalPinMode(2, ARD_INPUT);
    
    // Listen for changes on the digital and analog pins
    ofAddListener(ard.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
    ofAddListener(ard.EAnalogPinChanged, this, &ofApp::analogPinChanged);
}
//--------------------------------------------------------------
void ofApp::updateArduino(){
    
    // update the arduino, get any data or messages.
    // the call to ard.update() is required
    ard.update();
    
    // do not send anything until the arduino has been set up
    if (bSetupArduino) {
      //   fade the led connected to pin D11
        ard.sendPwm(11, (int)(128 + 128 * sin(ofGetElapsedTimef())));   // pwm...
    }
    
}
//--------------------------------------------------------------
void ofApp::digitalPinChanged(const int & pinNum) {
    // do something with the digital input. here we're simply going to print the pin number and
    // value to the screen each time it changes
       echoDigital = "E DIGITAL: " + ofToString(pinNum) + " = " + ofToString(ard.getDigital(pinNum));
}
void ofApp::analogPinChanged(const int & pinNum) {
    // do something with the analog input. here we're simply going to print the pin number and
    // value to the screen each time it changes
        echoAnalog = "ECHO: " + ofToString(pinNum) + " = " + ofToString(ard.getAnalog(pinNum));
}


//--------------------------------------------------------------





