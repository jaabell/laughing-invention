#ifndef Command_suggestion_H
#define Command_suggestion_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef vector<string> CmdPattern;
typedef vector<string>::const_iterator CmdPattern_iterator;

typedef vector<CmdPattern> CmdPatternList;
typedef vector<CmdPattern>::const_iterator CmdPatternList_iterator;

class CommandSuggestion
{
    public:
        CommandSuggestion();
        ~CommandSuggestion();

        string Suggest(string cmd);
        string Suggest(string cmd, int nsuggest);

    private:
        CmdPattern* patterns;
        string cmd;
        int number_of_suggestions;

};

#endif // Command_suggestion_H
