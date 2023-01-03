/** \file
*   Scales CC values
*   kayateia@
*   Licence: Apache 2.0
*/

#include "./library/Midi.hxx"
#include "./library/Utils.hxx"

// metadata
string name = "CCScaler";
string description = "Scales incoming CC values";

// parameters
array<double> inputParameters = { 1, 1.0, 0.0 };
array<string> inputParametersNames = { "CC Number", "Scale", "Bias" };
array<double> inputParametersMin = { 0, 0, -127 };
array<double> inputParametersMax = { 127, 10.0, 127 };
array<double> inputParametersDefault = { 1, 1.0, 0.0 };
array<int> inputParametersSteps = { 127, 0.1, 0.1 };
array<string> inputParametersFormats = { ".0", ".1", ".1" };

void initialize() {
}

// local variables
uint8 ccNumberExpected;
MidiEvent tempEvent; ///< defining temp object in the script to avoid allocations in time-critical processBlock function

/*void printEvent(const string& message, const MidiEvent& event) {
    print("midi: " + message + " " + event.byte0 + " " + event.byte1 + " " + event.byte2 + " " + event.byte3);
}*/

void processEvent(BlockData& data, const MidiEvent& event) {
    MidiEventType type = MidiEventUtils::getType(event);
    tempEvent = event;
    if (type == kMidiControlChange) {
        uint8 ccNumber = MidiEventUtils::getCCNumber(event);
        if (ccNumber == ccNumberExpected) {
            double value = MidiEventUtils::getCCValue(tempEvent);
            value = (value * inputParameters[1]) + inputParameters[2];
            if (value < 0) {
                value = 0;
            }
            if (value > 127) {
                value = 127;
            }
            MidiEventUtils::setCCValue(tempEvent, uint8(KittyDSP::Utils::roundDoubleToInt(value)));
        }
    }
    data.outputMidiEvents.push(tempEvent);
}

/* per-block processing function: called for every block with updated parameters values.
*
*/
void processBlock(BlockData& data) {
    // iterate on input MIDI events
    for (uint i=0; i<data.inputMidiEvents.length; i++) {
        processEvent(data, data.inputMidiEvents[i]);
    }
}

/* Update internal variables based on input parameters.
*
*/
void updateInputParametersForBlock(const TransportInfo@ info) {
    ccNumberExpected = KittyDSP::Utils::roundDoubleToInt(inputParameters[0]);
}
