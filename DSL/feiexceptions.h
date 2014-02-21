#ifndef FeiExceptions
#define FeiExceptions

#include <iostream>
#include <exception>
#include <string>

using namespace std;

class RunTimeException: public exception
{
    public:
        RunTimeException(string m);
        virtual const char* what() const throw();
        virtual ~RunTimeException() throw(); // Default constructor generates a looser throw specifier

    private:
        string msg;
};

#endif // FeiExceptions
