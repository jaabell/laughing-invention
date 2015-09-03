
//! Inputs: Nothing!
void define_numberer_rcm_for_analysis()
{
    RCM*  theRCM  = 0;
    theRCM = new RCM();

    if (theRCM == NULL)
    {
        cerr << "Error: (define_numberer_rcm_for_analysis) memory allocating for theRCM!" << endl;
        abort();
    }

    theNumberer  = new DOF_Numberer(*theRCM);

    if (theNumberer == NULL)
    {
        cerr << "Error: (define_numberer_rcm_for_analysis) memory allocating for theNumberer!" << endl;
        abort();
    }

};






