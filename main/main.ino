
// the setup function runs once when you press reset or power the board
int output_pin = 10;
int button_pushed = 0; // 1 if button is pushed and 0 if it is'nt 
int interruptPin = 2;

void setup() {

  pinMode(output_pin, OUTPUT);

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), stopAlarm, FALLING);

  // pinMode(LED_BUILTIN, OUTPUT); // meant for debugging 
}

// the loop function runs over and over again forever
void loop() {

  if (!button_pushed)
  {
    outputNote(1200, 500);
    outputNote(800, 500);
  }
  
}

void outputNote(int frequency, long time){

  int cycle_us = 1000000 / frequency;
  for (int i = 0; i <= (time * frequency) / 1000 ; i++){

    if(!button_pushed)
    {
      digitalWrite(output_pin, HIGH);  // turn the LED on (HIGH is the voltage level)
      delayMicroseconds((cycle_us / 2));
      
      
      digitalWrite(output_pin, LOW);   // turn the LED off by making the voltage LOW
      delayMicroseconds((cycle_us / 2));
    }
    
  }
}

void stopAlarm()
{
  button_pushed = 1;
  return;
}