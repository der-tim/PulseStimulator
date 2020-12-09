/*
  Pulsed Stimulation Script

  This script creates a simple TTL pulse pattern that can be used to drive 
  for example a stimulus isolator for electrical stimulation.

  v0.1 of the script creates a burst of 
    N pulses of 
    length L  
    with a frequency F 
    for a time B
    followed by an inter-burst interval I

  Version:
  0.1 (2020-11)

  Authors:
  Tim Schröder - https://github.com/der-tim
*/


// Things to set up - change this for your purposes
const int TTL_PIN = 12; // the number of the pin used for stimulation
int N_pulses = 6; // Number of consecutive pulses
unsigned long L_pulse_length = 250; // in microseconds (us)
unsigned long F_pulse_frequency = 24; // in Hz
unsigned long B_burst_duration = 250000; // burst duration (us)
unsigned long I_inter_burst_interval = 1750000; // inter-burst-interval (us)
unsigned long S_total_stimulation_period_seconds = 30*60; // toal time that the protocol is repeated (in seconds!!)

// nothing to change here
unsigned long off_time = (B_burst_duration - (N_pulses*L_pulse_length) ) / N_pulses; // interval between pulses in a burst
unsigned long loopStartTime;

void setup() {
  pinMode(TTL_PIN, OUTPUT); 
  digitalWrite(TTL_PIN, LOW);
  Serial.begin(9600); // Serial communication with BAUD 9600
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  
  Serial.println("Pulsed Stimulation Script");
  Serial.print("N Pulses: "); Serial.println(N_pulses);
  Serial.print("L Pulse length (us): "); Serial.println(L_pulse_length);
  Serial.print("F pulse frequency (Hz): "); Serial.println(F_pulse_frequency);
  Serial.print("B burst durationy (ms): "); Serial.println(B_burst_duration/1000.0);
  Serial.print("I inter burst interval (ms): "); Serial.println(I_inter_burst_interval/1000.0);
  Serial.print("S total stimulation period (minutes): "); Serial.println(S_total_stimulation_period_seconds/60.0);
  
  Serial.println("Send anything to the Arduino to start the script.");
}

// the loop function runs over and over again forever
void loop() {
  unsigned int n_loops = 0;

  // check every 100 ms whether something ewas sent to the serial port
  while (Serial.available() <= 0) {
    delay(100);
  }

  // empty the serial port, so that new input can be detected
  while (Serial.available() > 0) { Serial.read(); }
  Serial.println("Stimulation started. Send any input to emergency stop the script.");
  
  loopStartTime = millis(); // remember when the stimulation actually started
  // main loop, repeats for interval S
  while (millis() - loopStartTime < 1000*S_total_stimulation_period_seconds) {
    // stimulation burst: N pulses
    for (int i=0; i<N_pulses; i++) {
      // stimulation on:
      digitalWrite(TTL_PIN, HIGH);
      long_delay_microseconds(L_pulse_length);
      // stimulation off:
      digitalWrite(TTL_PIN, LOW);
      long_delay_microseconds(off_time);
    }
    
    // inter-burst interval
    long_delay_microseconds(I_inter_burst_interval);

    // check if new input arrived
    if (Serial.available() > 0) {
        Serial.println("Stimulation interrupted!");
        Serial.print(n_loops);
        Serial.println(" bursts were delivered");
        while (1) { delay(1000); } // loop forever
    }
    
    n_loops += 1; // count number of bursts
  }
  
  Serial.println("Stimulation ended.");
  while (1) { delay(1000); }

}

/*
 * DelayMicroseconds only works precisely for values until 16383. For longer durations, use this function or milis()
 */
void long_delay_microseconds(long delayTime)
{
  while (delayTime > 16383) {
    delayMicroseconds(16383);
    delayTime -= 16383;
  }
  delayMicroseconds(delayTime);
}
