#include <FastGPIO.h>
#include <util/delay.h>

// pin switch 
int switchPin = 12;

// helpers for the switch input - current and previous reading state
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

// times for switch debouncing
// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers


// define the flag - set to false at start, variables changed in interrupt must be volatile
volatile boolean PassThruMode = false;

// pulse width
int pulseWidth = 0;

// set the interrupt pin  for gate
int interruptPin = 2;

// set one input and one output for the LAMP SYNC signal
int inputLamp = 3;
int outputLamp = 9;

void setup() {
  // put your setup code here, to run once:

  // set fastGPIO to gate pin
  FastGPIO::Pin<2>::setInput();

  // set fastGPIO
  FastGPIO::Pin<3>::setInput();
  FastGPIO::Pin<9>::setOutput(LOW);
  
  // attach the interrupt function to interruptPin - has to be set to CHANGE
  attachInterrupt(digitalPinToInterrupt(interruptPin), gate, CHANGE);
  
  // 3 this is the interrupt pin for the LAMP SYNC signal
  attachInterrupt(digitalPinToInterrupt(inputLamp), output, RISING);


}

void output() {
  
      if(PassThruMode) {
        
        // I think I cannot use the pin variable here
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

  // in the main code, only the switch will be handled

}
