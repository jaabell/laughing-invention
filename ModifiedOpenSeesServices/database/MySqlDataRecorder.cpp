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
// PURPOSE:           Mysql data recorder
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              January 2013
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


#include <MySqlDataRecorder.h>

// #include <mysqld_error.h> .. use the following #define instead
#define ER_TABLE_EXISTS_ERROR 1050


MySqlDataRecorder::MySqlDataRecorder(const char* databaseName,
                                     const char* host,
                                     const char* user,
                                     const char* passwd,
                                     unsigned int port,
                                     const char* socket,
                                     Domain& theDomain,
                                     FEM_ObjectBroker& theObjectBroker)
    : FE_Datastore(theDomain, theObjectBroker),
      connection(true), query(0), sizeQuery(0)
{

    unsigned int clientFlag = 0;

    // initialise the mysql structure
    mysql_init(&mysql);

    // connect to the server & database
    if (mysql_real_connect(&mysql, host, user, passwd, databaseName, port, socket, clientFlag) == NULL)
    {

        // connect to the server & see if can link to database, or create a new one if one does not exist
        if (mysql_real_connect(&mysql, host, user, passwd, NULL, port, socket, clientFlag) == NULL)
        {
            cerr.flush() << "MySqlDataRecorder::MySqlDataRecorder() - could not connect to server\n";
            cerr.flush() << mysql_error(&mysql) << endln;
            connection = false;

        }
        else
        {

            // link to the database,
            if (mysql_select_db(&mysql, databaseName) != 0)
            {

                // if no database exists, try creating one
                if (this->createDatabase(databaseName) != 0)
                {
                    connection = false;
                    mysql_close(&mysql);
                    cerr.flush() << "MySqlDataRecorder::MySqlDataRecorder() - could not open the database\n";
                    cerr.flush() << mysql_error(&mysql) << endln;
                }
            }
        }
    }
}



MySqlDataRecorder::~MySqlDataRecorder()
{
    if (connection == true)
    {
        mysql_close(&mysql);
    }

    if (query != 0)
    {
        delete [] query;
    }
}



int
MySqlDataRecorder::createDatabase(const char* projectName)
{

    delete [] query;
    int sizeQuery = 512;

    query = new char [sizeQuery];


    // create the database
    sprintf(query, "CREATE DATABASE %s", projectName);

    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "MySqlDataRecorder::createDatabase() - failed to receive vector from MySQL database";
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        return -3;
    }

    // link to the database,
    if (mysql_select_db(&mysql, projectName) != 0)
    {
        cerr.flush() << "MySqlDataRecorder::createDatabase() - could not set the database\n";
        return -2;
    }


    // done
    return 0;
}


//=====================================================================================
int
MySqlDataRecorder::createDisplacementTables(int StepNumber)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }


    delete [] query;
    int sizeQuery = 520;

    query = new char [sizeQuery];


    ////////////////////////////////////////
    // converting integer to char
    string stepnumber;
    stringstream out;
    out << StepNumber;
    stepnumber = out.str();
    ////////////////////////////////////////


    string displacementVector = "NodalDisplacementStepNumber" + stepnumber;
    const char* vectortableName = displacementVector.c_str();


    sprintf(query,
            "CREATE TABLE %s ( Node_Number INT NOT NULL, \
DOF1 DOUBLE NOT NULL, \
DOF2 DOUBLE NOT NULL, \
DOF3 DOUBLE NOT NULL, \
DOF4 DOUBLE NOT NULL, \
DOF5 DOUBLE NOT NULL, \
DOF6 DOUBLE NOT NULL, \
DOF7 DOUBLE NOT NULL)", vectortableName);


    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "MySqlDataRecorder::createDisplacementTables() - failed to create table";
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        exit(-1);
    }


    return 0;
}



