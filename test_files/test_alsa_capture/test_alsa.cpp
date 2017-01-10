// Test ALSA capture -- A. Amiruddin -- 27/12/2016
// Records and outputs a sound file (16000Hz, mono, signed 16 bit little-endian to work with pocketsphinx)

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

#include <alsa/asoundlib.h>
#include <fcntl.h>
#include <iostream>
#include <cstdlib>
#include <stdint.h>

using namespace std;

//-------------------------------------------------------------------------------------------------
// Global variables

const char* pszSoundFilename = "test.wav";

const uint32_t uRate = 16000;
const uint32_t uChannels = 1;

//*************************************************************************************************

int main()
{
    snd_pcm_t* Handle_SND;
    snd_pcm_hw_params_t* Parameters_SND;
    int32_t nError;

    // Open capture device
    nError = snd_pcm_open(&Handle_SND, "default", SND_PCM_STREAM_CAPTURE, 0);

    if (nError < 0)
    {
        cout << "unable to open pcm device: " << snd_strerror(nError) << endl;
        return EXIT_FAILURE;
    }

    // Open file
    int32_t nFileHandler = open(pszSoundFilename, O_WRONLY, S_IRWXO);

    // Allocate parameter object
    snd_pcm_hw_params_alloca(&Parameters_SND);

    // Fill with default values
    snd_pcm_hw_params_any(Handle_SND, Parameters_SND);

    // Set parameters
    snd_pcm_uframes_t Frames_SDL = 32;
    int32_t nDIR = 0;

    snd_pcm_hw_params_set_access(Handle_SND, Parameters_SND, SND_PCM_ACCESS_RW_INTERLEAVED);    // Interleaved mode
    snd_pcm_hw_params_set_format(Handle_SND, Parameters_SND, SND_PCM_FORMAT_S16_LE);            // Signed 16 bit little-endian
    snd_pcm_hw_params_set_channels(Handle_SND, Parameters_SND, uChannels);                              // 1 Channel
    snd_pcm_hw_params_set_rate_near(Handle_SND, Parameters_SND, &uRate, &nDIR);                 // 16000 rate
    snd_pcm_hw_params_set_period_size_near(Handle_SND, Parameters_SND, &Frames_SDL, &nDIR);     // Set frames

    nError = snd_pcm_hw_params(Handle_SND, Parameters_SND);                                     // Set parameters

    if (nError < 0)
    {
        cout << "unable to set hw parameters: " << snd_strerror(nError) << endl;
        return EXIT_FAILURE;
    }

    // Allocate memory for buffer to hold one period
    snd_pcm_hw_params_get_period_size(Parameters_SND, &Frames_SDL, &nDIR);     // Get frames from driver

    int32_t nSize = Frames_SDL * 2;                                           // Calculate size, 1 channel, 2 bytes/sample
    int16_t* nBuffer;
    nBuffer = (int16_t*) malloc(nSize);

    // Set to loop for 5 seconds
    uint32_t uLoops;

    snd_pcm_hw_params_get_period_time(Parameters_SND, &uRate, &nDIR);
    uLoops = 5000000 / uRate;

    // Record
    cout << "Capturing audio from device" << endl;

    while (uLoops > 0)
    {
        uLoops--;

        // Read from capture device
        nError = snd_pcm_readi(Handle_SND, nBuffer, Frames_SDL);

        if (nError == -EPIPE)
        {
            // Buffer overrun
            cout << "Overrun occurred" << endl;
            snd_pcm_prepare(Handle_SND);
        }

        else if (nError < 0)
        {
            // Read error
            cout << "Read error: %s\n" << snd_strerror(nError) << endl;
        }

        else if (nError != (int)Frames_SDL)
        {
            cout << "Short read: read " << nError << " Frames_SDL\n" << endl;
        }

        // Write to file
        nError = write(nFileHandler, nBuffer, nSize);

        if (nError != nSize)
        {
            cout << "Short write: wrote " << nError << " bytes" << endl;
        }

    }

    cout << "Finished capture" << endl;

    // Close device and tidy-up
    snd_pcm_drain(Handle_SND);
    snd_pcm_close(Handle_SND);
    free(nBuffer);
    close(nFileHandler);

    return 0;
}

//*************************************************************************************************
