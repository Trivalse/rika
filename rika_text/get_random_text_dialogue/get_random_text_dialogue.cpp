// Obtain a random dialogue line in a text file -- A. Amiruddin -- 11/1/2017

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
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

//-------------------------------------------------------------------------------------------------
// Global variables

const string pszTextFilepath = "rika_text/rika_textpack/"; // Textpack folder relative to main

//*************************************************************************************************

string GetRandomTextDialogue(string pszTextFilename)
// Returns a random line in a text file.
{
    // Declare variables
    string pszTextDialogue = "";
    uint32_t uNoLines = 0;

    // Concatenate filepath and filename
    pszTextFilename = pszTextFilepath + pszTextFilename;

    // Open text file
    ifstream TextFile (pszTextFilename);

    // Count number of lines in text file
    while (getline(TextFile, pszTextDialogue))
    {
        uNoLines++;
    }

    TextFile.clear();
    TextFile.seekg(0, ios::beg);

    // Generate random number with max being uNoLines
    int32_t nRandomNumber = -1;

    // Set time as seed
    srand(time(0));

    // Get random number between 0-uNoLines
    nRandomNumber = rand()%uNoLines;
    if (nRandomNumber < 0)
    {
        cout << "Error generating random number" << endl;
        exit(1);
    }

    // Get <nRandomNumber>th line
    for (int32_t i = 0; i <= nRandomNumber; i++)
    {
        getline(TextFile, pszTextDialogue);
    }

    TextFile.close();

    return pszTextDialogue;
}

//*************************************************************************************************
