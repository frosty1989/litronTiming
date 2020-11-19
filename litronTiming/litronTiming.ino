#include <FastGPIO.h>
#include <util/delay.h>


// define the flag - set to false at start, variables changed in interrupt must be volatile
volatile boolean PassThruMode = false;

// pulse width
int pulseWidth = 0;

// set the interrupt pin
int interruptPin = 2;

// set one input and one output for the LAMP SYNC signal
int inputLamp = 8;
int outputLamp = 9;

void setup() {
  // put your setup code here, to run once:

  // set the interrupt PIN to 2
  //pinMode(interruptPin, INPUT);

  // set fastGPIO
  FastGPIO::Pin<2>::setInput();

  // set the pinModes for LAMP SYNC signal
  //pinMode(inputLamp, INPUT);
  //pinMode(outputLamp, OUTPUT);

  // set fastGPIO
  FastGPIO::Pin<3>::setInput();
  FastGPIO::Pin<9>::setOutput(LOW);
  
  // attach the interrupt function to interruptPin - has to be set to CHANGE
  attachInterrupt(digitalPinToInterrupt(interruptPin), gate, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), output, RISING);


}

void output() {
  
      if(PassThruMode) {

        FastGPIO::Pin<9>::setOutputValueHigh();
        _delay_us(10);
        FastGPIO::Pin<9>::setOutputValueLow();

        
      }
  
}

void gate() {

  // declare the gate function - it has two branches, depending if the DO signal is 
  // rising or falling

  // DO is rising branch
    if (FastGPIO::Pin<2>::isInputHigh()) {

      // redirect the LAMP SYNC signal to output - use a flag
      PassThruMode = true;
    
    }

    else {
      
      // stop redirecting the LAMP SYNC to output - use a flag
      PassThruMode = false;

    }

}

void loop() {
  // put your main code here, to run repeatedly
  // depending on the value of the flag, redirect LAMP SYNC to output

/*
    if(PassThruMode) {

    // fast GPIO read value of the input and according to that set LAMP SYNC - HIGH OR LOW
    bool inputHigh = FastGPIO::Pin<8>::isInputHigh();

    // try pulseIn

    if (inputHigh) {
      
        FastGPIO::Pin<9>::setOutputValueHigh();
        _delay_us(10);
        inputHigh = false;
      
    }

    else {
      
        FastGPIO::Pin<9>::setOutputValueLow();     
        
    }
      
    //digitalWrite(outputLamp, digitalRead(inputLamp));
  }

  else {

    digitalWrite(outputLamp, LOW);
    
  }
*/
}
