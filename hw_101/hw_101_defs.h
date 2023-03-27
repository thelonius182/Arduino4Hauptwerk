// Arduino Pins
const byte stepper_piston_pin = 2;
const byte division_piston_pin = 4;
const byte hw_midi_ch = 0; 
const byte event_type_is_cc = 0xB0;
const byte evt_msg_hdr = 0x0B;
const byte hw_ON = 127;
const byte hw_OFF = 0;
const bool pressed = true;
const bool not_pressed = false;
const bool hw_positif = false;
const bool hw_bombarde = true;
const unsigned long hw_ms = 75;


/*
    ControlChange values are the numbers in the decimal column of this table
    https://www.midi.org/specifications-old/item/table-3-control-change-messages-data-bytes-2
*/
const int stepper_piston_cc = 16;
const int division_piston_cc = 17;
const int hw_positif_cc = 17;
const int hw_bombarde_cc = 18;

bool stepper_piston_state = not_pressed;
bool division_piston_state = not_pressed;
bool div_toggle = hw_positif;
