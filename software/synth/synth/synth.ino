#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <SerialFlash.h>
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=265,416
AudioSynthNoiseWhite     noise1;         //xy=425,689
AudioFilterStateVariable filter1;        //xy=431,426
AudioEffectEnvelope      envelope1;      //xy=700,435
AudioEffectDelay         delay1;         //xy=739,609
AudioFilterStateVariable filter2;        //xy=899,585
AudioMixer4              mixer1;         //xy=910,425
AudioOutputI2S           i2s1;           //xy=1117,396
AudioConnection          patchCord1(waveform1, 0, filter1, 0);
AudioConnection          patchCord2(filter1, 0, envelope1, 0);
AudioConnection          patchCord3(envelope1, 0, mixer1, 0);
AudioConnection          patchCord4(delay1, 0, filter2, 0);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=394,544
// GUItool: end automatically generated code



int incomingByte = 0;   // for incoming serial data
float notes[12] = {523.25, 587.33, 659.25, 783.99, 880.00, 1046.50, 1174.66, 1318.51, 1567.98, 1760.00, 2093.00, 2349.32};
float cutoff = 10000.0;

int nextstate = 0;
int readstate = 0;
int keydown = 0;
float note = 440.0;

void setup() {
  Serial.begin(115200);
  AudioMemory(10);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.3);
  waveform1.begin(WAVEFORM_SQUARE);
  delay(1000);
  filter1.frequency(cutoff);
  waveform1.frequency(440.0);
  waveform1.amplitude(1.0);
  filter1.resonance(1.0);
  filter2.frequency(30);
  mixer1.gain(0, 1.0);
  mixer1.gain(1, 0.0);
}

void loop() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
    if (incomingByte == '\n') {
      nextstate = 0;
      if (keydown) {
        Serial.println("Bang");
        waveform1.frequency(note);
        envelope1.noteOn();
      } else {
        Serial.println("Off");
        envelope1.noteOff();
      }
    }
    if (readstate == 1 && incomingByte >= 49 && incomingByte < 58) {
      note = notes[incomingByte - 49];
      nextstate = 2;
    }
    else if (readstate == 0) {
      keydown = incomingByte == '1';
      Serial.println(keydown, DEC);
      nextstate = 1;
    }
    readstate = nextstate;
    /*
    if (incomingByte == '=') {
      cutoff *= 1.5;
      filter1.frequency(cutoff);
    } else if (incomingByte == '-') {
      cutoff /= 1.5;
      filter1.frequency(cutoff);
    }
    */
  }
}
