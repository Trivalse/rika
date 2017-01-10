// Reference list of rika_voicepack dialogue sets -- A. Amiruddin -- 25/12/2016

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

#ifndef RIKA_VOICEPACK_H_INCLUDED
#define RIKA_VOICEPACK_H_INCLUDED

//*************************************************************************************************
// Each dialogue set must contain a 'const char* pszRikaVoice_<name of set> = "<name of sound file without number and format>" '
// and a 'const uint32_t uNoRikaVoice_<name of set> = <number of sound files in set>'

// Say hi
const char* pszRikaVoice_Hi = "rika_voice_hi";
const uint32_t uNoRikaVoice_Hi = 2;

// Initialise
const char* pszRikaVoice_Init = "rika_voice_init";
const uint32_t uNoRikaVoice_Init = 1;

// Obtained command
const char* pszRikaVoice_CommandGet = "rika_voice_commandget";
const uint32_t uNoRikaVoice_CommandGet = 4;

// Called by user
const char* pszRikaVoice_Called = "rika_voice_called";
const uint32_t uNoRikaVoice_Called = 4;

// Return to idle
const char* pszRikaVoice_IdleReturn = "rika_voice_idlereturn";
const uint32_t uNoRikaVoice_IdleReturn = 1;

// Reply to "Thanks"
const char* pszRikaVoice_Thanks = "rika_voice_thanks";
const uint32_t uNoRikaVoice_Thanks = 3;

//*************************************************************************************************

#endif // RIKA_VOICEPACK_H_INCLUDED
