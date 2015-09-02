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




#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "command_suggestion.h"

#include <../boost/boost/tokenizer.hpp>
#include <../boost/boost/foreach.hpp>

using namespace std;
using namespace boost;




#define DEFAULT_NUMBER_OF_SUGGESTIONS 5

CommandSuggestion::CommandSuggestion()
{
    patterns = new CmdPattern();
    patterns->clear();

#include "command_patterns.cpp"

    number_of_suggestions = DEFAULT_NUMBER_OF_SUGGESTIONS;
}

CommandSuggestion::~CommandSuggestion()
{
    delete patterns;
}

string CommandSuggestion::Suggest(string cmd)
{
    //
    vector<size_t> scores(patterns->size(), 0);
    multimap<size_t, string&, greater<size_t> > ranking;

    size_t maxscore = 0;
    // This defines the separator for keyword identification
    boost::char_separator<char> sep(" ");   //break down words by spaces using boost::tokenizer
    tokenizer< char_separator<char> > tokens_in_cmd(cmd, sep);


    for (size_t i = 0; i < patterns->size(); i++)
    {
        string pattern = (*patterns)[i];
        tokenizer< char_separator<char> > tokens_in_pattern(pattern, sep);
        tokenizer< char_separator<char> >::const_iterator it1 = tokens_in_cmd.begin();
        tokenizer< char_separator<char> >::const_iterator it2 = tokens_in_pattern.begin();

        for (; it1 != tokens_in_cmd.end() && it2 != tokens_in_pattern.end(); it1++, it2++ )
        {
            if (*it1 == *it2)
            {
                scores[i]++;
            }
        }

        if (scores[i] > maxscore)
        {
            maxscore = scores[i];
        }
    }

    for (size_t i = 0; i < patterns->size(); i++)
    {
        ranking.insert(pair<size_t, string&>(scores[i], (*patterns)[i]));
    }

    string suggestion = "";


    multimap<size_t, string&>::const_iterator it = ranking.begin();

    while (it->first == maxscore)
    {
        if (maxscore == 1)
        {
            tokenizer< char_separator<char> > tokens_in_suggestion(it->second, sep);
            tokenizer< char_separator<char> >::const_iterator suggestion_it = tokens_in_suggestion.begin();
            suggestion += "\n" + *(suggestion_it++) + " " +  *suggestion_it + " ...";
        }
        else if (maxscore > 1)
        {
            suggestion += "\n" + it->second;
        }

        it++;
    }

    if (maxscore > 0)
    {
        cout << "\nDid you mean? \n" << suggestion << endl;
    }
    else
    {
        suggestion = "-- no suggestions --";
    }

    return suggestion;
}

string CommandSuggestion::Suggest(string cmd, int nsuggest)
{
    number_of_suggestions = nsuggest;
    string ret = this->Suggest(cmd);
    number_of_suggestions = DEFAULT_NUMBER_OF_SUGGESTIONS;
    return ret;
}

//int main()
//{
//    CommandSuggestion cmd_advisor;
//    string input = " ";
//
//    while(input.length() > 0)
//    {
//        cout << "Input a command: ";
//        getline(cin, input);
//        string suggestion = cmd_advisor.Suggest(input);
//    }
//
//    cout << "\nThank you!\n\n";
//    return 0;
//}
