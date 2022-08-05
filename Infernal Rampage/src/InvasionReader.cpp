#include "InvasionReader.h"

int InvasionReader::ReadNumber(FILE* f)
{
    int n = 0;
    char c;
    do
    {
        c = getc(f);
        if(c>=48 && c<=57)
        {
            n*=10;
            n+=(c-'0');
        }
    } while (c>=48 && c<=57);

    do
    {
        c = getc(f);
    } while (c != '\n');

    return n;
}

Invasion* InvasionReader::CurrentInvasion;

void InvasionReader::ReadInvasion(char* FileName)
{
    // The invasion information is stored here
    Invasion* ReturnValue = new Invasion();

    // Open the invasion file
    FILE* InvasionFile = fopen(FileName, "r");

    if (InvasionFile)
    {
        // Read invasion properties
        ReturnValue->ArchDemonHealth = ReadNumber(InvasionFile);
        ReturnValue->DemonHealth = ReadNumber(InvasionFile);
        ReturnValue->DemonProjectileRate = ReadNumber(InvasionFile);
        ReturnValue->DemonSpawnRate = ReadNumber(InvasionFile);
        ReturnValue->DemonSpeed = ReadNumber(InvasionFile);

        // Close the file
        fclose(InvasionFile);
    }
    else
    {
        puts("Invasion file not found!");
    }

    CurrentInvasion = ReturnValue;
}
