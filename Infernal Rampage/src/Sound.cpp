#include "Sound.h"

ISoundEngine* Sound::Engine;

void Sound::Initialize()
{
    if (!Engine)
        Engine = createIrrKlangDevice();
}

void Sound::Play(const char* file)
{
    Engine->play2D(file);
}

void Sound::PlayLoop(const char* file)
{
    Engine->play2D(file, true);
}
