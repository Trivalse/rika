// Rika main function -- A. Amiruddin -- 25/12/2016

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
#include <stdint.h>
#include <cstdlib>
#include <time.h>
#include <unistd.h>

#include "rika_voice/speech_recognition_wrapper/speech_recognition_wrapper.h"
#include "rika_voice/rika_voicepack/rika_voicepack_lib.h"
#include "rika_voice/sdl_mixer_wrapper/sdl_mixer_wrapper.h"

#include "rika_extra/find_keywords/find_keywords.h"

using namespace std;

//-------------------------------------------------------------------------------------------------
// Global variables

const uint32_t uCommandStateSeconds = 7;

//*************************************************************************************************

int main()
{
    // Initialise all modules and threads
    Initialise_SpeechRecognition();

    cout << "Initialised" << endl;

    PlayRandomDialogue(pszRikaVoice_Init, uNoRikaVoice_Init);

    // Main loop ------------------------------------------------------------------ MAIN LOOP START
    while(1)
    {
        // Idle state ------------------------------------------------------------- IDLE STATE
        string pszCommand = RecogniseSpeech();
        cout << "Word recognised : " << pszCommand << endl;

        // Rika called
        if (FindKeywords(pszCommand, "RIKA"))
        {
            // Print state status
            cout << "Command State" << endl;

            // Play 'called' sound
            PlayRandomDialogue(pszRikaVoice_Called, uNoRikaVoice_Called);


            time_t Timeup = time(0) + uCommandStateSeconds;

            while (time(0) < Timeup)
            {
                // Command state --------------------------------------------------- COMMAND STATE
                pszCommand = RecogniseSpeech();
                cout << "Word recognised : " << pszCommand << endl;

                // Say Hi
                if (FindKeywords(pszCommand, "SAY", "HI"))
                {
                    // Received command
                    cout << "Command received: Say hi" << endl;

                    // Add time in command state
                    Timeup += uCommandStateSeconds;

                    // Play 'Hi' dialogue
                    PlayRandomDialogue(pszRikaVoice_Hi, uNoRikaVoice_Hi);
                }

                // Going out
                else if (FindKeywords(pszCommand, "GOING", "OUT") || FindKeywords(pszCommand, "HEADING", "OUT"))
                {
                    // Received command
                    cout << "Command received: Going out" << endl;

                    // Play 'command get' dialogue
                    PlayRandomDialogue(pszRikaVoice_CommandGet, uNoRikaVoice_CommandGet);

                    // Do'going out' routine here (sleep computer, turn off lights for example)

                    break;
                }

                // Turn lights on
                else if (FindKeywords(pszCommand, "ON", "LIGHTS"))
                {
                    // Received command
                    cout << "Command received: Turn on lights" << endl;

                    // Add time in command state
                    Timeup += uCommandStateSeconds;

                    // Play 'get command' dialogue
                    PlayRandomDialogue(pszRikaVoice_CommandGet, uNoRikaVoice_CommandGet);

                    // Turn on lights here (for example)
                }

                // Turn lights off
                else if (FindKeywords(pszCommand, "OFF", "LIGHTS"))
                {
                    // Received command
                    cout << "Command received: Turn off lights" << endl;

                    // Add time in command state
                    Timeup += uCommandStateSeconds;

                    // Play 'get command' sound
                    PlayRandomDialogue(pszRikaVoice_CommandGet, uNoRikaVoice_CommandGet);

                    // Turn off lights here (for example)
                }

                // User says 'Thanks'
                else if (FindKeywords(pszCommand, "THANK", "YOU") || FindKeywords(pszCommand, "THANKS"))
                {
                    // Received command
                    cout << "Command received: Thanks!" << endl;

                    // Play 'Thanks' sound
                    PlayRandomDialogue(pszRikaVoice_Thanks, uNoRikaVoice_Thanks);

                    break;
                }

                // Return to Idle
                else if (FindKeywords(pszCommand, "NEVERMIND") || FindKeywords(pszCommand, "NOTHING") || FindKeywords(pszCommand, "ALRIGHT"))
                {
                    // Received command
                    cout << "Command received: Back to idle" << endl;

                    // Play 'idle return' sound
                    PlayRandomDialogue(pszRikaVoice_IdleReturn, uNoRikaVoice_IdleReturn);

                    break;
                }
            }

            cout << "Idle State" << endl;
        }
    }

    return EXIT_SUCCESS;
}

//*************************************************************************************************
