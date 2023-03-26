// ------------------------------------------------------------------
// MIDI-controller for HW foot pistons
// Adapted from: https://github.com/Hecsall/arduino-midi-footswitch
//               https://www.arduino.cc/en/Reference/MIDIUSB
//
// Version 2023-03-26.1
// ------------------------------------------------------------------

#include "MIDIUSB.h"
#include "hw_101_defs.h"

void controlChange(byte cc_num, byte cc_payload) {
  midiEventPacket_t event = { evt_msg_hdr, event_type_is_cc | hw_midi_ch, cc_num, cc_payload };
  MidiUSB.sendMIDI(event);
}

// Floating Division Routes 1 and 2 have their own switch in HW. To operate these 2 HW-switches from a single piston,
// a state variable (toggle) is introduced to represent the currently activated route. Pressing the piston and looking
// at the state can then activate "the other one" by engaging the "other" cc_num.

// get the cc-number of the "other" floating division route.
// Side effect: changes global variable div_toggle
int toggle_div() {

  if (div_toggle == hw_positif) {
    div_toggle = hw_bombarde;
    return hw_bombarde_cc;
  }

  div_toggle = hw_positif;
  return hw_positif_cc;
}

// get the cc-number of the current floating division route
int get_div() {

  if (div_toggle == hw_positif) {
    return hw_positif_cc;
  }

  return hw_bombarde_cc;
}

void handleStepperPiston() {
  if (digitalRead(stepper_piston_pin) == LOW && stepper_piston_state == not_pressed) {
    controlChange(stepper_piston_cc, hw_ON);
    MidiUSB.flush();
    stepper_piston_state = pressed;
    delay(hw_ms);
  } else if (digitalRead(stepper_piston_pin) == HIGH && stepper_piston_state == pressed) {
    controlChange(stepper_piston_cc, hw_OFF);
    MidiUSB.flush();
    stepper_piston_state = not_pressed;
    delay(hw_ms);
  }
}

void handleDivisionPiston() {
  if (digitalRead(division_piston_pin) == LOW && division_piston_state == not_pressed) {
    controlChange(toggle_div(), hw_ON);
    MidiUSB.flush();
    division_piston_state = pressed;
    delay(hw_ms);
  } else if (digitalRead(division_piston_pin) == HIGH && division_piston_state == pressed) {
    controlChange(get_div(), hw_OFF);
    MidiUSB.flush();
    division_piston_state = not_pressed;
    delay(hw_ms);
  }
}

// Actual logic
void setup() {
  Serial.begin(115200);
  pinMode(stepper_piston_pin, INPUT_PULLUP);
  pinMode(division_piston_pin, INPUT_PULLUP);
}

void loop() {
  handleStepperPiston();
  handleDivisionPiston();
}