//=====================================================================================
int
MySqlDataRecorder::createLineElementForceTables(int StepNumber)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }


    delete [] query;
    int sizeQuery = 1000;

    query = new char [sizeQuery];


    ////////////////////////////////////////
    // converting integer to char
    string stepnumber;
    stringstream out;
    out << StepNumber;
    stepnumber = out.str();
    ////////////////////////////////////////


    string forceVector = "LineElementForcesStepNumber" + stepnumber;
    const char* forcetableName = forceVector.c_str();


    sprintf(query,
            "CREATE TABLE %s ( Element_Number INT NOT NULL, \
Force_1 DOUBLE NOT NULL, \
Force_2 DOUBLE NOT NULL, \
Force_3 DOUBLE NOT NULL, \
Force_4 DOUBLE NOT NULL, \
Force_5 DOUBLE NOT NULL, \
Force_6 DOUBLE NOT NULL, \
Force_7 DOUBLE NOT NULL, \
Force_8 DOUBLE NOT NULL, \
Force_9 DOUBLE NOT NULL, \
Force_10 DOUBLE NOT NULL, \
Force_11 DOUBLE NOT NULL, \
Force_12 DOUBLE NOT NULL)", forcetableName);


    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "MySqlDataRecorder::createLineElementForceTables() - failed to create table";
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        exit(-1);
    }


    return 0;
}


//=====================================================================================
int
MySqlDataRecorder::create8GPBrickStressTables(int StepNumber)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }


    delete [] query;
    int sizeQuery = 6000;

    query = new char [sizeQuery];


    ////////////////////////////////////////
    // converting integer to char
    string stepnumber;
    stringstream out;
    out << StepNumber;
    stepnumber = out.str();
    ////////////////////////////////////////


    string stresstable = "Brick8GPStressStepNumber" + stepnumber;
    const char* stresstablename = stresstable.c_str();


    sprintf(query,
            "CREATE TABLE %s ( Element_Number INT NOT NULL, \
Stress_1_1 DOUBLE NOT NULL, \
Stress_1_2 DOUBLE NOT NULL, \
Stress_1_3 DOUBLE NOT NULL, \
Stress_1_4 DOUBLE NOT NULL, \
Stress_1_5 DOUBLE NOT NULL, \
Stress_1_6 DOUBLE NOT NULL, \
Stress_2_1 DOUBLE NOT NULL, \
Stress_2_2 DOUBLE NOT NULL, \
Stress_2_3 DOUBLE NOT NULL, \
Stress_2_4 DOUBLE NOT NULL, \
Stress_2_5 DOUBLE NOT NULL, \
Stress_2_6 DOUBLE NOT NULL, \
Stress_3_1 DOUBLE NOT NULL, \
Stress_3_2 DOUBLE NOT NULL, \
Stress_3_3 DOUBLE NOT NULL, \
Stress_3_4 DOUBLE NOT NULL, \
Stress_3_5 DOUBLE NOT NULL, \
Stress_3_6 DOUBLE NOT NULL, \
Stress_4_1 DOUBLE NOT NULL, \
Stress_4_2 DOUBLE NOT NULL, \
Stress_4_3 DOUBLE NOT NULL, \
Stress_4_4 DOUBLE NOT NULL, \
Stress_4_5 DOUBLE NOT NULL, \
Stress_4_6 DOUBLE NOT NULL, \
Stress_5_1 DOUBLE NOT NULL, \
Stress_5_2 DOUBLE NOT NULL, \
Stress_5_3 DOUBLE NOT NULL, \
Stress_5_4 DOUBLE NOT NULL, \
Stress_5_5 DOUBLE NOT NULL, \
Stress_5_6 DOUBLE NOT NULL, \
Stress_6_1 DOUBLE NOT NULL, \
Stress_6_2 DOUBLE NOT NULL, \
Stress_6_3 DOUBLE NOT NULL, \
Stress_6_4 DOUBLE NOT NULL, \
Stress_6_5 DOUBLE NOT NULL, \
Stress_6_6 DOUBLE NOT NULL, \
Stress_7_1 DOUBLE NOT NULL, \
Stress_7_2 DOUBLE NOT NULL, \
Stress_7_3 DOUBLE NOT NULL, \
Stress_7_4 DOUBLE NOT NULL, \
Stress_7_5 DOUBLE NOT NULL, \
Stress_7_6 DOUBLE NOT NULL, \
Stress_8_1 DOUBLE NOT NULL, \
Stress_8_2 DOUBLE NOT NULL, \
Stress_8_3 DOUBLE NOT NULL, \
Stress_8_4 DOUBLE NOT NULL, \
Stress_8_5 DOUBLE NOT NULL, \
Stress_8_6 DOUBLE NOT NULL )", stresstablename);


    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "\nMySqlDataRecorder::create8nodeBrickStressTables() - failed to create table";
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        return -3;
    }


    return 0;
}



