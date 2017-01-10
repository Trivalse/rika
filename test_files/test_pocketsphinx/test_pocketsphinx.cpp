// Test pocketsphinx -- A. Amiruddin -- 27/12/2016

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
#include <iostream>

using namespace std;

//-------------------------------------------------------------------------------------------------
// Global variables

const char* pszSoundFilename = "test.wav";

//*************************************************************************************************

int main()
{
    // Declare variables
    ps_decoder_t *ps;     // Decoder
    cmd_ln_t *Config;     // Configuration object
    FILE *FileHandler;

    // Initialise configuration object
    Config = cmd_ln_init(NULL, ps_args(), TRUE,
                         "-hmm", "model/en-us/en-us",
                         "-lm", "model/en-us/en-us.lm.bin",
                         "-dict", "model/en-us/cmudict-en-us.dict",
                         NULL);

    if (Config == NULL)
    {
        cout << "Failed to create Config object, see log for details\n" << endl;
        return EXIT_FAILURE;
    }

    // Initialise decoder
    ps = ps_init(Config);
    if (ps == NULL)
    {
        cout << "Failed to create recognizer, see log for details\n" << endl;
        return EXIT_FAILURE;
    }

    // Obtain file handler of sound file
    FileHandler = fopen(pszSoundFilename, "rb");
    if (FileHandler == NULL)
    {
        cout << "Unable to open audio file!\n" << endl;
        return EXIT_FAILURE;
    }

    // Start utterance
    if (ps_start_utt(ps) < 0)
    {
        cout << "Error!" << endl;
        return EXIT_FAILURE;
    }

    // Feed decoder with samples until end of file
    int16_t anBuffer[512];

    while (!feof(FileHandler))
    {
        size_t uNoSamples;
        uNoSamples = fread(anBuffer, 2, 512, FileHandler);
        if (ps_process_raw(ps, anBuffer, uNoSamples, FALSE, FALSE) < 0)
        {
            cout << "Error!" << endl;
            return EXIT_FAILURE;
        }
    }

    // Stop utterance
    if (ps_end_utt(ps) < 0)
    {
        cout << "Error!" << endl;
        return EXIT_FAILURE;
    }

    // Obtain hypothesis/recognised words and score
    const char* pszHypothesis;
    int32_t nScore;

    pszHypothesis = ps_get_hyp(ps, &nScore);
    cout << "Recognized: " << pszHypothesis << endl;

    // Tidy-up
    fclose(FileHandler);
    ps_free(ps);
    cmd_ln_free_r(Config);

    return EXIT_SUCCESS;
}

//*************************************************************************************************
