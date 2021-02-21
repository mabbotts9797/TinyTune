#include <specific/tn85.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <TinyTune.h>

volatile long timerOverflowCount = 0;
volatile long timerOverflowCountTarget = 1;
volatile bool playNextInSequence = false;

void Common_Initialise()
{
    SELECTED_OUTPUT_PIN_REGISTER |= (1 << SELECTED_OUTPUT_PIN);

    #if (SELECTED_OUTPUT_PIN == PB0)
        TCCR0A=(0<<COM0A1) | (1<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (1<<WGM01) | (0<<WGM00);
    #endif
    TCCR0B=(0<<WGM02) | (1<<CS02) | (0<<CS01) | (0<<CS00);
    // Enable Compare Match A Interrupt, this is used for delays
    TIMSK |= (1 << OCIE0A);
    TCNT0=0x00;
    OCR0B = 0x00;
    sei();
}

void Common_DisableOutput()
{
    TCCR0A &= ~(1 << COM0A0) & ~(1 << COM0A1);
}

void Common_EnableOutput()
{
    TCCR0A &= ~(1 << COM0A1);
    TCCR0A |= (1 << COM0A0);
}

void Common_PlayNote(note note)
{
    // Value calculated from rearranging the equation in terms of OCRnX, page 72 of ATtiny datasheet
    // https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf
    uint8_t ctcValue = ((F_CPU - (2 * F_PRESCALER * (unsigned long)note.frequency)) / (2 * F_PRESCALER * (unsigned long)note.frequency)) + 1;
    OCR0A= ctcValue;
}

void Common_PlayMelody(melody melody)
{
    timerOverflowCount = 0;
    timerOverflowCountTarget = 0;
    uint8_t melodyRepeat = melody.melodyRepeatCount;
    float oneBeatDuration = (60.0/melody.bpm);

    do 
    {    
        for( unsigned int i = 0; i < melody.numberOfNotes; i++ )
        {
            Common_DisableOutput();
        
            playNextInSequence = false;
            if (melody.notes[i].type == Note)
            {
                Common_EnableOutput();
                Common_PlayNote(melody.notes[i]);
            }


            double totalTicks = (double)(OCR0A * ONE_TICK_MS);
            timerOverflowCountTarget = (long)(oneBeatDuration * melody.notes[i].duration / totalTicks) / 2.0;
            while(playNextInSequence == false){ ;; }
        }
        melodyRepeat--;
        Common_DisableOutput();
    }
    while (melodyRepeat != 0); 
}


ISR(TIMER0_COMPA_vect)
{
  timerOverflowCount++;
  if (timerOverflowCount == timerOverflowCountTarget)
  {
    playNextInSequence = true;
    timerOverflowCountTarget = 0;
    timerOverflowCount = 0;
  }
}