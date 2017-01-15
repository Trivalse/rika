// Rika main function -- A. Amiruddin -- 25/12/2016

// REVISION HISTORY
// Added telegram bot and rika_text modules -- A. Amiruddin -- 15/01/2017

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
#include <signal.h>
#include <unistd.h>
#include <exception>
#include <thread>
#include <tgbot/tgbot.h>

#include "rika_voice/speech_recognition_wrapper/speech_recognition_wrapper.h"
#include "rika_voice/rika_voicepack/rika_voicepack_lib.h"
#include "rika_voice/sdl_mixer_wrapper/sdl_mixer_wrapper.h"

#include "rika_text/get_random_text_dialogue/get_random_text_dialogue.h"
#include "rika_text/rika_textpack/rika_textpack_lib.h"

#include "rika_extra/find_keywords/find_keywords.h"

using namespace std;
using namespace TgBot;

void RikaTelegramBot(); // Telegram bot thread

//-------------------------------------------------------------------------------------------------
// Global variables

const char* pszBotToken = "INSERT BOT TOKEN"; // Insert bot token here
const int64_t nPersonalChatID = 123456789; // Insert personal chat ID here

const uint32_t uCommandStateSeconds = 7;

//*************************************************************************************************

int main()
{
    // SIGINT handler
    signal(SIGINT, [](int s)
    {
        cout << "SIGINT got" << endl;

        //Clean-up
        Clean_SpeechRecognition();
        exit(0);
    });

    // Initialise all modules and threads
    Initialise_SpeechRecognition();

    thread tTelegramBot(RikaTelegramBot);
    tTelegramBot.detach();

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
                    
                    // Do 'leaving home' routine here

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

                    // Turn on lights here
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

                    // Turn off lights here
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

//---------------------------------------TELEGRAM BOT----------------------------------------------------------

void RikaTelegramBot()
// Telegram bot thread
{
    // Initialise bot
    Bot bot(pszBotToken);
    uint32_t uNoCatches = 0;

    bot.getApi().sendMessage(nPersonalChatID, "Initialised!");

    // Initialise function on any message
    bot.getEvents().onAnyMessage([&bot](Message::Ptr message)
    {
        // Wait a little
        sleep(1);

        // Unidentified user
        if (message->chat->id != nPersonalChatID)
        {
            cout << "Unidentified user, chat ID = " << message->chat->id << endl;

            bot.getApi().sendMessage(message->chat->id, GetRandomTextDialogue(pszRikaText_NoAuth));
            return;
        }

        string pszMessage = message->text.c_str();
        cout << "User wrote " <<  pszMessage << endl;

        // Coming home
        if (FindKeywords(pszMessage, "be home") || FindKeywords(pszMessage, "be back") || FindKeywords(pszMessage, "on my way home"))
        {
            bot.getApi().sendMessage(message->chat->id, GetRandomTextDialogue(pszRikaText_ComingHome));

            // Do 'coming home' routine here

            return;
        }

        // Thanks
        else if (FindKeywords(pszMessage, "Thanks") || FindKeywords(pszMessage, "Thank", "you"))
        {
            bot.getApi().sendMessage(message->chat->id, GetRandomTextDialogue(pszRikaText_Thanks));
            return;
        }

        // Rika called
        else if (FindKeywords(pszMessage, "Rika") || FindKeywords(pszMessage, "rika"))
        {
            bot.getApi().sendMessage(message->chat->id, GetRandomTextDialogue(pszRikaText_Called));
            return;
        }

        // Cancel
        else if (FindKeywords(pszMessage, "Nothing") || FindKeywords(pszMessage, "Nevermind"))
        {
            bot.getApi().sendMessage(message->chat->id, GetRandomTextDialogue(pszRikaText_Cancel));
            return;
        }
    });

    // Long poll
    while (uNoCatches < 10)
    {
        try
        {
            cout << "Bot username: " << bot.getApi().getMe()->username.c_str() << endl;

            TgLongPoll longPoll(bot);

            while (1)
            {
                cout << "Long poll started" << endl;
                longPoll.start();
            }
        }

        catch (exception& e)
        {
            cout << "error: " << e.what() << endl;
            uNoCatches++;
            sleep(60);
        }
    }

    cout << "Unable to connect to the internet for a long period" << endl;
    exit(1);
}

//*************************************************************************************************
