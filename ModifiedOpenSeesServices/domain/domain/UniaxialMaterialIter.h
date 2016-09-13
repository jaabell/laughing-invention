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
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              September 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#ifndef UniaxialMaterialIter_h
#define UniaxialMaterialIter_h

class UniaxialMaterial;

class UniaxialMaterialIter
{
    public:
        UniaxialMaterialIter() {};
        virtual ~UniaxialMaterialIter() {};
        virtual void reset(void)=0;
        virtual UniaxialMaterial* operator()(void) = 0;

    protected:

    private:

};

#endif