//=====================================================================================
int
MySqlDataRecorder::create27GPBrickStressTables(int StepNumber)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }


    delete [] query;
    int sizeQuery = 6000;

    query = new char [sizeQuery];


    ////////////////////////////////////////
    // converting integer to char
    string stepnumber;
    stringstream out;
    out << StepNumber;
    stepnumber = out.str();
    ////////////////////////////////////////


    string stresstable = "Brick27GPStressStepNumber" + stepnumber;
    const char* stresstablename = stresstable.c_str();


    sprintf(query,
            "CREATE TABLE %s ( Element_Number INT NOT NULL, \
Stress_1_1 DOUBLE NOT NULL, \
Stress_1_2 DOUBLE NOT NULL, \
Stress_1_3 DOUBLE NOT NULL, \
Stress_1_4 DOUBLE NOT NULL, \
Stress_1_5 DOUBLE NOT NULL, \
Stress_1_6 DOUBLE NOT NULL, \
Stress_2_1 DOUBLE NOT NULL, \
Stress_2_2 DOUBLE NOT NULL, \
Stress_2_3 DOUBLE NOT NULL, \
Stress_2_4 DOUBLE NOT NULL, \
Stress_2_5 DOUBLE NOT NULL, \
Stress_2_6 DOUBLE NOT NULL, \
Stress_3_1 DOUBLE NOT NULL, \
Stress_3_2 DOUBLE NOT NULL, \
Stress_3_3 DOUBLE NOT NULL, \
Stress_3_4 DOUBLE NOT NULL, \
Stress_3_5 DOUBLE NOT NULL, \
Stress_3_6 DOUBLE NOT NULL, \
Stress_4_1 DOUBLE NOT NULL, \
Stress_4_2 DOUBLE NOT NULL, \
Stress_4_3 DOUBLE NOT NULL, \
Stress_4_4 DOUBLE NOT NULL, \
Stress_4_5 DOUBLE NOT NULL, \
Stress_4_6 DOUBLE NOT NULL, \
Stress_5_1 DOUBLE NOT NULL, \
Stress_5_2 DOUBLE NOT NULL, \
Stress_5_3 DOUBLE NOT NULL, \
Stress_5_4 DOUBLE NOT NULL, \
Stress_5_5 DOUBLE NOT NULL, \
Stress_5_6 DOUBLE NOT NULL, \
Stress_6_1 DOUBLE NOT NULL, \
Stress_6_2 DOUBLE NOT NULL, \
Stress_6_3 DOUBLE NOT NULL, \
Stress_6_4 DOUBLE NOT NULL, \
Stress_6_5 DOUBLE NOT NULL, \
Stress_6_6 DOUBLE NOT NULL, \
Stress_7_1 DOUBLE NOT NULL, \
Stress_7_2 DOUBLE NOT NULL, \
Stress_7_3 DOUBLE NOT NULL, \
Stress_7_4 DOUBLE NOT NULL, \
Stress_7_5 DOUBLE NOT NULL, \
Stress_7_6 DOUBLE NOT NULL, \
Stress_8_1 DOUBLE NOT NULL, \
Stress_8_2 DOUBLE NOT NULL, \
Stress_8_3 DOUBLE NOT NULL, \
Stress_8_4 DOUBLE NOT NULL, \
Stress_8_5 DOUBLE NOT NULL, \
Stress_8_6 DOUBLE NOT NULL, \
Stress_9_1 DOUBLE NOT NULL, \
Stress_9_2 DOUBLE NOT NULL, \
Stress_9_3 DOUBLE NOT NULL, \
Stress_9_4 DOUBLE NOT NULL, \
Stress_9_5 DOUBLE NOT NULL, \
Stress_9_6 DOUBLE NOT NULL, \
Stress_10_1 DOUBLE NOT NULL, \
Stress_10_2 DOUBLE NOT NULL, \
Stress_10_3 DOUBLE NOT NULL, \
Stress_10_4 DOUBLE NOT NULL, \
Stress_10_5 DOUBLE NOT NULL, \
Stress_10_6 DOUBLE NOT NULL, \
Stress_11_1 DOUBLE NOT NULL, \
Stress_11_2 DOUBLE NOT NULL, \
Stress_11_3 DOUBLE NOT NULL, \
Stress_11_4 DOUBLE NOT NULL, \
Stress_11_5 DOUBLE NOT NULL, \
Stress_11_6 DOUBLE NOT NULL, \
Stress_12_1 DOUBLE NOT NULL, \
Stress_12_2 DOUBLE NOT NULL, \
Stress_12_3 DOUBLE NOT NULL, \
Stress_12_4 DOUBLE NOT NULL, \
Stress_12_5 DOUBLE NOT NULL, \
Stress_12_6 DOUBLE NOT NULL, \
Stress_13_1 DOUBLE NOT NULL, \
Stress_13_2 DOUBLE NOT NULL, \
Stress_13_3 DOUBLE NOT NULL, \
Stress_13_4 DOUBLE NOT NULL, \
Stress_13_5 DOUBLE NOT NULL, \
Stress_13_6 DOUBLE NOT NULL, \
Stress_14_1 DOUBLE NOT NULL, \
Stress_14_2 DOUBLE NOT NULL, \
Stress_14_3 DOUBLE NOT NULL, \
Stress_14_4 DOUBLE NOT NULL, \
Stress_14_5 DOUBLE NOT NULL, \
Stress_14_6 DOUBLE NOT NULL, \
Stress_15_1 DOUBLE NOT NULL, \
Stress_15_2 DOUBLE NOT NULL, \
Stress_15_3 DOUBLE NOT NULL, \
Stress_15_4 DOUBLE NOT NULL, \
Stress_15_5 DOUBLE NOT NULL, \
Stress_15_6 DOUBLE NOT NULL, \
Stress_16_1 DOUBLE NOT NULL, \
Stress_16_2 DOUBLE NOT NULL, \
Stress_16_3 DOUBLE NOT NULL, \
Stress_16_4 DOUBLE NOT NULL, \
Stress_16_5 DOUBLE NOT NULL, \
Stress_16_6 DOUBLE NOT NULL, \
Stress_17_1 DOUBLE NOT NULL, \
Stress_17_2 DOUBLE NOT NULL, \
Stress_17_3 DOUBLE NOT NULL, \
Stress_17_4 DOUBLE NOT NULL, \
Stress_17_5 DOUBLE NOT NULL, \
Stress_17_6 DOUBLE NOT NULL, \
Stress_18_1 DOUBLE NOT NULL, \
Stress_18_2 DOUBLE NOT NULL, \
Stress_18_3 DOUBLE NOT NULL, \
Stress_18_4 DOUBLE NOT NULL, \
Stress_18_5 DOUBLE NOT NULL, \
Stress_18_6 DOUBLE NOT NULL, \
Stress_19_1 DOUBLE NOT NULL, \
Stress_19_2 DOUBLE NOT NULL, \
Stress_19_3 DOUBLE NOT NULL, \
Stress_19_4 DOUBLE NOT NULL, \
Stress_19_5 DOUBLE NOT NULL, \
Stress_19_6 DOUBLE NOT NULL, \
Stress_20_1 DOUBLE NOT NULL, \
Stress_20_2 DOUBLE NOT NULL, \
Stress_20_3 DOUBLE NOT NULL, \
Stress_20_4 DOUBLE NOT NULL, \
Stress_20_5 DOUBLE NOT NULL, \
Stress_20_6 DOUBLE NOT NULL, \
Stress_21_1 DOUBLE NOT NULL, \
Stress_21_2 DOUBLE NOT NULL, \
Stress_21_3 DOUBLE NOT NULL, \
Stress_21_4 DOUBLE NOT NULL, \
Stress_21_5 DOUBLE NOT NULL, \
Stress_21_6 DOUBLE NOT NULL, \
Stress_22_1 DOUBLE NOT NULL, \
Stress_22_2 DOUBLE NOT NULL, \
Stress_22_3 DOUBLE NOT NULL, \
Stress_22_4 DOUBLE NOT NULL, \
Stress_22_5 DOUBLE NOT NULL, \
Stress_22_6 DOUBLE NOT NULL, \
Stress_23_1 DOUBLE NOT NULL, \
Stress_23_2 DOUBLE NOT NULL, \
Stress_23_3 DOUBLE NOT NULL, \
Stress_23_4 DOUBLE NOT NULL, \
Stress_23_5 DOUBLE NOT NULL, \
Stress_23_6 DOUBLE NOT NULL, \
Stress_24_1 DOUBLE NOT NULL, \
Stress_24_2 DOUBLE NOT NULL, \
Stress_24_3 DOUBLE NOT NULL, \
Stress_24_4 DOUBLE NOT NULL, \
Stress_24_5 DOUBLE NOT NULL, \
Stress_24_6 DOUBLE NOT NULL, \
Stress_25_1 DOUBLE NOT NULL, \
Stress_25_2 DOUBLE NOT NULL, \
Stress_25_3 DOUBLE NOT NULL, \
Stress_25_4 DOUBLE NOT NULL, \
Stress_25_5 DOUBLE NOT NULL, \
Stress_25_6 DOUBLE NOT NULL, \
Stress_26_1 DOUBLE NOT NULL, \
Stress_26_2 DOUBLE NOT NULL, \
Stress_26_3 DOUBLE NOT NULL, \
Stress_26_4 DOUBLE NOT NULL, \
Stress_26_5 DOUBLE NOT NULL, \
Stress_26_6 DOUBLE NOT NULL, \
Stress_27_1 DOUBLE NOT NULL, \
Stress_27_2 DOUBLE NOT NULL, \
Stress_27_3 DOUBLE NOT NULL, \
Stress_27_4 DOUBLE NOT NULL, \
Stress_27_5 DOUBLE NOT NULL, \
Stress_27_6 DOUBLE NOT NULL)", stresstablename);


    //   cout << endl << query << endl;

    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "\nMySqlDataRecorder::create27GPnodeBrickStressTables() - failed to create table";
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        return -3;
    }


    return 0;
}


