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
// PURPOSE:           C Binding Commands
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              September 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



int save_state_of_model_mysql_format(string databaseName, string host,
                                     string username, string password, unsigned int port,
                                     string socket)
{

    int commitTag = 0;

    const char* databaseName_char = databaseName.c_str();
    const char* host_char         = host.c_str();
    const char* username_char     = username.c_str();
    const char* password_char     = password.c_str();
    const char* socket_char       = socket.c_str();


    FE_Datastore* theDatabase  = 0;


    theDatabase = new MySqlDatastore(commitTag, databaseName_char, host_char, username_char, password_char, port, socket_char, theDomain, theBroker);


    if (theDatabase == NULL)
    {
        cerr << "Error: (save_state_of_model_mysql_format) memory allocation problem!" << endl;
        return -1;
    }



    if (theDatabase->commitState(commitTag) < 0)
    {
        cerr << "WARNING - database failed to commitState \n";
        return -1;
    }


    return 0;
};



