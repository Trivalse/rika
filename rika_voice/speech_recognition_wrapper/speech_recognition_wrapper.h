// speech_recognition_wrapper function declarations -- A. Amiruddin -- 30/12/2016


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

#ifndef SPEECH_RECOGNITION_WRAPPER_H_INCLUDED
#define SPEECH_RECOGNITION_WRAPPER_H_INCLUDED

using namespace std;

//*************************************************************************************************

void Initialise_SpeechRecognition();
// Returns 0 on success, -1 on error

string RecogniseSpeech();
// Waits until user start speaking, then records until user stops speaking. Returns string of decoded speech.

void Clean_SpeechRecognition();
// Clean up. Returns 0 on success, -1 on error

//*************************************************************************************************

#endif // SPEECH_RECOGNITION_WRAPPER_H_INCLUDED
