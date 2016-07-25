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
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <algorithm>
#include <cctype>

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

struct compare_strings_class
{
    bool operator() (const string &a, const string &b) const
    {
        string alower(a);
        string blower(b);
        std::transform(a.begin(), a.end(), alower.begin(), (int(*)(int)) std::tolower);
        std::transform(b.begin(), b.end(), blower.begin(), (int(*)(int)) std::tolower);

        //cout << a << " (" << alower << ") comparing to ";
        //cout << b << " (" << blower << ")" << endl;

        return alower < blower;
    }
};
typedef std::set<string, compare_strings_class> lowerstring ;

void write_latex_highlighting(string filename, lowerstring const &keywords, lowerstring const &keywords2);
void write_geany_autocomplete(string filename, lowerstring commands);

int main()
{
    // Open files for reading and writing
    ifstream fid("feiparser.yy");
    ofstream out("available_commands.txt");
    ofstream keywords_list("keywords.list");

    // This defines the separator for keyword identification
    boost::char_separator<char> sep(" .");   //break down words by spaces and points using boost::tokenizer

    string line;
    string docstring;
    string docstring_no_braces;
    string docstring_no_keywords2;

    // Store identified keywords
    lowerstring keywords;
    lowerstring keywords2;
    lowerstring notkeywords;
    lowerstring commands;
    lowerstring::const_iterator iter;

    //These are excluded as keywords
    notkeywords.insert("\"");
    //notkeywords.insert("|");
    //notkeywords.insert(";");
    notkeywords.insert("#");
    notkeywords.insert("(");
    notkeywords.insert(")");
    notkeywords.insert("=");
    notkeywords.insert("<");
    notkeywords.insert(">");
    notkeywords.insert("[");
    notkeywords.insert("]");
    notkeywords.insert("{");
    notkeywords.insert("}");

    cout << "\n Creating FEI Automatic documentation file\n";
    cout << "===================================================================================\n";
    cout << "Writing output: \n\n";

    // Check if files have been correctly opened
    if (!fid.good())
    {
        cout << "Couldn't open feiparser.yy for reading \n";
    }

    if (!out.good())
    {
        cout << "Couldn't open available_commands.txt for writing \n";
    }

    if (!keywords_list.good())
    {
        cout << "Couldn't open keywords.list for writing \n";
    }

    //Start writing the geany format file
    while (!fid.eof())
    {
        getline(fid, line);
        int here;

        if ( (here = line.find("//!FEIDOC")) != string::npos) // Identify lines starting with //!FEIDOC
        {
            // Remove the "//!FEIDOC " at the beggining of the line and
            docstring = line.substr(here + 10, string::npos);

            docstring_no_braces = docstring;
            docstring_no_keywords2 = docstring;

            // Use braces ({}) to identify second order keywords. Also remove them from string
            size_t found;
            size_t i1 = string::npos;
            size_t i2 = string::npos;

            //cout << 1 << endl;
            try
            {
                while ( ((i1  = docstring_no_braces.find("[")) != string::npos) &
                        ((i2  = docstring_no_braces.find("]")) != string::npos ))
                {
                    keywords2.insert(docstring_no_braces.substr(i1 + 1, i2 - i1 - 1));
                    docstring_no_braces.erase(i1, 1);
                    docstring_no_braces.erase(i2 - 1, 1);
                }

                while ( ((i1  = docstring_no_keywords2.find("[")) != string::npos) &
                        ((i2  = docstring_no_keywords2.find("]")) != string::npos ))
                {
                    docstring_no_keywords2.erase(i1, i2 - i1);
                }
            }
            catch (...)
            {
                cout << "Error parsing line: " << endl;
                cout << line << endl;
            }

            //cout << 2 << endl;
            //while( (found = docstring_no_braces.find("|")) != string::npos )
            //    docstring_no_braces.erase(found,1);

            // Make sure all lines end with a semicolon, print a warning otherwise
            found = docstring_no_braces.find(";");

            if (found == string::npos)
            {
                cout << "  \n\n  Warning: The following line is not ended by semicolon!! \n";
                cout << "\"" << docstring_no_braces << "\"" << endl << endl;
            }

            commands.insert(docstring_no_braces);

            //now separate the sentence into words and add to dictionary
            tokenizer< char_separator<char> > wordsInCurrentSentence(docstring_no_keywords2, sep);

            //This uses boost "tokenizer" class to separate sentence into tokens at the white spaces and tabs.
            // Store each (key)word into a std::set (which only allows unique entries and is sorted)
            int nWordsInThisSentence = 0;
            BOOST_FOREACH(string thisWord, wordsInCurrentSentence)
            {
                // Check if the word is not a keyword
                int ok = 1;

                for (iter = notkeywords.begin(); iter != notkeywords.end(); iter++)
                {
                    found  = thisWord.find(*iter);

                    if (found != string::npos)
                    {
                        ok = 0;
                        break;
                    }
                }

                //If control is here, then the word must be a keyword.
                if (ok == 1)
                {
                    nWordsInThisSentence++;
                    found = thisWord.find(";");

                    if (found != string::npos)
                    {
                        thisWord.erase(found, 1);
                    }

                    keywords.insert(thisWord);
                }
            }
        }
    }

    //Remember to close files!!
    fid.close();

    //remove ; from keywords
    lowerstring::iterator it_erase_this = keywords.find(";");

    //cout << 3 << endl;
    if (it_erase_this != keywords.end())
    {
        keywords.erase(it_erase_this);
    }


    // Output command list
    for (iter = commands.begin(); iter != commands.end(); iter++)
    {
        out << *iter << endl;
    }

    out.close();

    // Output first and second order keywords
    for (iter = keywords.begin(); iter != keywords.end(); iter++)
    {
        keywords_list << *iter << endl;
    }

    keywords_list << endl << endl;

    for (iter = keywords2.begin(); iter != keywords2.end(); iter++)
    {
        keywords_list << *iter << endl;
    }

    keywords_list.close();


    // Output LaTeX lstlistings class file
    write_latex_highlighting("fei_latex_highlighting.tex", keywords, keywords2);

    //Output geany autocompletion file and syntax highlighting
    write_geany_autocomplete("fei.tags", commands);

    cout << "\n\n Output written to: available_commands.txt" << endl;
    cout << "===================================================================================\n\n";

    return 0;
}


