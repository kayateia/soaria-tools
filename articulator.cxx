/** \file
*   Articulation / Keyswitch helper
*   Articulates the articulations.
*   kayateia@
*   Licence: Apache 2.0
*/

#include "./library/Midi.hxx"
#include "./library/Utils.hxx"

// metadata
string name = "ChannelSwitcher";
string description = "Converts MIDI channels into keyswitches for articulations";

// parameters
array<double> inputParameters(16 * 2);

array<string> inputParametersNames(16 * 2);

array<double> inputParametersMin(16 * 2, 0);
array<double> inputParametersMax(16 * 2);
array<double> inputParametersDefault(16 * 2, 0);
array<int> inputParametersSteps(16 * 2);
array<string> inputParametersFormats(16 * 2, ".0");
array<string> inputParametersEnums(16 * 2, "");

void initialize() {
    for (uint i=0; i<16; i++) {
        uint dispChannel = i + 1;
        inputParametersNames[i*2 + 0] = "Ch" + dispChannel + " octave";
        inputParametersNames[i*2 + 1] = "Ch" + dispChannel + " note";
        inputParametersMax[i*2 + 0] = 10;
        inputParametersMax[i*2 + 1] = 11;
        inputParametersSteps[i*2 + 0] = 11;
        inputParametersSteps[i*2 + 1] = 12;
        inputParametersEnums[i*2 + 1] = "C;C#;D;D#;E;F;F#;G;G#;A;A#;B";
    }
}

// local variables
array<int8> channelOctaves(16, 0);
array<int8> channelNotes(16, 0);
MidiEvent tempEvent; ///< defining temp object in the script to avoid allocations in time-critical processBlock function

/*void printEvent(const string& message, const MidiEvent& event) {
    print("midi: " + message + " " + event.byte0 + " " + event.byte1 + " " + event.byte2 + " " + event.byte3);
}*/

void processEvent(BlockData& data, const MidiEvent& event) {
    MidiEventType type = MidiEventUtils::getType(event);
    if (type == kMidiNoteOn) {
        uint8 channel = MidiEventUtils::getChannel(event) - 1;
        int8 ksNote = channelOctaves[channel] * 12 + channelNotes[channel];

        tempEvent = event;
        MidiEventUtils::setChannel(tempEvent, 1);
        MidiEventUtils::setNote(tempEvent, ksNote);
        //printEvent("sending ksOn", tempEvent);
        //print("That was ksNote " + (ksNote / 12) + "/" + (ksNote % 12));
        data.outputMidiEvents.push(tempEvent);
        MidiEventUtils::setType(tempEvent, kMidiNoteOff);
        //printEvent("sending ksOff", tempEvent);
        data.outputMidiEvents.push(tempEvent);
    }

    tempEvent = event;
    MidiEventUtils::setChannel(tempEvent, 1);
    //printEvent("sending other", tempEvent);
    //print("That was note " + MidiEventUtils::getNote(tempEvent));
    data.outputMidiEvents.push(event);
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
    for (uint i=0; i<16; i++) {
        channelOctaves[i] = KittyDSP::Utils::roundDoubleToInt(inputParameters[i * 2]);
        channelNotes[i] = KittyDSP::Utils::roundDoubleToInt(inputParameters[i * 2 + 1]);
    }
}
