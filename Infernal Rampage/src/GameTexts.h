#include <stdio.h>
#include <stdlib.h>
#include "GameResults.h"

class GameTexts
{
public:
    static void IntroText()
    {
        auto InvasionInfo = InvasionReader::GetCurrentInvasion();

        // Introduce the Archdemon
        // -----------------------------------------------------------
        printf("DEMON INVASION!\n");
        printf("Earth has been invaded once again by Satan's bloodthirsty demon legions.\n");
        printf("This Archdemon's destructive power has been measured as %d\n", InvasionInfo->GetArchDemonDestructivePower());
        printf("You must save Mankind!\n");
        // -----------------------------------------------------------

        printf("\n\n");

        // How to play
        // -----------------------------------------------------------
        printf("HOW TO PLAY\n");
        printf("Use the mouse to look around.\n");
        printf("Press the left mouse button to fire.\n");
        printf("Use the W A S D keys to move.\n");
        printf("Hold the left mouse button to lock on to a target.\n");
        // -----------------------------------------------------------

        printf("\n\n");
        printf("PRESS ENTER TO START THE FIGHT...\n");
    }

    static void VictoryText(GameResults Results)
    {
        printf("VICTORY\n");
        printf("Mankind has been saved!\n");
        printf("Your score is %d\n", Results.GetScore());
        printf("Surely you could beat stronger demons! Don't worry, they will be back...\n");
        printf("\n\n");
    }

    static void FailureText(GameResults Results)
    {
        printf("FAILURE\n");
        printf("You failed humanity!\n");
        printf("Hopefully the planet will be able to recover from this disaster.\n");
        printf("If the next invasion is weaker, maybe we have a chance to strike back...\n");
        printf("\n\n");
    }

    static void GameStatisticsText(Invasion* RecievedInvasion, GameResults Results)
    {
        printf("STATISTICS\n");
        printf("Archdemon's total destructive power: %d\n", RecievedInvasion->GetArchDemonDestructivePower());
        printf("Archdemon's health: %d\n", RecievedInvasion->ArchDemonHealth);
        printf("Lesser demons' health: %d\n", RecievedInvasion->DemonHealth);
        printf("Lesser demons' projectile rate: %d\n", RecievedInvasion->DemonProjectileRate);
        printf("Lesser demons' spawn rate: %d\n", RecievedInvasion->DemonSpawnRate);
        printf("Lesser demons' speed: %d\n", RecievedInvasion->DemonSpeed);
        printf("Play time: %f seconds\n", Results.PlayTime);
    }
};