// Specific syntax highlighting formats.

//\lstdefinelanguage{fei}
//{
//  morekeywords={one,two,three,four,five,six,seven,eight,nine,ten,eleven,twelve,o,clock,rock,around,the,tonight},
//  sensitive=true,
//  morecomment=[l]{//},
//  morecomment=[l]{#},
//  morestring=[b]",
//}
void write_latex_highlighting(string filename, lowerstring const &keywords, lowerstring const &keywords2)
{
    ofstream texfile(filename.c_str());

    if (!texfile.good())
    {
        cout << "Couldn't open " << filename << " for writing \n";
    }
    else
    {
        cout << filename << endl;
        texfile << "\\lstdefinelanguage{fei}" << endl;
        texfile << "{" << "morekeywords={";

        //Actually write the keywords
        unsigned int i = 1;

        for (lowerstring::const_iterator iter = keywords.begin(); iter != keywords.end(); iter++)
        {
            texfile << *iter;

            if (i++ < keywords.size())
            {
                texfile << ", ";
            }
        }

        texfile << "}," << endl;

        //Write second order keywords
        texfile << "morekeywords={[2] ";
        i = 1;

        for (lowerstring::const_iterator iter = keywords2.begin(); iter != keywords2.end(); iter++)
        {
            texfile << *iter;

            if (i++ < keywords2.size())
            {
                texfile << ", ";
            }
        }

        texfile << "}," << endl;


        texfile << "sensitive=true," << endl;
        texfile << "morecomment=[l]{//}," << endl;
        //texfile << "morecomment=[l]{#}," << endl;
        texfile << "morestring=[b]\"," << endl;
        texfile << "}" << endl;
    }

    texfile.close();
}

// Specific autocompletion files


void write_geany_autocomplete(string filename, lowerstring commands)
{
    ofstream geany_tags(filename.c_str());

    if (!geany_tags.good())
    {
        cout << "Couldn't open " << filename << " for writing \n";
    }

    {
        cout << filename << endl;
        geany_tags << "# format=pipe" << endl;

        for (lowerstring::const_iterator iter = commands.begin();
                iter != commands.end();
                iter++)
        {
            geany_tags << *iter << "|||" << endl;
        }
    }

    geany_tags.close();
}


