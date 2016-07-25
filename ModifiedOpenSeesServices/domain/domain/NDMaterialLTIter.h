///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral GPL-like license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Jose Abell, Boris Jeremic
// DATE:              December 2014
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#ifndef NDMaterialLTIter_h
#define NDMaterialLTIter_h

class NDMaterialLT;

class NDMaterialLTIter
{
    public:
        NDMaterialLTIter() {};
        virtual ~NDMaterialLTIter() {};

        virtual NDMaterialLT *operator()(void) = 0;

    protected:

    private:

};

#endif

