#include <common/common.h>

TinyTune::TinyTune()
{
    Init();
}

void TinyTune::Init()
{    
    Common_Initialise();    
}
void TinyTune::DisableOutput()
{
    Common_DisableOutput();
}

void TinyTune::EnableOutput()
{
    Common_EnableOutput();
}

void TinyTune::PlayNote(note note)
{  
    Common_PlayNote(note);
}

void TinyTune::PlayMelody(melody melody)
{ 
    Common_PlayMelody(melody);
}

