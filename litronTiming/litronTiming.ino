#include <FastGPIO.h>
#include <util/delay.h>

// pin switch 
int switchPin = 12;

// swichFlag for interrupts - it does not matter how you initialize it
volatile boolean switchFlagSingleShot = false;

// define the flag - set to false at start, variables changed in interrupt must be volatile
volatile boolean PassThruMode = false;

// flag for LAMP SYNC interrupt signal
volatile boolean lampSyncRising = false;

// pulse width
int pulseWidth = 0;

// set the interrupt pin  for gate
int interruptPin = 2;

// set one input and one output for the LAMP SYNC signal
int inputLamp = 3;
int outputLamp = 9;

void setup() {
  // put your setup code here, to run once:

  // set the internal pull up resistor, unpressed switch is HIGH
  pinMode(switchPin, INPUT_PULLUP); 

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

        // here comes the single shot or continous selector

        // continuous
        if(switchFlagSingleShot == false) {
        
          // I think I cannot use the pin variable here
          FastGPIO::Pin<9>::setOutputValueHigh();
          _delay_us(10);
          FastGPIO::Pin<9>::setOutputValueLow();

        }

        else if(switchFlagSingleShot == true) {
          
        // output only one pulse and set passThruMode to false
        FastGPIO::Pin<9>::setOutputValueHigh();
        _delay_us(10);
        FastGPIO::Pin<9>::setOutputValueLow();     

        PassThruMode = false;
          
        }
        
      }
  
}

void gate() {

  // declare the gate function - it has two branches, depending if the DO signal is 
  // rising or falling

  // DO is rising branch
    if (FastGPIO::Pin<2>::isInputHigh()) {

      // wait for the rising edge of the lamp signal - use fastgpio
      // this is kinda a synchronizing condition for the second interrupt 
      // the same way could be probably used for the falling branch of the interrupt
      while (!FastGPIO::Pin<2>::isInputHigh()) {
          // Do nothing
      }
      // and directly after set  PassThruMode = true

      // redirect the LAMP SYNC signal to output - use a flag
      PassThruMode = true;
    
    }

    // falling branch
    else {
      
      // stop redirecting the LAMP SYNC to output - use a flag
      PassThruMode = false;

    }

}

void loop() {

  // unpressed switch is HIGH --> let's say it is single shot
  if (digitalRead(switchPin) == HIGH) {
    
      switchFlagSingleShot = true;

  } 

  // else the switch is pressed and set to continuous
  else {
    
      switchFlagSingleShot = false;
    
  }

}
