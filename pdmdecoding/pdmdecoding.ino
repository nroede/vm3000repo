/*
  This example reads audio data from the on-board PDM microphones, and prints
  out the samples to the Serial console. The Serial Plotter built into the
  Arduino IDE can be used to plot the audio data (Tools -> Serial Plotter)

  Circuit:
  - Arduino Nano 33 BLE board, or
  - Arduino Nano RP2040 Connect, or
  - Arduino Portenta H7 board plus Portenta Vision Shield, or
  - Arduino Nicla Vision

  This example code is in the public domain.
*/

#include <PDM.h>

// default number of output channels
static const char channels = 1;

// default PCM output frequency
static const int frequency = 16000;

// Buffer to read samples into, each sample is 16-bits
int16_t sampleBuffer[512];

// Number of audio samples read
volatile int samplesRead;

float start_timestamp;
float timestamp;
volatile int totalSamples = 0;

void setup() {
  Serial.begin(19200);
  while (!Serial);

  // Configure the data receive callback
  PDM.onReceive(onPDMdata);

  // Optionally set the gain
  // Defaults to 20 on the BLE Sense and 24 on the Portenta Vision Shield
  // PDM.setGain(30);

  // Initialize PDM with:
  // - one channel (mono mode)
  // - a 16 kHz sample rate for the Arduino Nano 33 BLE Sense
  // - a 32 kHz or 64 kHz sample rate for the Arduino Portenta Vision Shield
  if (!PDM.begin(channels, frequency)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }
//  start_timestamp = micros()/(float)1000;
    start_timestamp = millis();
}

void loop() {
  while (totalSamples < 32000) {
    // Wait for samples to be read
    if (samplesRead) {
  
      // Print samples to the serial monitor or plotter
      for (int i = 0; i < samplesRead; i++) {
  //      timestamp = micros()/(float)1000;
  //      timestamp = millis();
//        if(channels == 2) {
//          Serial.print("L:");
//          Serial.print(sampleBuffer[i]);
//          Serial.print(" R:");
//          i++;
//        }

          if(abs(sampleBuffer[i]) >= 32767) continue;
          Serial.write((byte *) &sampleBuffer[i], 2);
      }
  
      // Clear the read count
      totalSamples += samplesRead;
      samplesRead = 0;
    }
  }
//  Serial.print("TOTAL SAMPLES = ");
//  Serial.println(totalSamples);
//  Serial.print("Time for execution = ");
//  Serial.println(millis() - start_timestamp);
//    Serial.print("L");
}

/**
 * Callback function to process the data from the PDM microphone.
 * NOTE: This callback is executed as part of an ISR.
 * Therefore using `Serial` to print messages inside this function isn't supported.
 * */
void onPDMdata() {
  // Query the number of available bytes
  int bytesAvailable = PDM.available();

  // Read into the sample buffer
  PDM.read(sampleBuffer, bytesAvailable);

  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;
}
