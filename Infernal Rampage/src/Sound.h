// A biblioteca irrKlang foi utilizada para lidar com áudio.

#pragma once

// At the beginning, we need to include the irrKlang headers (irrKlang.h) and
// the iostream headers needed to print text to the console.
#include <stdio.h>
#include <irrKlang.h>

// include console I/O methods (conio.h for windows)
#include <conio.h>

// Also, we tell the compiler to use the namespaces 'irrklang'.
// All classes and functions of irrKlang can be found in the namespace 'irrklang'.
// If you want to use a class of the engine,
// you'll have to type an irrklang:: before the name of the class.
// For example, to use the ISoundEngine, write: irrklang::ISoundEngine. To avoid having
// to put irrklang:: before of the name of every class, we tell the compiler that
// we use that namespaces here.

using namespace irrklang;

class Sound
{
private:
	static ISoundEngine* Engine;

public:
    static void Initialize();

    static void Play(const char* file);
    static void PlayLoop(const char* file);
};
