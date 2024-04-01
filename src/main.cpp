#include <daisy_seed.h>

#include <algorithm>
#include <span>

#include "hid/midi.h"
#include "phasedist.h"

using namespace daisy;

class PhaseDist {
 public:
  PhaseDist() { Phasedist_process_init(data); }

  void Init() { Phasedist_default(data); }

  // Handles note on events
  void NoteOn(NoteOnEvent event) {
    // If the velocity is larger than zero, means that is turning on
    if (event.velocity != 0) {
      Phasedist_noteOn(data, event.note, event.velocity, event.channel);
    } else {
      Phasedist_noteOff(data, event.note, event.channel);
    }
  }

  // Handles note off events
  void NoteOff(NoteOffEvent event) {
    Phasedist_noteOff(data, event.note, event.channel);
  }

  // Handles control change events
  void ControlChange(ControlChangeEvent event) {
    Phasedist_controlChange(data, event.control_number, event.value,
                            event.channel);
  }

  void ProcessBlock(std::span<const float> input, std::span<float> output) {
    std::transform(input.begin(), input.end(), output.begin(),
                   [this](float input) { return ProcessOne(input); });
  }

  float ProcessOne(float input) { return Phasedist_process(data, input); }

 private:
  Phasedist_process_type data;
};

DaisySeed hw;
MidiUsbHandler midi;
PhaseDist voice;

/** Function that gets called at a regular interval by the hardware to
 *  process, and/or generate audio signals
 */
void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out,
                   size_t size) {
  voice.ProcessBlock({in[0], size}, {out[0], size});
}

int main(void) {
  /** Basic initialization of Daisy hardware */
  hw.Configure();
  hw.Init();

  /** Initialize USB Midi
   *  by default this is set to use the built in (USB FS) peripheral.
   *
   *  by setting midi_cfg.transport_config.periph =
   * MidiUsbTransport::Config::EXTERNAL the USB HS pins can be used (as FS) for
   * MIDI
   */
  MidiUsbHandler::Config midi_cfg;
  midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
  midi.Init(midi_cfg);

  /** Initialize our test tone */
  voice.Init();

  /** And start the audio callback */
  hw.StartAudio(AudioCallback);
  while (1) {
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
}
