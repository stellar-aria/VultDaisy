#include <daisy_seed.h>

#include <algorithm>
#include <span>

#include "phasedist.h"
#include "vult_daisy.hpp"

vult2daisy(Phasedist);

using namespace daisy;

DaisySeed hw;
MidiUsbHandler midi;
Phasedist voice;

/** Function that gets called at a regular interval by the hardware to
 *  process, and/or generate audio signals
 */
void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out,
                   size_t size) {
  voice.ProcessBlock({&in[0][0], size}, {&out[0][0], size});
}

void HandleMidi() {
  /** Listen to MIDI for new changes */
  midi.Listen();

  /** When there are messages waiting in the queue... */
  while (midi.HasEvents()) {
    /** Pull the oldest one from the list... */
    auto msg = midi.PopEvent();
    switch (msg.type) {
      case NoteOn:
        voice.NoteOn(msg.AsNoteOn());
        break;

      case NoteOff:
        voice.NoteOff(msg.AsNoteOff());
        break;

      case ControlChange:
        voice.ControlChange(msg.AsControlChange());
        break;

      default:
        break;
    }
  }
}

int main(void) {
  /** Basic initialization of Daisy hardware */
  hw.Configure();
  hw.Init();

  /** Initialize USB Midi
   *  by default this is set to use the built in (USB FS) peripheral.
   *
   *  by setting midi_cfg.transport_config.periph =
   * MidiUsbTransport::Config::EXTERNAL the USB HS pins can be used (as FS)
   * for MIDI
   */
  MidiUsbHandler::Config midi_cfg;
  midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
  midi.Init(midi_cfg);

  /** Initialize our test tone */
  voice.Init();

  /** And start the audio callback */
  hw.StartAudio(AudioCallback);

  while (1) {
    HandleMidi();
  }
}
