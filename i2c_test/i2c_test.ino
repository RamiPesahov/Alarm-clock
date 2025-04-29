#include <Wire.h>
#include <DS3231.h>


DS3231 myRTC;
void setup(){

    Serial.begin(38400);
    Wire.begin();
    
}

void loop(){
    bool h12;
    bool hAM;
    byte hour = myRTC.getHour(h12,hAM);

    byte minute = myRTC.getMinute();

    byte sec = myRTC.getSecond();
    
    Serial.println(hour);
    Serial.println(minute);
    Serial.println(sec);

}