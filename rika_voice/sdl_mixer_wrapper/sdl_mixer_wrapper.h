// Function declarations for sdl_mixer_wrapper -- A. Amiruddin -- 25/12/2016

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

#ifndef SDL_MIXER_WRAPPER_H_INCLUDED
#define SDL_MIXER_WRAPPER_H_INCLUDED

//*************************************************************************************************

void PlayRandomDialogue(const char* pszSoundFilename, const uint32_t uNoVoices);
// Plays a random dialogue in the dialogue set specified in pszRikaVoiceFilename.
// See "../rika_voicepack/rika_voicepack_lib.h" for list of dialogue sets available

void PlayDialogue(const char* pszSoundFilename);
// Plays sound file specified in pszSoundFilename.

//*************************************************************************************************

#endif // SDL_MIXER_WRAPPER_H_INCLUDED
