// Wrapper for speech recognition -- A. Amiruddin -- 30/12/2016
// Uses ALSA and Pocketsphinx.

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

#include <pocketsphinx.h>
#include <alsa/asoundlib.h>
#include <string>
#include <cstdlib>
#include <stdint.h>
#include <iostream>
#include <unistd.h>

#include "speech_recognition_wrapper.h"

using namespace std;

//-------------------------------------------------------------------------------------------------
// Global variables

// Ensure that you put the files in the same /model/en-us directory as these
const char* pszAcousticModel = "rika_voice/speech_recognition_wrapper/model/en-us/en-us";          // Change this if using different language
const char* pszLanguageModel = "rika_voice/speech_recognition_wrapper/model/en-us/4980.lm";       // Change file number to file obtained from lm-tool
const char* pszDictionary = "rika_voice/speech_recognition_wrapper/model/en-us/4980.dic";         // Change file number to file obtained from lm-tool

static cmd_ln_t* Config = NULL;
static ps_decoder_t* PS_Decoder = NULL;
static snd_pcm_t *Handle_SND = NULL;
static snd_pcm_uframes_t Frames = 4096;
static int32_t nDIR = 0;
static snd_pcm_hw_params_t *Parameters_SND = NULL;

//*************************************************************************************************

void Initialise_SpeechRecognition()
// Returns 0 on success, -1 on error
{
    // Initialise configuration object
    Config = cmd_ln_init(NULL, ps_args(), TRUE,
                         "-hmm", pszAcousticModel,
                         "-lm", pszLanguageModel,
                         "-dict", pszDictionary,
                         "-logfn", "/dev/null",                                                 // Suppress log info from being sent to screen
                         NULL);

    if (Config == NULL)
    {
        cout << "Failed to create Config object, see log for details\n" << endl;
        exit(1);
    }

    // Initialise decoder
    PS_Decoder = ps_init(Config);

    if (PS_Decoder == NULL)
    {
        cout << "Failed to create recognizer, see log for details\n" << endl;
        exit(1);
    }

    // Initialise capture device
    if (snd_pcm_open(&Handle_SND, "default", SND_PCM_STREAM_CAPTURE, 0) < 0)
    {
        cout << "unable to open pcm device: " << endl;
        exit(1);
    }

    // Initialise hardware configuration
    uint32_t uRate = 16000;

    snd_pcm_hw_params_alloca(&Parameters_SND);                                                // Parameter object allocation
    snd_pcm_hw_params_any(Handle_SND, Parameters_SND);                                        // Add in default values
    snd_pcm_hw_params_set_access(Handle_SND, Parameters_SND, SND_PCM_ACCESS_RW_INTERLEAVED);  // Interleaved mode
    snd_pcm_hw_params_set_format(Handle_SND, Parameters_SND, SND_PCM_FORMAT_S16_LE);          // Signed 16-bit little-endian format
    snd_pcm_hw_params_set_channels(Handle_SND, Parameters_SND, 1);                            // Single channel
    snd_pcm_hw_params_set_rate_near(Handle_SND, Parameters_SND, &uRate, &nDIR);               // Set rate

    // Uncomment if needed
//    uint32_t uBufferTime = 0;
//    snd_pcm_hw_params_get_buffer_time_max(Parameters_SND, &uBufferTime, 0);
//    uint32_t uPeriodTime = uBufferTime/4;
//    snd_pcm_hw_params_set_period_time_near(Handle_SND, Parameters_SND, &uPeriodTime, 0);
//    snd_pcm_hw_params_set_buffer_time_near(Handle_SND, Parameters_SND, &uBufferTime, 0);

    snd_pcm_hw_params_set_period_size_near(Handle_SND, Parameters_SND, &Frames, &nDIR);       // Set frames
    snd_pcm_nonblock(Handle_SND, 1);                                                          // Open in non-blocking

    // Enable parameters
    if (snd_pcm_hw_params(Handle_SND, Parameters_SND) < 0)
    {
        cout << "unable to set hw parameters" << endl;
        exit(1);
    }

    return;
} // Initialise_SpeechRecognition()

