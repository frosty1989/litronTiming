// define the flag - set to false at start, variables changed in interrupt must be volatile
volatile boolean PassThruMode = false;


int interruptPin = 2;

// set one input and one output for the LAMP SYNC signal
int inputLamp = 8;
int outputLamp = 9;

void setup() {
  // put your setup code here, to run once:

  // set the interrupt PIN to 2
  pinMode(interruptPin, INPUT);

  // set the pinModes for LAMP SYNC signal
  pinMode(inputLamp, INPUT);
  pinMode(outputLamp, OUTPUT);
  
  // attach the interrupt function to interruptPin - has to be set to CHANGE
  attachInterrupt(digitalPinToInterrupt(interruptPin), gate, CHANGE);

}

void gate() {

  // declare the gate function - it has two branches, depending if the DO signal is 
  // rising or falling

  // DO is rising branch
    if (digitalRead(interruptPin) == HIGH) {

      // redirect the LAMP SYNC signal to output - use a flag
      PassThruMode = true;
    
    }


    else if (digitalRead(interruptPin) == LOW)  {
      
      // stop redirecting the LAMP SYNC to output - use a flag
      PassThruMode = false;

    }

}

void loop() {
  // put your main code here, to run repeatedly
  // depending on the value of the flag, redirect LAMP SYNC to output
    if(PassThruMode) {
    digitalWrite(outputLamp, digitalRead(inputLamp));
  }

  else {

    digitalWrite(outputLamp, LOW);
    
  }

}
