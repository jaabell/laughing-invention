/*

///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell
// DATE:               Jan 2012
// UPDATE HISTORY:     Full update history in git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////




2012 UCD CompGeoMech Group

Part of FEI parser.

This program takes the file available_commands.txt and generates C++ code to populate the command
suggestions used by the parser. This code is included within the command_suggestion class in compile
time.

Coded by Jose Abell

*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

//#include <../boost/boost/tokenizer.hpp>

using namespace std;
//using namespace boost;

int main()
{
    fstream commands_file("available_commands.txt", ios::in);
    fstream output_file("command_patterns.cpp", ios::out);

    string line;

    while (!commands_file.eof())
    {
        getline(commands_file, line);
        size_t here  = 0;
        here = line.find("\"", here);

        while ( here != string::npos )
        {
            //cout << "Before: " << line << endl;
            //cout << "here = " << here << endl;
            line.replace(here, 1, "\\\"");
            //cout << " After: " << line << endl << endl;
            here += 2;
            here  = line.find("\"", here);
        }

        output_file << "patterns->push_back(\"" << line << "\");" << endl;
    }

    commands_file.close();
    output_file.close();

    return 0;
}
