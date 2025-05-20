#include <DS3231.h>
#include <Wire.h>



// the setup function runs once when you press reset or power the board
int output_pin = 10;
int buttonInterruptPin = 2;
int alarmInterruptPin = 3;

volatile byte alarmOn = 0; // 0 If the alarm isn't occuring. 1 If we are alarming


DS3231 myRTC;
byte alarmDay;
byte alarmHour;
byte alarmMinute;
byte alarmSecond;
byte alarmBits;
bool alarmDayIsDay;
bool alarmH12;
bool alarmPM;

bool century = false;
bool h12Flag;
bool pmFlag;

int del = 1000000;

void setup() {

  Wire.begin();
  Serial.begin(38400);

// Set the DS3231 clock mode to 24-hour
  myRTC.setClockMode(false); // false = not using the alternate, 12-hour mode

  // Assign parameter values for Alarm 1
  alarmDay = myRTC.getDate();
  alarmHour = 20;
  alarmMinute = 40;
  alarmSecond = 0; // initialize to the interval length
  alarmBits = 0b00001000; // Alarm 1 when seconds match
  alarmDayIsDay = false; // using date of month

  // Upload initial parameters of Alarm 1
  myRTC.turnOffAlarm(1);
  myRTC.setA1Time(
      alarmDay, alarmHour, alarmMinute, alarmSecond,
      alarmBits, alarmDayIsDay, alarmH12, alarmPM);
  // clear Alarm 1 flag after setting the alarm time
  myRTC.checkIfAlarm(1);
  // now it is safe to enable interrupt output
  myRTC.turnOnAlarm(1);

  // When using interrupt with only one of the DS3231 alarms, as in this example,
  // it may be possible to prevent the other alarm entirely,
  // so it will not covertly block the outgoing interrupt signal.

  // Try to prevent Alarm 2 altogether by assigning a 
  // nonsensical alarm minute value that cannot match the clock time,
  // and an alarmBits value to activate "when minutes match".
  alarmMinute = 0xFF; // a value that will never match the time
  alarmBits = 0b01100000; // Alarm 2 when minutes match, i.e., never
  
  // Upload the parameters to prevent Alarm 2 entirely
  myRTC.setA2Time(
      alarmDay, alarmHour, alarmMinute,
      alarmBits, alarmDayIsDay, alarmH12, alarmPM);
  // disable Alarm 2 interrupt
  myRTC.turnOffAlarm(2);
  // clear Alarm 2 flag
  myRTC.checkIfAlarm(2);

  // NOTE: both of the alarm flags must be clear
  // to enable output of a FALLING interrupt

  // attach clock interrupt
  pinMode(alarmInterruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(alarmInterruptPin), startAlarm, FALLING);


  pinMode(output_pin, OUTPUT);

  pinMode(buttonInterruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonInterruptPin), stopAlarm, FALLING);

  // pinMode(LED_BUILTIN, OUTPUT); // meant for debugging 

  
}

// the loop function runs over and over again forever
void loop() {


  if (alarmOn)
  {
    outputNote(1200, 500);
    outputNote(800, 500);
  }
  
}

void outputNote(int frequency, long time){

  int cycle_us = 1000000 / frequency;
  for (int i = 0; i <= (time * frequency) / 1000 ; i++){

    if(alarmOn)
    {
      digitalWrite(output_pin, HIGH);  
      delayMicroseconds((cycle_us / 2));
      
      
      digitalWrite(output_pin, LOW);  
      delayMicroseconds((cycle_us / 2));
    }
    
  }
}

void stopAlarm()
{
  alarmOn = 0;
  myRTC.checkIfAlarm(1);
  return;
}

void startAlarm()
{ 
  alarmOn = 1;
  return;
}