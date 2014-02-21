#ifndef LTENSORDISPLAY_CPP_INCLUDED
#define LTENSORDISPLAY_CPP_INCLUDED

#include "LTensorDisplay.h"




namespace LTensorDisplay
{
    char fillchar = ' ';
    int width = 15;
    int smallwidth = 1;
    int precision = 4;

    void print(DTensor1 a, string name, string msg, int baseindex)
    {
        int dim1 = a.get_dim1();
        //Display message if existent (leave whitespace if not)
        cout << msg << endl;

        for (int ii = 0; ii < dim1; ii++)
        {
            cout << name
                 << "("
                 << std::right << std::noshowpos
                 << std::setw(smallwidth) << ii + baseindex
                 << ")=";
            cout << std::scientific
                 << std::setfill(fillchar)
                 << std::left << std::showpos
                 << std::setw(width)
                 << std::setprecision(precision)
                 << a(ii)
                 << endl;
        }
    }

    void print(DTensor2 a, string name, string msg, int baseindex)
    {
        int dim1 = a.get_dim1();
        int dim2 = a.get_dim2();
        //Display message if existent (leave whitespace if not)
        cout << msg << endl;

        for (int ii = 0; ii < dim1; ii++)
        {
            for (int kk = 0; kk < dim2; kk++)
            {
                cout << name
                     << "("
                     << std::right << std::noshowpos
                     << std::setw(smallwidth) << ii + baseindex << ","
                     << std::setw(smallwidth) << kk + baseindex
                     << ")=";
                cout << std::scientific
                     << std::setfill(fillchar)
                     << std::left << std::showpos
                     << std::setw(width)
                     << std::setprecision(precision)
                     << a(ii, kk) << " ";
            }

            cout << endl;
        }
    }

    void print(DTensor3 a, string name, string msg, int baseindex)
    {
        int dim1 = a.get_dim1();
        int dim2 = a.get_dim2();
        int dim3 = a.get_dim3();
        //Display message if existent (leave whitespace if not)
        cout << msg << endl;

        for (int ii = 0; ii < dim1; ii++)
        {
            for (int kk = 0; kk < dim2; kk++)
            {
                for (int ll = 0; ll < dim3; ll++)
                {
                    cout << name
                         << "("
                         << std::right << std::noshowpos
                         << std::setw(smallwidth) << ii + baseindex << ","
                         << std::setw(smallwidth) << kk + baseindex << ","
                         << std::setw(smallwidth) << ll + baseindex
                         << ")=";
                    cout << std::scientific
                         << std::setfill(fillchar)
                         << std::left << std::showpos
                         << std::setw(width)
                         << std::setprecision(precision)
                         << a(ii, kk, ll) << " ";
                }

                cout << endl;
            }
        }
    }

    void print(DTensor4 a, string name, string msg, int baseindex)
    {
        int dim1 = a.get_dim1();
        int dim2 = a.get_dim2();
        int dim3 = a.get_dim3();
        int dim4 = a.get_dim4();
        //Display message if existent (leave whitespace if not)
        cout << msg << endl;

        for (int ii = 0; ii < dim1; ii++)
        {
            for (int kk = 0; kk < dim2; kk++)
            {
                for (int ll = 0; ll < dim3; ll++)
                {
                    for (int jj = 0; jj < dim4; jj++)
                    {
                        cout << name
                             << "("
                             << std::right << std::noshowpos
                             << std::setw(smallwidth) << ii + baseindex << ","
                             << std::setw(smallwidth) << kk + baseindex << ","
                             << std::setw(smallwidth) << ll + baseindex << ","
                             << std::setw(smallwidth) << jj + baseindex
                             << ")=";
                        cout << std::scientific
                             << std::setfill(fillchar)
                             << std::left << std::showpos
                             << std::setw(width)
                             << std::setprecision(precision)
                             << a(ii, kk, ll, jj) << " ";
                    }

                    cout << endl;
                }
            }
        }
    }

};
#endif // LTENSORDISPLAY_CPP_INCLUDED