//=====================================================================================
int
MySqlDataRecorder::sendquery(int commitTag, char* Domainquery, ChannelAddress* theAddress)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }


    if (mysql_query(&mysql, Domainquery) != 0)
    {
        cerr.flush() << "\nMySqlDataRecorder::sendNodalDisplacements - failed to send the Vector to MySQL database";
        exit(-1);
    }



    return 0;
}



//=====================================================================================


double
MySqlDataRecorder::restoreNodalDisplacement(int NodeNumber, int dof, int stepNumber)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }


    ////////////////////////////////////////
    // converting integer to char
    string stepnumber;
    stringstream out;
    out << stepNumber;
    stepnumber = out.str();
    ////////////////////////////////////////


    string displacementID = "NodalDisplacementStepNumber" + stepnumber;
    const char* vectortableName = displacementID.c_str();



    // check that query string is big enough to hold the data, if not enlarge
    int sizeData = 7 * sizeof(double);

    if ((query == 0) || ((sizeData * 2 + 256) > sizeQuery)) // *2 for extra space mysql needs
    {
        if (query != 0)
        {
            delete [] query;
        }

        sizeQuery = 2 * sizeData + 256;
        query = new char [sizeQuery];

        if (query == 0)
        {
            cerr.flush() << "MySqlDataRecorder::restoreNodalDisplacement - out of memory creating query of size";
            cerr.flush() << sizeQuery << endln;
            return -2;
        }
    }



    // fetch the results from the database
    MYSQL_RES* results;
    MYSQL_ROW row;


    ////////////////////////////////////////
    // converting integer to char
    string dofnumber;
    stringstream out2;
    out2 << dof;
    dofnumber = out2.str();
    ////////////////////////////////////////


    string dofnumber_column = "DOF" + dofnumber;
    const char* dof_number_restore = dofnumber_column.c_str();


    sprintf(query, "Select %s FROM %s WHERE Node_Number=%d",
            dof_number_restore, vectortableName, NodeNumber);


    // execute the SELECT query
    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "MySqlDataRecorder::restoreNodalDisplacement() - failed to receive vector from MySQL database";
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        return -3;
    }


    results = mysql_store_result(&mysql);

    if (results == NULL)
    {
        // no vector stored in db with these keys
        cerr.flush() << "MySqlDataRecorder::restoreNodalDisplacement - no data in database for Vector with NodeNumber, cTag: ";
        cerr.flush() << NodeNumber << endln;
        return -4;
    }

    row = mysql_fetch_row(results);

    if (row == NULL)
    {
        // no vector stored in db with these keys
        cerr.flush() << "MySqlDataRecorder::restoreNodalDisplacement - no data in database for Vector with NodeNumber, cTag: ";
        cerr.flush() << NodeNumber << endln;
        mysql_free_result(results);
        return -5;
    }


    cout << "mysql dof value is: " << (*row) << endl;

    double displacementValue = atof(*row);


    mysql_free_result(results);

    return displacementValue;
}

//=====================================================================================


