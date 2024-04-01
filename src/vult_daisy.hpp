#pragma once
#include <daisy.h>

#include <algorithm>
#include <span>

#define vult2daisy(name)                                                       \
  class name {                                                                 \
   public:                                                                     \
    name() { name##_process_init(data); }                                      \
                                                                               \
    void Init() { name##_default(data); }                                      \
                                                                               \
    void NoteOn(daisy::NoteOnEvent event) {                                    \
      if (event.velocity != 0) {                                               \
        name##_noteOn(data, event.note, event.velocity, event.channel);        \
      } else {                                                                 \
        name##_noteOff(data, event.note, event.channel);                       \
      }                                                                        \
    }                                                                          \
                                                                               \
    void NoteOff(daisy::NoteOffEvent event) {                                  \
      name##_noteOff(data, event.note, event.channel);                         \
    }                                                                          \
                                                                               \
    void ControlChange(daisy::ControlChangeEvent event) {                      \
      name##_controlChange(data, event.control_number, event.value,            \
                           event.channel);                                     \
    }                                                                          \
                                                                               \
    void ProcessBlock(std::span<const float> input, std::span<float> output) { \
      std::transform(input.begin(), input.end(), output.begin(),               \
                     [this](float input) { return ProcessOne(input); });       \
    }                                                                          \
                                                                               \
    float ProcessOne(float input) { return name##_process(data, input); }      \
                                                                               \
   private:                                                                    \
    name##_process_type data;                                                  \
  }
