// Wrapper functions for SDL_mixer -- A. Amiruddin -- 25/12/2016
// Requires SDL2 and SDL_mixer v2.0

// REVISION HISTORY
// None

//=================================================================================================
//    Copyright (C) 2016  Afeeq Amiruddin
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//=================================================================================================

#include <iostream>
#include <unistd.h>
#include <time.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "sdl_mixer_wrapper.h"
#include "../rika_voicepack/rika_voicepack_format.h"

using namespace std;

//*************************************************************************************************

void PlayRandomDialogue(const char* pszSoundFilename, const uint32_t uNoVoices)
// Designed to work when called from rika_main
// Plays a random dialogue in the dialogue set specified in pszRikaVoiceFilename.
// See "/rika_voicepack/rika_voicepack.h" for list of dialogue sets available
{
    // Define buffers for voice filename and number
    char szBuffer1[64] = "rika_voice/rika_voicepack/";
    char szBuffer2[64] = "";

    // Concatenate "rika_voicepack/" with voice filename
    strcat(szBuffer1, pszSoundFilename);

    // Generate random number with max being uNoVoices
    int32_t nRandomNumber = -1;

    // Set time as seed
    srand(time(0));

    // Get random number between 1-100
    nRandomNumber = rand()%uNoVoices +1;

    if (nRandomNumber < 0)
    {
        cout << "Error generating random number" << endl;
        exit(1);
    }

    // Turn nRandomNumber into string, store in szBuffer2
    if (sprintf(szBuffer2, "%u", nRandomNumber) < 0)
    {
        cout << "Error in sprintf" << endl;
        exit(1);
    }

    // Concatenate szBuffer1 with szBuffer2
    strcat(szBuffer1, szBuffer2);

    // Concatenate szBuffer1 with pszSoundFileFormat for final directory
    strcat(szBuffer1, pszSoundFileFormat);

    // Play sound file
    PlayDialogue(szBuffer1);

    return;
} // PlayRandomDialogue(pszSoundFilename, uNoVoices)

//-------------------------------------------------------------------------------------------------

void PlayDialogue(const char* pszSoundFilename)
// Plays sound file specified in pszSoundFilename.
{
    // Define music variable
    Mix_Music *gDialogue = NULL;

    // Open audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        cout << "Failed to initialise audio! Error = " << Mix_GetError() << endl;
        exit(1);
    }

    // Load music
    gDialogue = Mix_LoadMUS(pszSoundFilename);
    if (gDialogue == NULL)
    {
        cout << "Failed to load music! Error = " << Mix_GetError() << endl;
        exit(1);
    }

    // Play music
    if (Mix_PlayMusic(gDialogue, 0) < 0)
    {
        cout << "Unable to play music!" << Mix_GetError() << endl;
        exit(1);
    }

    // Do nothing until music stops playing
    while(Mix_PlayingMusic() != 0)
    {
        usleep(10000);
    }

    // Clean-up
    Mix_CloseAudio();
    Mix_FreeMusic(gDialogue);
    Mix_Quit();

    return;
} // PlayDialogue(pszSoundFilename)

//*************************************************************************************************