//-------------------------------------------------------------------------------------------------

string RecogniseSpeech()
// Waits until user start speaking, then records until user stops speaking. Returns string of decoded speech.
{
    // Start capture and decode
    uint8_t uUtteranceFlag = FALSE;
    uint8_t uSpeechFlag = FALSE;
    int16_t nBuffer[4096];
    int32_t nFramesInBuffer = 0;
    const char* pszHypothesis = '\0';

    // Start utterance
    if (ps_start_utt(PS_Decoder) < 0)
    {
        cout << "Error starting utterance!" << endl;
        exit(1);
    }

    snd_pcm_prepare(Handle_SND);                           // Prepare recording

    uint32_t uNoTotalFrames = 0;

    while (1)
    {
        // Capture audio data to buffer
        nFramesInBuffer = snd_pcm_readi(Handle_SND, nBuffer, Frames);

        // No frames
        if (nFramesInBuffer == -EAGAIN)
        {
            nFramesInBuffer = 0;
        }

        // Input overrun
        else if (nFramesInBuffer == -EPIPE)
        {
            cout << "Input Overrun!" << endl;
            if (snd_pcm_prepare(Handle_SND) < 0)
            {
                cout << "Can't recover from underrun" << endl;
                exit(1);
            }

            nFramesInBuffer = 0;
        }

        // Sound driver asleep
        else if (nFramesInBuffer == -ESTRPIPE)
        {
            cout << "Resuming sound driver." << endl;
            while (nFramesInBuffer = snd_pcm_resume(Handle_SND) == -EAGAIN)
            {
                sleep(1);
            }

            if (nFramesInBuffer < 0)
            {
                if (snd_pcm_prepare(Handle_SND) < 0)
                {
                    cout << "Can't recover from underrun" << endl;
                    exit(1);
                }

                nFramesInBuffer = 0;
            }
        }

        // Error
        else if (nFramesInBuffer < 0)
        {
            cout << "Error in reading capture device!" << endl;
            nFramesInBuffer = 0;
        }

        // Send audio data to encoder
        if (ps_process_raw(PS_Decoder, nBuffer, nFramesInBuffer, FALSE, FALSE) < 0)
        {
            cout << "Error sending frames to decoder!" << endl;
            exit(1);
        }

        // Add to total of frames
        uNoTotalFrames += nFramesInBuffer;

        // Test to see if speech is being detected
        uSpeechFlag = ps_get_in_speech(PS_Decoder);

        // If speech has started and utterance flag is false
        if (uSpeechFlag && !uUtteranceFlag)
        {
            uUtteranceFlag = TRUE;
        }

        // If more than 150000 frames
        if (uNoTotalFrames >= 150000)
        {
            uSpeechFlag = FALSE;
            uUtteranceFlag = TRUE;
        }

        // If speech has ended and the utterance flag is true
        if (!uSpeechFlag && uUtteranceFlag)
        {

            // Stop utterance
            if (ps_end_utt(PS_Decoder) < 0)
            {
                cout << "Error stopping utterance!" << endl;
                exit(1);
            }

            // Stop recording and query pocketsphinx for decoded speech
            snd_pcm_drop(Handle_SND);
            pszHypothesis = ps_get_hyp(PS_Decoder, NULL);

            // NULL returned
            if (pszHypothesis == '\0')
            {
                pszHypothesis = "none";
            }

            break;
        }
    }

    return pszHypothesis;
} // RecogniseSpeech()

//-------------------------------------------------------------------------------------------------

void Clean_SpeechRecognition()
// Clean up.
{
    // Tidy-up
    ps_free(PS_Decoder);
    cmd_ln_free_r(Config);

    snd_pcm_drain(Handle_SND);
    snd_pcm_close(Handle_SND);

    return;
}  // Clean_SpeechRecognition()

//*************************************************************************************************
