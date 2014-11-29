///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral LGPL (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     LGPL
// PROJECT:           Object Oriented Finite Element Program
//
// PURPOSE:           Domain Specific Language (DSL)
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Jose Abell, Boris Jeremic
//
// DATE:              Dec 2014

//
///////////////////////////////////////////////////////////////////////////////


int set_output_compression_level(int level)
{
#ifdef _PARALLEL_PROCESSING
    cout << "Warning! Output compresison not available in parallel as HDF5 (as of Dec 2014, version 1.8.12) does not support parallel filtered outputt. \n";
    cout << "     Please check HDF5 website for further information. \n";
    return 0;
#else
    return theDomain.setOutputCompressionLevel(level);
#endif
}

