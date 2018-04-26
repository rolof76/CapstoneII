#include "MIDIUSB.h"

void noteOn(byte channel, byte pitch, byte velocity){
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

const int TRIG_PIN = 1; //pd3
const int ECHO_PIN = 4; //pd4
int cm_new = 20;
int cm_old = 20;
float cm;
long duration;

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

}

void loop() {

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);

  cm_new = map(constrain(duration, 20, 10000), 6000, 50, 96, 24);

  if((cm_new > 96)||(cm_new <  24)) {
    noteOff(0, cm_old, 0);
    cm_old = cm_new;
  }
  else if(cm_new != cm_old) {
    noteOn(0, cm_new, 127);
    MidiUSB.flush();
    delay(15);
    noteOff(0, cm_old, 0);
    MidiUSB.flush();
    cm_old = cm_new;
  }
  delay(15);
}
