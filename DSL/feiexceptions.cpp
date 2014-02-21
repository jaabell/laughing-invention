#include <iostream>
#include <exception>
#include <string>

#include "feiexceptions.h"

using namespace std;


RunTimeException :: RunTimeException(string m) : msg(m)
{

}

const char* RunTimeException :: what() const throw()
{
    string completemessage = "FeiRunTimeError: ";
    completemessage += msg;
    return completemessage.c_str();
}


RunTimeException :: ~RunTimeException() throw()\
{

}
