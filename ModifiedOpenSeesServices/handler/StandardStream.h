/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

// $Revision: 1.2 $
// $Date: 2003/02/25 23:33:14 $
// $Source: /usr/local/cvs/OpenSees/SRC/handler/StandardStream.h,v $

#ifndef _StandardStream
#define _StandardStream

#include <iostream>

#include <fstream>
using std::ofstream;

class StandardStream : public ostream
{
    public:
        StandardStream();
        virtual ~StandardStream();

        int setFile(const char* fileName, openmode mode = OVERWRITE);
        int setPrecision(int precision);
        int setFloatField(floatField);
        int precision(int precision)
        {
            return 0;
        };
        int width(int width)
        {
            return 0;
        };

        ostream& write(const char* s, int n);
        ostream& write(const unsigned char* s, int n);
        ostream& write(const signed char* s, int n);
        ostream& write(const void* s, int n);
        ostream& operator<<(char c);
        ostream& operator<<(unsigned char c);
        ostream& operator<<(signed char c);
        ostream& operator<<(const char* s);
        ostream& operator<<(const unsigned char* s);
        ostream& operator<<(const signed char* s);
        ostream& operator<<(const void* p);
        ostream& operator<<(int n);
        ostream& operator<<(unsigned int n);
        ostream& operator<<(long n);
        ostream& operator<<(unsigned long n);
        ostream& operator<<(short n);
        ostream& operator<<(unsigned short n);
        ostream& operator<<(bool b);
        ostream& operator<<(double n);
        ostream& operator<<(float n);
        // ostream& operator<<(__omanip func);
        // ostream& operator<<(__manip func);
        // ostream& operator<<(streambuf*);
        // ostream& ends(ostream& outs);
        // ostream& flush(ostream& outs);
        // ostream& (ostream& outs);

    private:
        ofstream theFile;
        int fileOpen;
};

#endif
