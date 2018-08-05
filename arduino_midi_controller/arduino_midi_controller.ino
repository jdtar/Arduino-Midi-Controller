#include <MIDI.h>
#include "Controller.h"

MIDI_CREATE_DEFAULT_INSTANCE();

byte NUMBER_BUTTONS = 8;      // number of buttons connected to Digital Input
byte NUMBER_POTS = 2;         // number of pots connected to Analog Input
byte NUMBER_MUX_BUTTONS = 8;  // number of buttons connected to multiplexer


Mux M1(5, 8, false);          // Creating multiplexer object
                // Mux(z pin, NUMBER_MUX_BUTTONS, analog pin?)

/*  For every input, define as (pin_number, Command, CC Control, Channel).
  Command - Midi Command Parameter. Changes CC Control Values.  
  CC Control - Midi Control Change. Defines which parameter to be changed.
  Channel - Defines which channel commands are sent over. 
  * Refer to Midi.h for Command Parameter and CC Control Values * 
*/

Pot PO1(A0, 0, 7, 5);     // Define potentiometers as objects
Pot PO2(A2, 0, 9, 5);       

// Initialize Potentiometer Array
Pot *POTS[] {&PO1,&PO2};   

/*  Defines buttons directly connected to arduino DigitalIn as objects 
  Button (pin_number, Command, Note Number, Channel, Debounce Time in ms)
  Command: 0 = note
       1 = CC Control
       2 = CC Control toggle, -> toggles CC Control. 1 press init, 2 disable
  Note Number: Refer to Midi.h
*/

Button BU1(6, 0, 48, 1, 5 );  
Button BU2(7, 0, 49, 1, 5 );
Button BU3(8, 0, 50, 1, 5 );
Button BU4(9, 0, 51, 1, 5 );	// Connected to button w/ resistor
Button BU5(10, 0, 44, 1, 5 );
Button BU6(11, 0, 45, 1, 5 );
Button BU7(12, 0, 46, 1, 5 );
Button BU8(13, 0, 47, 1, 5 );


// Initialize DigitalIn button array

Button *BUTTONS[] {&BU1, &BU2, &BU3, &BU4, &BU5, &BU6, &BU7, &BU8};

/*  Defines buttons connected to multiplexer as objects
  Button (Multiplexer Object, pin_number, Command , Value, Channel, Debounce Time in ms)
  Command: 0 = note
       1 = CC Control
       2 = CC Control toggle, -> toggles CC Control. 1 press init, 2 disable
  Value: Note Value or CC command value
  Note Number: Refer to Midi.h
*/
Button MBO1(M1, 0, 0, 40, 1, 5);
Button MBO2(M1, 1, 0, 41, 1, 5);
Button MBO3(M1, 2, 0, 42, 1, 5);
Button MBO4(M1, 3, 0, 43, 1, 5);
Button MBO5(M1, 4, 0, 36, 1, 5);
Button MBO6(M1, 5, 0, 37, 1, 5);
Button MBO7(M1, 6, 0, 38, 1, 5);
Button MBO8(M1, 7, 0, 39, 1, 5);

//Initialize multiplexer button array
Button *MUXBUTTONS[] {&MBO1, &MBO2, &MBO3, &MBO4, &MBO5, &MBO6, &MBO7,&MBO8};

void setup() {
  MIDI.begin(MIDI_CHANNEL_OFF); // Initialize all inputs off
  Serial.begin(115200);


}

void loop() {
  Buttons_DI();
  Pots_AI();
  MuxButtons();

}


/*    Main DigitalIn Button Loop    */
void Buttons_DI() {

  // Cycle through Button array
  for (int i = 0; i < NUMBER_BUTTONS; i = i + 1) {
    byte message = BUTTONS[i]->getValue();

    //  Button is pressed
    if (message == 0) {
      switch (BUTTONS[i]->Bcommand) {
        case 0: //Note
          MIDI.sendNoteOn(BUTTONS[i]->Bvalue, 127, BUTTONS[i]->Bchannel);
          break;
        case 1: //CC
          MIDI.sendControlChange(BUTTONS[i]->Bvalue, 127, BUTTONS[i]->Bchannel);
          break;
        case 2: //Toggle
          if (BUTTONS[i]->Btoggle == 0) {
            MIDI.sendControlChange(BUTTONS[i]->Bvalue, 127, BUTTONS[i]->Bchannel);
            BUTTONS[i]->Btoggle = 1;
          }
          else if (BUTTONS[i]->Btoggle == 1) {
            MIDI.sendControlChange(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);
            BUTTONS[i]->Btoggle = 0;
          }
          break;
      }
    }

    //  Button is not pressed
    if (message == 1) {
      switch (BUTTONS[i]->Bcommand) {
        case 0:
          MIDI.sendNoteOff(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);
          break;
        case 1:
          MIDI.sendControlChange(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);
          break;
      }
    }
  }
}
/*    Main Multiplexed Button Loop    */
void MuxButtons() {

  // Cycle through Mux Button array
  for (int i = 0; i < NUMBER_MUX_BUTTONS; i = i + 1) {

    MUXBUTTONS[i]->muxUpdate();
    byte message = MUXBUTTONS[i]->getValue();

    //  Button is pressed
    if (message == 0) {
      switch (MUXBUTTONS[i]->Bcommand) {
        case 0: //Note
          MIDI.sendNoteOn(MUXBUTTONS[i]->Bvalue, 127, MUXBUTTONS[i]->Bchannel);
          break;
        case 1: //CC
          MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue, 127, MUXBUTTONS[i]->Bchannel);
          break;
        case 2: //Toggle
          if (MUXBUTTONS[i]->Btoggle == 0) {
            MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue, 127, MUXBUTTONS[i]->Bchannel);
            MUXBUTTONS[i]->Btoggle = 1;
          }
          else if (MUXBUTTONS[i]->Btoggle == 1) {
            MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue, 0, MUXBUTTONS[i]->Bchannel);
            MUXBUTTONS[i]->Btoggle = 0;
          }
          break;
      }
    }
    //  Button is not pressed
    if (message == 1) {
      switch (MUXBUTTONS[i]->Bcommand) {
        case 0:
          MIDI.sendNoteOff(MUXBUTTONS[i]->Bvalue, 0, MUXBUTTONS[i]->Bchannel);
          break;
        case 1:
          MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue, 0, MUXBUTTONS[i]->Bchannel);
          break;
      }
    }
  }
}
/*    Main Pot Loop   */

void Pots_AI() {
  for (int i = 0; i < NUMBER_POTS; i = i + 1) {
    byte potmessage = POTS[i]->getValue();
  
    if (potmessage != 255) MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage, POTS[i]->Pchannel);
  }
  
}
 