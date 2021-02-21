
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// MCU specific configuration
#if defined (__AVR_ATtiny85__)
#  include <specific/tn85.h>
#endif

#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#ifndef F_PRESCALAR
#define F_PRESCALER 256
#endif
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif
#ifndef ONE_TICK_MS
#define ONE_TICK_MS (1.0/(F_CPU/F_PRESCALER))
#endif

#define C4 261
#define B3 246
#define D3 146
#define D4 293
#define E4 329
#define A3 220
#define D4 293
#define A4 440
#define F3 174
#define F4 349
#define Gb4 363
#define G4 391
#define A4 440
#define B4 493
#define E5 659
#define B5 987
#define G5 783
#define A5 880
#define D5 587
#define Gb5 739

#define Whole 4.0f
#define Half 2.0f
#define Quarter 1.0f
#define Eighth 0.5f
#define Sixteenth 0.25f

#define Note 1
#define Rest 2

struct note
{   
   note(int frequency, float duration, uint8_t type) : frequency(frequency), duration(duration), type(type) {}
   note(float duration, uint8_t type) : duration(duration), type(type) {}
   note(int frequency, uint8_t type) : frequency(frequency), duration(Half),type(type) {}
   note(int frequency) : frequency(frequency), duration(Half), type(Note) {}
   unsigned int frequency;
   float duration;
   uint8_t type;
};

struct melody {
  melody(note *notes, unsigned int numberOfNotes, unsigned int repeatCount, unsigned int bpm) : notes(notes),numberOfNotes(numberOfNotes), melodyRepeatCount(repeatCount), bpm(bpm) {}
  melody(note *notes, unsigned int numberOfNotes) : notes(notes),numberOfNotes(numberOfNotes), melodyRepeatCount(1), bpm(60) {}
  note *notes;
  uint8_t numberOfNotes;
  uint8_t melodyRepeatCount;
  int bpm;
};

class TinyTune
{
    public:
        TinyTune();
        void DisableOutput();
        void EnableOutput();
        void PlayNote(note note);
        void PlayMelody(melody melody);  
    private:
        void Init();
};
