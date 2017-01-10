// Plays sound file specified in pszMusicFilename -- A. Amiruddin -- 25/12/2016
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
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

using namespace std;

//-------------------------------------------------------------------------------------------------
// Global variables

const char* pszMusicFilename = "/home/pi/beat.wav";

//*************************************************************************************************

int main()
{
    // Define music variable
    Mix_Music *gDialogue = NULL;

    // Open audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        cout << "Failed to initialise audio! Error = " << Mix_GetError() << endl;
        return EXIT_FAILURE;
    }

    // Wait a little for speakers to properly turn on
    sleep(1);

    // Load music
    gDialogue = Mix_LoadMUS(pszMusicFilename);
    if (gDialogue == NULL)
    {
        cout << "Failed to load music! Error = " << Mix_GetError() << endl;
        return EXIT_FAILURE;
    }

    // Play music
    if (Mix_PlayMusic(gDialogue, 0) < 0)
    {
        cout << "Unable to play music!" << Mix_GetError() << endl;
    }

    cout << "Playing dialogue!" << endl;

    // Do nothing until music stops playing
    while(Mix_PlayingMusic() != 0)
    {

    }

    // Clean-up
    Mix_CloseAudio();
    Mix_FreeMusic(gDialogue);
    Mix_Quit();

    cout << "Test success!\n" << endl;
    return EXIT_SUCCESS;
} // main()

//*************************************************************************************************
