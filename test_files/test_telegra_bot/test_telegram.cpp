// Test telegram bot, and to obtain personal chat ID -- A. Amiruddin -- 15/01/2017

/*
 * Copyright (c) 2015 Oleg Morozenkov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// REVISION HISTORY
// Cleaned-up, commented and added printing of chat ID -- A. Amiruddin -- 15/01/2017

#include <signal.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <exception>

#include <tgbot/tgbot.h>

using namespace std;
using namespace TgBot;

//-------------------------------------------------------------------------------------------------
// Global variables

const char* pszBotToken = "INSERT BOT TOKEN"; // Insert bot token here

bool sigintGot = false;

//*************************************************************************************************

int main()
{
    // SIGINT handler
    signal(SIGINT, [](int s)
    {
        printf("SIGINT got");
        sigintGot = true;
    });

    // Initialise bot
    Bot bot(pszBotToken);

    // Initialise function on any message event
    bot.getEvents().onAnyMessage([&bot](Message::Ptr message)
    {
        // Wait a little
        sleep(1);

        string pszMessage = message->text.c_str();
        cout << "User wrote: " <<  pszMessage << endl;
        cout << "Chat ID of user = " << message->chat->id << endl;

        bot.getApi().sendMessage(message->chat->id, "Hello!");

        return;
    });

    // Long poll
    try
    {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());

        TgLongPoll longPoll(bot);

        while (!sigintGot)
        {
            printf("Long poll started\n");
            longPoll.start();
        }
    }

    catch (exception& e)
    {
        printf("error: %s\n", e.what());
    }

    return 0;
}

//*************************************************************************************************
