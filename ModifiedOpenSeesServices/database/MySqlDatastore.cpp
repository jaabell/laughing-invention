/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 2002, The Regents of the University of California    **
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


//**********************************************************************
// Major changes by Nima Tafazzoli (September 2012):
// 1. Adding SendnDarray and RecvnDarray functions
// 2. Replacing mysql_escape_string by mysql_real_escape_string
//    (suggested for new version of mysql)
// 3. sendDisplacementsID
// 4. sendDisplacementsVector
// 5. recvDisplacementsVector
// 6. recvDisplacementsID
// 7. createDisplacementTables
// 8. getCommitTag
//**********************************************************************


#include <MySqlDatastore.h>

// #include <mysqld_error.h> .. use the following #define instead
#define ER_TABLE_EXISTS_ERROR 1050


#define MAX_BLOB_SIZE 16777215


MySqlDatastore::MySqlDatastore(const char* projectName,
                               Domain& theDomain,
                               FEM_ObjectBroker& theObjectBroker,
                               int run)
    : FE_Datastore(theDomain, theObjectBroker), dbTag(0), dbRun(run),
      connection(true), query(0), sizeQuery(0), sizeColumnString(0)
{
    // initialise the mysql structure
    mysql_init(&mysql);

    // connect to the server
    if (mysql_real_connect(&mysql, NULL, NULL, NULL, NULL, 0, "/var/lib/mysql/mysql.sock", 0) == NULL)
    {

        cerr.flush() << "MySqlDatastore::MySqlDatastore() - could not connect to server\n";
        cerr.flush() << mysql_error(&mysql) << endln;
        connection = false;

    }
    else
    {

        // link to the database,
        if (mysql_select_db(&mysql, projectName) != 0)
        {

            // if no database exists, try creating one
            if (this->createDatabase(projectName) != 0)
            {
                connection = false;
                mysql_close(&mysql);
                cerr.flush() << "MySqlDatastore::MySqlDatastore() - could not open the database\n";
                cerr.flush() << mysql_error(&mysql) << endln;
            }
        }
    }

}



MySqlDatastore::MySqlDatastore(int commitTag,
                               const char* databaseName,
                               const char* host,
                               const char* user,
                               const char* passwd,
                               unsigned int port,
                               const char* socket,
                               Domain& theDomain,
                               FEM_ObjectBroker& theObjectBroker,
                               int run)
    : FE_Datastore(theDomain, theObjectBroker), dbTag(0), dbRun(run),
      connection(true), query(0), sizeQuery(0), sizeColumnString(0), CommitTag(commitTag)
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
            cerr.flush() << "MySqlDatastore::MySqlDatastore() - could not connect to server\n";
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
                    cerr.flush() << "MySqlDatastore::MySqlDatastore() - could not open the database\n";
                    cerr.flush() << mysql_error(&mysql) << endln;
                }
            }
        }
    }
}


MySqlDatastore::~MySqlDatastore()
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
MySqlDatastore::sendMsg(int dataTag, int commitTag,
                        const Message&,
                        ChannelAddress* theAddress)
{
    cerr.flush() << "MySqlDatastore::sendMsg() - not yet implemented\n";
    return -1;
}

int
MySqlDatastore::receiveMsg(int dataTag, int commitTag,
                        Message&,
                        ChannelAddress* theAddress)
{
    cerr.flush() << "MySqlDatastore::receiveMsg() - not yet implemented\n";
    return -1;
}



int
MySqlDatastore::sendMatrix(int dbTag, int commitTag,
                           const Matrix& theMatrix,
                           ChannelAddress* theAddress)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }

    // check that the data will fit, MySQL has a limit on blob sizes
    int sizeData = theMatrix.dataSize * sizeof(double);

    if (sizeData > MAX_BLOB_SIZE)
    {
        cerr.flush() << "MySqlDatastore::sendMatrix - matrix too big to send to MySQL databse, enlarge BLOBS!";
        return  -2;
    }

    // check that query string is big enough to hold the data, if not enlarge
    if ((query == 0) || ((sizeData * 2 + 254) > sizeQuery)) // *2 for extra space MySQL may need
    {
        if (query != 0)
        {
            delete [] query;
        }

        sizeQuery = 2 * sizeData + 254; // 254 for the INSERT INTO Vec... blah blah
        query = new char [sizeQuery];

        if (query == 0)
        {
            cerr.flush() << "MySqlDatastore::sendMatrix - out of memory creating query of size";
            cerr.flush() << sizeQuery << endln;
            return -2;
        }
    }

    //
    // to send the data the database we do the following:
    // 1. try to INSERT the data into the database
    // 2. if INSERT fails we have to reformulate the query to UPDATE the existing data,
    //    as the row given by dbTag, commitTag and vectorSize may already already exist.
    //

    // form the INSERT query
    sprintf(query, "INSERT INTO Matrices VALUES (%d, %d, %d,'", dbTag, commitTag, theMatrix.dataSize);
    char* p = query + strlen(query);
    p += mysql_real_escape_string(&mysql, p, (char*)theMatrix.data, sizeData);  // add matrix values in binary form
    strcpy(p, "')");

    // execute the INSERT query
    if (mysql_query(&mysql, query) != 0)
    {

        //
        // if INSERT fails we reformulate query and try an UPDATE
        //

        // formulate the query
        sprintf(query, "UPDATE Matrices SET data='");
        char* p = query + strlen(query);
        p += mysql_real_escape_string(&mysql, p, (char*)theMatrix.data, sizeData);
        sprintf(p, "' WHERE dbTag=%d AND commitTag=%d AND size=%d", dbTag, commitTag, theMatrix.dataSize);

        // invoke the query on the database
        if (mysql_query(&mysql, query) != 0)
        {
            cerr.flush() << "MySqlDatastore::sendMatrix() - failed to send the Matrix to MySQL database";
            cerr.flush() << endln << mysql_error(&mysql) << endln;
            return -3;
        }
    }

    return 0;
}

int
MySqlDatastore::receiveMatrix(int dbTag, int commitTag,
                           Matrix& theMatrix,
                           ChannelAddress* theAddress)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }

    // check that query string is big enough to hold the data, if not enlarge
    int sizeData = theMatrix.dataSize * sizeof(double);

    if ((query == 0) || ((sizeData * 2 + 256) > sizeQuery)) // *2 for extra space mysql needs
    {
        if (query != 0)
        {
            delete [] query;
        }

        sizeQuery = 2 * sizeData + 256; // 256 for the SLECECT data FROM ... blah blah
        query = new char [sizeQuery];

        if (query == 0)
        {
            cerr.flush() << "MySqlDatastore::receiveMatrix - out of memory creating query of size";
            cerr.flush() << sizeQuery << endln;
            return -2;
        }
    }

    //
    // to receive the data the database we do the following:
    // 1. use SELECT to receive the data from the database
    // 2. fetch the results from the server and copy to the Matrices data area
    // 3. clean up the MYSQL_RES datastructure
    //
    // NOTE: probably using a malloc() and free() under the covers for this, could be
    // very expensive .. IS THERE ANOTHER WAY TO DO THIS, i.e. SAVE A MYSQL_RES FOR
    // NUMEROUS USES??

    // form the SELECT query
    sprintf(query, "Select data FROM Matrices WHERE dbTag=%d AND commitTag=%d AND size=%d",
            dbTag, commitTag, theMatrix.dataSize);

    // execute the SELECT query
    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "MySqlDatastore::receiveMatrix() - failed to receive Matrix from MySQL database";
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        return -3;
    }

    // fetch the results from the database
    MYSQL_RES* results;
    MYSQL_ROW row;
    results = mysql_store_result(&mysql);

    if (results == NULL)
    {
        // no matrix stored in db with these keys
        cerr.flush() << "MySqlDatastore::receiveMatrix - no data in database for Matrix with dbTag, cTag: ";
        cerr.flush() << dbTag << ", " << commitTag << endln;
        return -4;
    }

    row = mysql_fetch_row(results);

    if (row == NULL)
    {
        // no matrix stored in db with these keys
        cerr.flush() << "MySqlDatastore::receiveMatrix - no data in database for Matrix with dbTag, cTag: ";
        cerr.flush() << dbTag << ", " << commitTag << endln;
        mysql_free_result(results);
        return -5;
    }

    // place the results into the matrices double array
    double* data = (double*)row[0];

    for (int i = 0; i < theMatrix.dataSize; i++)
    {
        theMatrix.data[i] = data[i];
    }


    // free the MYSQL_RES structure
    mysql_free_result(results);

    return 0;
}



int
MySqlDatastore::sendVector(int dbTag, int commitTag,
                           const Vector& theVector,
                           ChannelAddress* theAddress)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }

    // check that the data will fit, MySQL has a limit on blob sizes
    int sizeData = theVector.sz * sizeof(double);

    if (sizeData > MAX_BLOB_SIZE)
    {
        cerr.flush() << "MySqlDatastore::sendVector - vector too big to send to MySQL databse, enlarge BLOBS!";
        return  -2;
    }

    // check that query string is big enough to hold the data, if not enlarge
    if ((query == 0) || ((sizeData * 2 + 254) > sizeQuery)) // *2 for extra space MySQL may need
    {
        if (query != 0)
        {
            delete [] query;
        }

        sizeQuery = 2 * sizeData + 254; // 254 for the INSERT INTO Vec... blah blah
        query = new char [sizeQuery];

        if (query == 0)
        {
            cerr.flush() << "MySqlDatastore::sendVector - out of memory creating query of size";
            cerr.flush() << sizeQuery << endln;
            return -2;
        }
    }

    //
    // to send the data the database we do the following:
    // 1. try to INSERT the data into the database
    // 2. if INSERT fails we have to reformulate the query to UPDATE the existing data,
    //    as the row given by dbTag, commitTag and vectorSize may already already exist.
    //

    // form the INSERT query
    sprintf(query, "INSERT INTO Vectors VALUES (%d, %d, %d,'", dbTag, commitTag, theVector.sz);
    char* p = query + strlen(query);
    p += mysql_real_escape_string(&mysql, p, (char*)theVector.theData, sizeData);  // add vector values in binary form
    strcpy(p, "')");

    // execute the INSERT query
    if (mysql_query(&mysql, query) != 0)
    {


        //
        // if INSERT fails we reformulate query and try an UPDATE
        //

        // formulate the query
        sprintf(query, "UPDATE Vectors SET data='");
        char* p = query + strlen(query);
        p += mysql_real_escape_string(&mysql, p, (char*)theVector.theData, sizeData);
        sprintf(p, "' WHERE dbTag=%d AND commitTag=%d AND size=%d", dbTag, commitTag, theVector.sz);

        // invoke the query on the database
        if (mysql_query(&mysql, query) != 0)
        {
            cerr.flush() << "MySqlDatastore::sendVector() - failed to send the Vector to MySQL database";
            cerr.flush() << endln << mysql_error(&mysql) << endln;
            return -3;
        }
    }

    return 0;
}

int
MySqlDatastore::receiveVector(int dbTag, int commitTag,
                           Vector& theVector,
                           ChannelAddress* theAddress)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }

    // check that query string is big enough to hold the data, if not enlarge
    int sizeData = theVector.sz * sizeof(double);

    if ((query == 0) || ((sizeData * 2 + 256) > sizeQuery)) // *2 for extra space mysql needs
    {
        if (query != 0)
        {
            delete [] query;
        }

        sizeQuery = 2 * sizeData + 256; // 256 for the SLECECT data FROM ... blah blah
        query = new char [sizeQuery];

        if (query == 0)
        {
            cerr.flush() << "MySqlDatastore::receiveVector - out of memory creating query of size";
            cerr.flush() << sizeQuery << endln;
            return -2;
        }
    }

    //
    // to receive the data the database we do the following:
    // 1. use SELECT to receive the data from the database
    // 2. fetch the results from the server and copy to the Vectors data area
    // 3. clean up the MYSQL_RES datastructure
    //
    // NOTE: probably using a malloc() and free() under the covers for this, could be
    // very expensive .. IS THERE ANOTHER WAY TO DO THIS, i.e. SAVE A MYSQL_RES FOR
    // NUMEROUS USES??

    // form the SELECT query
    sprintf(query, "Select data FROM Vectors WHERE dbTag=%d AND commitTag=%d AND size=%d",
            dbTag, commitTag, theVector.sz);

    // execute the SELECT query
    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "MySqlDatastore::receiveVector() - failed to receive vector from MySQL database";
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        return -3;
    }

    // fetch the results from the database
    MYSQL_RES* results;
    MYSQL_ROW row;
    results = mysql_store_result(&mysql);

    if (results == NULL)
    {
        // no vector stored in db with these keys
        cerr.flush() << "MySqlDatastore::receiveVector - no data in database for Vector with dbTag, cTag: ";
        cerr.flush() << dbTag << ", " << commitTag << endln;
        return -4;
    }

    row = mysql_fetch_row(results);

    if (row == NULL)
    {
        // no vector stored in db with these keys
        cerr.flush() << "MySqlDatastore::receiveVector - no data in database for Vector with dbTag, cTag: ";
        cerr.flush() << dbTag << ", " << commitTag << endln;
        mysql_free_result(results);
        return -5;
    }

    // place the results into the vectors double array
    double* data = (double*)row[0];

    for (int i = 0; i < theVector.sz; i++)
    {
        theVector.theData[i] = data[i];
    }


    // free the MYSQL_RES structure
    mysql_free_result(results);

    return 0;
}


int
MySqlDatastore::sendID(int dbTag, int commitTag,
                       const ID& theID,
                       ChannelAddress* theAddress)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }

    // check that the data will fit, MySQL has a limit on blob sizes
    int sizeData = theID.sz * sizeof(int);

    if (sizeData > MAX_BLOB_SIZE)
    {
        cerr.flush() << "MySqlDatastore::sendID - ID too big to send to MySQL databse, enlarge BLOBS!";
        return  -2;
    }

    // check that query string is big enough to hold the data, if not enlarge
    if ((query == 0) || ((sizeData * 2 + 254) > sizeQuery)) // *2 for extra space MySQL may need
    {
        if (query != 0)
        {
            delete [] query;
        }

        sizeQuery = 2 * sizeData + 254; // 254 for the INSERT INTO Vec... blah blah
        query = new char [sizeQuery];

        if (query == 0)
        {
            cerr.flush() << "MySqlDatastore::sendID - out of memory creating query of size";
            cerr.flush() << sizeQuery << endln;
            return -2;
        }
    }

    //
    // to send the data the database we do the following:
    // 1. try to INSERT the data into the database
    // 2. if INSERT fails we have to reformulate the query to UPDATE the existing data,
    //    as the row given by dbTag, commitTag and vectorSize may already already exist.
    //

    // form the INSERT query
    sprintf(query, "INSERT INTO IDs VALUES (%d, %d, %d,'", dbTag, commitTag, theID.sz);
    char* p = query + strlen(query);
    p += mysql_real_escape_string(&mysql, p, (char*)theID.data, sizeData);  // add ID values in binary form
    strcpy(p, "')");

    // execute the INSERT query
    if (mysql_query(&mysql, query) != 0)
    {

        //
        // if INSERT fails we reformulate query and try an UPDATE
        //

        // formulate the query
        sprintf(query, "UPDATE IDs SET data='");
        char* p = query + strlen(query);
        p += mysql_real_escape_string(&mysql, p, (char*)theID.data, sizeData);
        sprintf(p, "' WHERE dbTag=%d AND commitTag=%d AND size=%d", dbTag, commitTag, theID.sz);

        // invoke the query on the database
        if (mysql_query(&mysql, query) != 0)
        {
            cerr.flush() << "MySqlDatastore::sendID() - failed to send the ID to MySQL database";
            cerr.flush() << endln << mysql_error(&mysql) << endln;
            return -3;
        }
    }

    return 0;
}


int
MySqlDatastore::receiveID(int dbTag, int commitTag,
                       ID& theID,
                       ChannelAddress* theAddress)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }

    // check that query string is big enough to hold the data, if not enlarge
    int sizeData = theID.sz * sizeof(int);

    if ((query == 0) || ((sizeData * 2 + 256) > sizeQuery)) // *2 for extra space mysql needs
    {
        if (query != 0)
        {
            delete [] query;
        }

        sizeQuery = 2 * sizeData + 256; // 256 for the SLECECT data FROM ... blah blah
        query = new char [sizeQuery];

        if (query == 0)
        {
            cerr.flush() << "MySqlDatastore::receiveID - out of memory creating query of size";
            cerr.flush() << sizeQuery << endln;
            return -2;
        }
    }

    //
    // to receive the data the database we do the following:
    // 1. use SELECT to receive the data from the database
    // 2. fetch the results from the server and copy to the IDs data area
    // 3. clean up the MYSQL_RES datastructure
    //
    // NOTE: probably using a malloc() and free() under the covers for this, could be
    // very expensive .. IS THERE ANOTHER WAY TO DO THIS, i.e. SAVE A MYSQL_RES FOR
    // NUMEROUS USES??

    // form the SELECT query
    sprintf(query, "Select data FROM IDs WHERE dbTag=%d AND commitTag=%d AND size=%d",
            dbTag, commitTag, theID.sz);

    // execute the SELECT query
    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "MySqlDatastore::receiveID() - failed to receive ID from MySQL database";
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        return -3;
    }

    // fetch the results from the database
    MYSQL_RES* results;
    MYSQL_ROW row;
    results = mysql_store_result(&mysql);

    if (results == NULL)
    {
        // no ID stored in db with these keys
        cerr.flush() << "MySqlDatastore::receiveID - no data in database for ID with dbTag, cTag: ";
        cerr.flush() << dbTag << ", " << commitTag << endln;
        return -4;
    }

    row = mysql_fetch_row(results);

    if (row == NULL)
    {
        // no ID stored in db with these keys
        cerr.flush() << "MySqlDatastore::receiveID - no data in database for ID with dbTag, cTag: ";
        cerr.flush() << dbTag << ", " << commitTag << endln;
        mysql_free_result(results);
        return -5;
    }

    // place the results into the IDs double array
    int* data = (int*)row[0];

    for (int i = 0; i < theID.sz; i++)
    {
        theID.data[i] = data[i];
    }


    // free the MYSQL_RES structure
    mysql_free_result(results);

    return 0;
}




int
MySqlDatastore::createTable(const char* tableName, int numColumns, char* columns[])
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }

    // check that the query string can old the actual query, if not enlarge the string
    int requiredSize = 100 + strlen(tableName); // create table blah blah blah
    int sizeColumn = 0;

    for (int i = 0; i < numColumns; i++)
    {
        sizeColumn += strlen(columns[i]) + 2;
    }

    if (sizeColumnString < sizeColumn)
    {
        sizeColumnString = sizeColumn;
    }

    requiredSize += sizeColumn + 18 * numColumns;

    if (query == 0 || sizeQuery < requiredSize)
    {
        if (query != 0)
        {
            delete [] query;
        }

        query = new char[requiredSize];
        sizeQuery = requiredSize;
    }

    // create the sql query
    char* p = query;
    int numChar = sprintf(query, "CREATE TABLE %s (dbRun INT NOT NULL, commitTag INT NOT NULL, ", tableName);
    p += numChar;

    for (int j = 0; j < numColumns; j++)
    {
        numChar = sprintf(p, "%s DOUBLE NOT NULL, ", columns[j]);
        p += numChar;
    }

    sprintf(p, "PRIMARY KEY (dbRun, commitTag) )");

    // execute the query
    if (mysql_query(&mysql, query) != 0)
    {
        if (mysql_errno(&mysql) != ER_TABLE_EXISTS_ERROR)
        {
            cerr.flush() << "MySqlDatastore::createTable() - failed to create the table in the database";
            cerr.flush() << endln << mysql_error(&mysql) << endln;
            cerr.flush() << "SQL query: " << query << endln;
            return -3;
        }
    }

    return 0;
}




int
MySqlDatastore::insertData(const char* tableName, char* columns[], int commitTag, const Vector& data)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }

    // check that query string is big enough to hold the data, if not enlarge
    int sizeData = 128 + strlen(tableName);

    if ((query == 0) || (sizeData > sizeQuery))   // *2 for extra space mysql needs
    {
        if (query != 0)
        {
            delete [] query;
        }

        sizeQuery = sizeData; // 256 for the SLECECT data FROM ... blah blah
        query = new char [sizeQuery];

        if (query == 0)
        {
            cerr.flush() << "MySqlDatastore::getData - out of memory creating query of size";
            cerr.flush() << sizeQuery << endln;
            return -2;
        }
    }

    // form the insert query
    sprintf(query, "INSERT INTO %s VALUES (%d, %d ", tableName, dbRun, commitTag);
    char* p = query + strlen(query);

    for (int i = 0; i < data.Size(); i++)
    {
        p += sprintf(p, ", %f ", data(i));
    }

    strcpy(p, ")");

    // execute the query
    if (mysql_query(&mysql, query) != 0)
    {


        //
        // if INSERT fails we reformulate query and try an UPDATE
        //

        // formulate the query
        sprintf(query, "UPDATE %s SET %s=%f", tableName, columns[0], data(0));
        char* p = query + strlen(query);

        for (int i = 1; i < data.Size(); i++)
        {
            p += sprintf(p, ", %s=%f ", columns[i], data(i));
        }

        sprintf(p, " WHERE dbRun=%d AND commitTag=%d", dbTag, commitTag);

        // invoke the query on the database
        if (mysql_query(&mysql, query) != 0)
        {
            cerr.flush() << "MySqlDatastore::insertData() - failed to send the data to MySQL database";
            cerr.flush() << p;
            cerr.flush() << endln << mysql_error(&mysql) << endln;
            return -3;
        }
    }

    return 0;
}

int
MySqlDatastore::getData(const char* tableName, char* columns[], int commitTag, Vector& data)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }

    // check that query string is big enough to hold the data, if not enlarge
    int sizeData = 128 + strlen(tableName);

    if ((query == 0) || (sizeData > sizeQuery))   // *2 for extra space mysql needs
    {
        if (query != 0)
        {
            delete [] query;
        }

        sizeQuery = sizeData; // 256 for the SLECECT data FROM ... blah blah
        query = new char [sizeQuery];

        if (query == 0)
        {
            cerr.flush() << "MySqlDatastore::getData - out of memory creating query of size";
            cerr.flush() << sizeQuery << endln;
            return -2;
        }
    }

    // to receive the data the database we do the following:
    // 1. use SELECT to receive the data from the database
    // 2. fetch the results from the server and copy to the Vectors data area
    // 3. clean up the MYSQL_RES datastructure
    //
    // NOTE: probably using a malloc() and free() under the covers for this, could be
    // very expensive .. IS THERE ANOTHER WAY TO DO THIS, i.e. SAVE A MYSQL_RES FOR
    // NUMEROUS USES??

    // form the SELECT query
    sprintf(query, "Select * FROM %s WHERE dbRun=%d AND commitTag=%d",
            tableName, dbRun, commitTag);

    // execute the SELECT query
    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "MySqlDatastore::getData() - failed to receive vector from MySQL database";
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        return -3;
    }

    // fetch the results from the database
    MYSQL_RES* results;
    MYSQL_ROW row;
    results = mysql_store_result(&mysql);

    if (results == NULL)
    {
        // no vector stored in db with these keys
        cerr.flush() << "MySqlDatastore::getData - no data in database for Vector with dbTag, cTag: ";
        cerr.flush() << dbTag << ", " << commitTag << endln;
        return -4;
    }

    row = mysql_fetch_row(results);

    if (row == NULL)
    {
        // no vector stored in db with these keys
        cerr.flush() << "MySqlDatastore::getData - no data in database for Vector with dbTag, cTag: ";
        cerr.flush() << dbTag << ", " << commitTag << endln;
        mysql_free_result(results);
        return -5;
    }

    // place the results into the vectors double array
    char* dataRes;

    for (int i = 0; i < data.sz; i++)
    {
        dataRes = row[i + 2];
        data.theData[i] = atof(dataRes);
    }

    // free the MYSQL_RES structure
    mysql_free_result(results);

    return 0;
}


int
MySqlDatastore::setDbRun(int run)
{
    dbRun = run;
    return dbRun;
}

int
MySqlDatastore::getDbRun(void)
{
    return dbRun;
}


int
MySqlDatastore::createDatabase(const char* projectName)
{
    if (query == 0)
    {
        query = new char[512];
        sizeQuery = 512;
    }

    // create the database
    sprintf(query, "CREATE DATABASE %s", projectName);

    if (this->execute(query) != 0)
    {
        cerr.flush() << "MySqlDatastore::createDatabase() - could not create the database\n";
        return -1;
    }

    // link to the database,
    if (mysql_select_db(&mysql, projectName) != 0)
    {
        cerr.flush() << "MySqlDatastore::createDatabase() - could not set the database\n";
        return -2;
    }

    // now create the tables in the database
    sprintf(query, "CREATE TABLE Messages ( dbTag INT NOT NULL, commitTag INT NOT NULL, size INT NOT NULL,\
                                          data MEDIUMBLOB, PRIMARY KEY (dbTag, commitTag, size) )");

    if (this->execute(query) != 0)
    {
        cerr.flush() << "MySqlDatastore::createDatabase() - could not create the Messagess table\n";
        return -3;
    }

    sprintf(query, "CREATE TABLE Matrices ( dbTag INT NOT NULL, commitTag INT NOT NULL, size INT NOT NULL,\
                                         data MEDIUMBLOB, PRIMARY KEY (dbTag, commitTag, size) )");

    if (this->execute(query) != 0)
    {
        cerr.flush() << "MySqlDatastore::createDatabase() - could not create the Matricess table\n";
        return -3;
    }

    sprintf(query, "CREATE TABLE Vectors ( dbTag INT NOT NULL, commitTag INT NOT NULL, size INT NOT NULL,\
                                         data MEDIUMBLOB, PRIMARY KEY (dbTag, commitTag, size) )");

    if (this->execute(query) != 0)
    {
        cerr.flush() << "MySqlDatastore::createDatabase() - could not create the Vectors table\n";
        return -3;
    }

    sprintf(query, "CREATE TABLE IDs ( dbTag INT NOT NULL, commitTag INT NOT NULL, size INT NOT NULL,\
                                     data MEDIUMBLOB, PRIMARY KEY (dbTag, commitTag, size) )");

    if (this->execute(query) != 0)
    {
        cerr.flush() << "MySqlDatastore::createDatabase() - could not create the ID's table\n";
        return -3;
    }


    sprintf(query, "CREATE TABLE nDarrays ( dbTag INT NOT NULL, commitTag INT NOT NULL, size INT NOT NULL,\
                                     data MEDIUMBLOB, PRIMARY KEY (dbTag, commitTag, size) )");

    if (this->execute(query) != 0)
    {
        cerr.flush() << "MySqlDatastore::createDatabase() - could not create the nDarray's table\n";
        return -3;
    }


    // done
    return 0;
}


int
MySqlDatastore::execute(const char* query)
{

    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "MySqlDatastore::execute() - could not execute command: " << query;
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        return -1;
    }
    else
    {
        return 0;
    }
}


//*******************************************************************************************
// Nima Tafazzoli (Sep. 2012)

int
MySqlDatastore::getCommitTag()
{
    return CommitTag;
}




int
MySqlDatastore::sendnDarray(int dbTag, int commitTag,
                            const nDarray& theNDarray,
                            ChannelAddress* theAddress)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }

    // check that the data will fit, MySQL has a limit on blob sizes
    int nDarraySize = (theNDarray.pc_nDarray_rep)->total_numb;
    int sizeData = nDarraySize * sizeof(double);

    if (sizeData > MAX_BLOB_SIZE)
    {
        cerr.flush() << "MySqlDatastore::sendnDarray - nDarray too big to send to MySQL databse, enlarge BLOBS!";
        return  -2;
    }

    // check that query string is big enough to hold the data, if not enlarge
    if ((query == 0) || ((sizeData * 2 + 254) > sizeQuery))
    {
        // *2 for extra space MySQL may need
        if (query != 0)
        {
            delete [] query;
        }

        sizeQuery = 2 * sizeData + 254; // 254 for the INSERT INTO Vec...
        query = new char [sizeQuery];

        if (query == 0)
        {
            cerr.flush() << "MySqlDatastore::sendnDarray - out of memory creating query of size";
            cerr.flush() << sizeQuery << endln;
            return -2;
        }
    }

    //
    // to send the data the database we do the following:
    // 1. try to INSERT the data into the database
    // 2. if INSERT fails we have to reformulate the query to UPDATE the existing data,
    //    as the row given by dbTag, commitTag and nDarray size may already already exist.
    //

    // form the INSERT query
    sprintf(query, "INSERT INTO nDarrays VALUES (%d, %d, %d,'", dbTag, commitTag, nDarraySize);
    char* p = query + strlen(query);
    p += mysql_real_escape_string(&mysql, p, (char*)((theNDarray.pc_nDarray_rep)->pd_nDdata), sizeData);  // add nDarray values in binary form
    strcpy(p, "')");

    // execute the INSERT query
    if (mysql_query(&mysql, query) != 0)
    {

        //
        // if INSERT fails we reformulate query and try an UPDATE
        //

        // formulate the query
        sprintf(query, "UPDATE nDarrays SET data='");
        char* p = query + strlen(query);
        p += mysql_real_escape_string(&mysql, p, (char*)((theNDarray.pc_nDarray_rep)->pd_nDdata), sizeData);
        sprintf(p, "' WHERE dbTag=%d AND commitTag=%d AND size=%d", dbTag, commitTag, nDarraySize);

        // invoke the query on the database
        if (mysql_query(&mysql, query) != 0)
        {
            cerr.flush() << "MySqlDatastore::sendnDarray() - failed to send the nDarray to MySQL database";
            cerr.flush() << endln << mysql_error(&mysql) << endln;
            return -3;
        }
    }

    return 0;
}



int
MySqlDatastore::receivenDarray(int dbTag, int commitTag,
                            nDarray& theNDarray,
                            ChannelAddress* theAddress)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }

    // check that query string is big enough to hold the data, if not enlarge
    int nDarraySize = (theNDarray.pc_nDarray_rep)->total_numb;
    int sizeData = nDarraySize * sizeof(double);

    if ((query == 0) || ((sizeData * 2 + 256) > sizeQuery))
    {
        // *2 for extra space mysql needs
        if (query != 0)
        {
            delete [] query;
        }

        sizeQuery = 2 * sizeData + 256; // 256 for the SLECECT data FROM ... blah blah
        query = new char [sizeQuery];

        if (query == 0)
        {
            cerr.flush() << "MySqlDatastore::receivenDarray - out of memory creating query of size";
            cerr.flush() << sizeQuery << endln;
            return -2;
        }
    }

    //
    // to receive the data the database we do the following:
    // 1. use SELECT to receive the data from the database
    // 2. fetch the results from the server and copy to the nDarray data area
    // 3. clean up the MYSQL_RES datastructure
    //
    // NOTE: probably using a malloc() and free() under the covers for this, could be
    // very expensive .. IS THERE ANOTHER WAY TO DO THIS, i.e. SAVE A MYSQL_RES FOR
    // NUMEROUS USES??

    // form the SELECT query
    sprintf(query, "Select data FROM nDarrays WHERE dbTag=%d AND commitTag=%d AND size=%d",
            dbTag, commitTag, nDarraySize);

    // execute the SELECT query
    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "MySqlDatastore::receivenDarray() - failed to receive nDarray from MySQL database";
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        return -3;
    }

    // fetch the results from the database
    MYSQL_RES* results;
    MYSQL_ROW row;
    results = mysql_store_result(&mysql);

    if (results == NULL)
    {
        // no nDarray stored in db with these keys
        cerr.flush() << "MySqlDatastore::receivenDarray - no data in database for nDarray with dbTag, cTag: ";
        cerr.flush() << dbTag << ", " << commitTag << endln;
        return -4;
    }

    row = mysql_fetch_row(results);


    if (row == NULL)
    {
        // no nDarray stored in db with these keys
        cerr.flush() << "MySqlDatastore::receivenDarray - no data in database for nDarray with dbTag, cTag: ";
        cerr.flush() << dbTag << ", " << commitTag << endln;
        mysql_free_result(results);
        return -5;
    }

    // place the results into the nDarray double array
    double* data = (double*)row[0];

    //   for (int i=0; i<theNDarray.dataSize; i++)
    for (int i = 0; i < nDarraySize; i++)
    {
        ((theNDarray.pc_nDarray_rep)->pd_nDdata)[i] = data[i];
    }


    // free the MYSQL_RES structure
    mysql_free_result(results);

    return 0;
}

//*******************************************************************************************

int
MySqlDatastore::createDisplacementTables(int StepNumber)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }


    if (query == 0)
    {
        query = new char[1000];
        sizeQuery = 512;
    }


    ////////////////////////////////////////
    // converting integer to char
    string stepnumber;
    stringstream out;
    out << StepNumber;
    stepnumber = out.str();
    ////////////////////////////////////////


    string displacementVector = "NodalDisplacementVectorStepNumber" + stepnumber;
    const char* vectortableName = displacementVector.c_str();

    //    string displacementID = "NodalDisplacementIDStepNumber" + stepnumber;
    //    const char *idtableName = displacementID.c_str();



    // now create the tables in the database
    //   sprintf(query, "CREATE TABLE %s ( dbTag INT NOT NULL, commitTag INT NOT NULL, size INT NOT NULL,\
    //                                           data MEDIUMBLOB, PRIMARY KEY (dbTag, commitTag, size) )", vectortableName);
    sprintf(query, "CREATE TABLE %s ( Node_Number INT NOT NULL, DOF1 DOUBLE NOT NULL, DOF2 DOUBLE NOT NULL, DOF3 DOUBLE NOT NULL, DOF4 DOUBLE NOT NULL, DOF5 DOUBLE NOT NULL, DOF6 DOUBLE NOT NULL, DOF7 DOUBLE NOT NULL)", vectortableName);

    if (this->execute(query) != 0)
    {
        cerr.flush() << "MySqlDatastore::createDisplacementTables() - could not create the " << (vectortableName) << " table\n";
        return -3;
    }


    //   sprintf(query, "CREATE TABLE %s ( dbTag INT NOT NULL, commitTag INT NOT NULL, size INT NOT NULL,\
    //                                      data MEDIUMBLOB, PRIMARY KEY (dbTag, commitTag, size) )", idtableName);

    //   if (this->execute(query) != 0)
    //   {
    //     cerr.flush() << "MySqlDatastore::createDisplacementTables() - could not create the " << (*idtableName) << " table\n";
    //     return -3;
    //   }


    return 0;
}




int
MySqlDatastore::sendDisplacementsVector(int commitTag, char* Domainquery,
                                        ChannelAddress* theAddress)
{
    // check that we have a connection
    if (connection == false)
    {
        return -1;
    }



    if (mysql_query(&mysql, Domainquery) != 0)
    {
        cerr.flush() << "MySqlDatastore::sendDisplacementsVector - failed to send the Vector to MySQL database";
    }



    return 0;
}





int
MySqlDatastore::sendDisplacementsID(int dbTag, int commitTag, int StepNumber,
                                    const ID& theID,
                                    ChannelAddress* theAddress)
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
    out << StepNumber;
    stepnumber = out.str();
    ////////////////////////////////////////


    string displacementID = "NodalDisplacementIDStepNumber" + stepnumber;
    const char* idtableName = displacementID.c_str();


    // check that the data will fit, MySQL has a limit on blob sizes
    int sizeData = theID.sz * sizeof(int);

    if (sizeData > MAX_BLOB_SIZE)
    {
        cerr.flush() << "MySqlDatastore::sendID - ID too big to send to MySQL databse, enlarge BLOBS!";
        return  -2;
    }

    // check that query string is big enough to hold the data, if not enlarge
    if ((query == 0) || ((sizeData * 2 + 254) > sizeQuery)) // *2 for extra space MySQL may need
    {
        if (query != 0)
        {
            delete [] query;
        }

        sizeQuery = 2 * sizeData + 254; // 254 for the INSERT INTO Vec... blah blah
        query = new char [sizeQuery];

        if (query == 0)
        {
            cerr.flush() << "MySqlDatastore::sendID - out of memory creating query of size";
            cerr.flush() << sizeQuery << endln;
            return -2;
        }
    }

    //
    // to send the data the database we do the following:
    // 1. try to INSERT the data into the database
    // 2. if INSERT fails we have to reformulate the query to UPDATE the existing data,
    //    as the row given by dbTag, commitTag and vectorSize may already already exist.
    //

    // form the INSERT query
    sprintf(query, "INSERT INTO %s VALUES (%d, %d, %d,'", idtableName, dbTag, commitTag, theID.sz);
    char* p = query + strlen(query);
    p += mysql_real_escape_string(&mysql, p, (char*)theID.data, sizeData);  // add ID values in binary form
    strcpy(p, "')");

    // execute the INSERT query
    if (mysql_query(&mysql, query) != 0)
    {

        //
        // if INSERT fails we reformulate query and try an UPDATE
        //

        // formulate the query
        sprintf(query, "UPDATE %s SET data='", idtableName);
        char* p = query + strlen(query);
        p += mysql_real_escape_string(&mysql, p, (char*)theID.data, sizeData);
        sprintf(p, "' WHERE dbTag=%d AND commitTag=%d AND size=%d", dbTag, commitTag, theID.sz);

        // invoke the query on the database
        if (mysql_query(&mysql, query) != 0)
        {
            cerr.flush() << "MySqlDatastore::sendID() - failed to send the ID to MySQL database";
            cerr.flush() << endln << mysql_error(&mysql) << endln;
            return -3;
        }
    }

    return 0;
}





double
MySqlDatastore::recvDisplacementsVector(int NodeNumber, int dof, int commitTag, int stepNumber,
                                        ChannelAddress* theAddress)
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


    string displacementID = "NodalDisplacementVectorStepNumber" + stepnumber;
    const char* vectortableName = displacementID.c_str();





    // check that query string is big enough to hold the data, if not enlarge
    //   int sizeData = theVector.sz * sizeof(double);
    int sizeData = 7 * sizeof(double);

    if ((query == 0) || ((sizeData * 2 + 256) > sizeQuery)) // *2 for extra space mysql needs
    {
        if (query != 0)
        {
            delete [] query;
        }

        sizeQuery = 2 * sizeData + 256; // 256 for the SLECECT data FROM ... blah blah
        query = new char [sizeQuery];

        if (query == 0)
        {
            cerr.flush() << "MySqlDatastore::recvDisplacementVector - out of memory creating query of size";
            cerr.flush() << sizeQuery << endln;
            return -2;
        }
    }



    // form the SELECT query
    sprintf(query, "Select Number_of_DOF FROM %s WHERE Node_Number=%d AND commitTag=%d",
            vectortableName, NodeNumber, commitTag);


    // execute the SELECT query
    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "MySqlDatastore::recvDisplacementVector() - failed to receive vector from MySQL database";
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        return -3;
    }

    // fetch the results from the database
    MYSQL_RES* results;
    MYSQL_ROW row;
    results = mysql_store_result(&mysql);

    if (results == NULL)
    {
        // no vector stored in db with these keys
        cerr.flush() << "MySqlDatastore::recvDisplacementVector - no data in database for Vector with NodeNumber, cTag: ";
        cerr.flush() << NodeNumber << ", " << commitTag << endln;
        return -4;
    }

    row = mysql_fetch_row(results);

    if (row == NULL)
    {
        // no vector stored in db with these keys
        cerr.flush() << "MySqlDatastore::recvDisplacementVector - no data in database for Vector with NodeNumber, cTag: ";
        cerr.flush() << NodeNumber << ", " << commitTag << endln;
        mysql_free_result(results);
        return -5;
    }


    cout << "mysql vector size is: " << (*row) << endl;

    //   int VectorSize = row;


    ////////////////////////////////////////
    // converting integer to char
    string dofnumber;
    stringstream out2;
    out2 << dof;
    dofnumber = out2.str();
    ////////////////////////////////////////


    string dofnumber_column = "DOF" + dofnumber;
    const char* dof_number_restore = dofnumber_column.c_str();


    sprintf(query, "Select %s FROM %s WHERE Node_Number=%d AND commitTag=%d",
            dof_number_restore, vectortableName, NodeNumber, commitTag);


    // execute the SELECT query
    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "MySqlDatastore::recvDisplacementVector() - failed to receive vector from MySQL database";
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        return -3;
    }


    results = mysql_store_result(&mysql);

    if (results == NULL)
    {
        // no vector stored in db with these keys
        cerr.flush() << "MySqlDatastore::recvDisplacementVector - no data in database for Vector with NodeNumber, cTag: ";
        cerr.flush() << NodeNumber << ", " << commitTag << endln;
        return -4;
    }

    row = mysql_fetch_row(results);

    if (row == NULL)
    {
        // no vector stored in db with these keys
        cerr.flush() << "MySqlDatastore::recvDisplacementVector - no data in database for Vector with NodeNumber, cTag: ";
        cerr.flush() << NodeNumber << ", " << commitTag << endln;
        mysql_free_result(results);
        return -5;
    }


    cout << "mysql dof value is: " << (*row) << endl;

    //   double displacementValue = (*row);
    double displacementValue = atof(*row);

    // place the results into the vectors double array
    //   double *data = (double *)row[0];
    //   for (int i=0; i<theVector.sz; i++)
    //     theVector.theData[i] = data[i];


    // free the MYSQL_RES structure
    mysql_free_result(results);

    return displacementValue;
}





// int
// MySqlDatastore::recvDisplacementsVector(int dbTag, int commitTag, int stepNumber,
//                                         Vector &theVector,
//                                         ChannelAddress *theAddress)
// {
//   // check that we have a connection
//   if (connection == false)
//     return -1;
//
//
// ////////////////////////////////////////
// // converting integer to char
//     string stepnumber;
//     stringstream out;
//     out << stepNumber;
//     stepnumber = out.str();
// ////////////////////////////////////////
//
//
//    string displacementID = "NodalDisplacementVectorStepNumber" + stepnumber;
//    const char *vectortableName = displacementID.c_str();
//
//
//
//
//
//   // check that query string is big enough to hold the data, if not enlarge
//   int sizeData = theVector.sz * sizeof(double);
//   if ((query == 0) || ((sizeData*2 + 256) > sizeQuery)) { // *2 for extra space mysql needs
//     if (query != 0)
//       delete [] query;
//     sizeQuery = 2 * sizeData + 256; // 256 for the SLECECT data FROM ... blah blah
//     query = new char [sizeQuery];
//     if (query == 0) {
//       cerr.flush() << "MySqlDatastore::receiveVector - out of memory creating query of size";
//       cerr.flush() << sizeQuery << endln;
//       return -2;
//     }
//   }
//
//   //
//   // to receive the data the database we do the following:
//   // 1. use SELECT to receive the data from the database
//   // 2. fetch the results from the server and copy to the Vectors data area
//   // 3. clean up the MYSQL_RES datastructure
//   //
//   // NOTE: probably using a malloc() and free() under the covers for this, could be
//   // very expensive .. IS THERE ANOTHER WAY TO DO THIS, i.e. SAVE A MYSQL_RES FOR
//   // NUMEROUS USES??
//
//   // form the SELECT query
//   sprintf(query, "Select data FROM %s WHERE dbTag=%d AND commitTag=%d AND size=%d",
//           vectortableName, dbTag, commitTag, theVector.sz);
//
//   // execute the SELECT query
//   if (mysql_query(&mysql, query) != 0) {
//     cerr.flush() << "MySqlDatastore::receiveVector() - failed to receive vector from MySQL database";
//     cerr.flush() << endln << mysql_error(&mysql) << endln;
//     return -3;
//   }
//
//   // fetch the results from the database
//   MYSQL_RES *results;
//   MYSQL_ROW row;
//   results = mysql_store_result(&mysql);
//   if (results == NULL) {
//     // no vector stored in db with these keys
//     cerr.flush() << "MySqlDatastore::receiveVector - no data in database for Vector with dbTag, cTag: ";
//     cerr.flush() << dbTag << ", " << commitTag << endln;
//     return -4;
//   }
//   row = mysql_fetch_row(results);
//   if (row == NULL) {
//     // no vector stored in db with these keys
//     cerr.flush() << "MySqlDatastore::receiveVector - no data in database for Vector with dbTag, cTag: ";
//     cerr.flush() << dbTag << ", " << commitTag << endln;
//     mysql_free_result(results);
//     return -5;
//   }
//
//   // place the results into the vectors double array
//   double *data = (double *)row[0];
//   for (int i=0; i<theVector.sz; i++)
//     theVector.theData[i] = data[i];
//
//
//   // free the MYSQL_RES structure
//   mysql_free_result(results);
//
//   return 0;
// }





int
MySqlDatastore::recvDisplacementsID(int dbTag, int commitTag, int stepNumber,
                                    ID& theID,
                                    ChannelAddress* theAddress)
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


    string displacementID = "NodalDisplacementIDStepNumber" + stepnumber;
    const char* idtableName = displacementID.c_str();



    // check that query string is big enough to hold the data, if not enlarge
    int sizeData = theID.sz * sizeof(int);

    if ((query == 0) || ((sizeData * 2 + 256) > sizeQuery)) // *2 for extra space mysql needs
    {
        if (query != 0)
        {
            delete [] query;
        }

        sizeQuery = 2 * sizeData + 256; // 256 for the SLECECT data FROM ... blah blah
        query = new char [sizeQuery];

        if (query == 0)
        {
            cerr.flush() << "MySqlDatastore::receiveID - out of memory creating query of size";
            cerr.flush() << sizeQuery << endln;
            return -2;
        }
    }

    //
    // to receive the data the database we do the following:
    // 1. use SELECT to receive the data from the database
    // 2. fetch the results from the server and copy to the IDs data area
    // 3. clean up the MYSQL_RES datastructure
    //
    // NOTE: probably using a malloc() and free() under the covers for this, could be
    // very expensive .. IS THERE ANOTHER WAY TO DO THIS, i.e. SAVE A MYSQL_RES FOR
    // NUMEROUS USES??

    // form the SELECT query
    sprintf(query, "Select data FROM %s WHERE dbTag=%d AND commitTag=%d AND size=%d",
            idtableName, dbTag, commitTag, theID.sz);

    // execute the SELECT query
    if (mysql_query(&mysql, query) != 0)
    {
        cerr.flush() << "MySqlDatastore::receiveID() - failed to receive ID from MySQL database";
        cerr.flush() << endln << mysql_error(&mysql) << endln;
        return -3;
    }

    // fetch the results from the database
    MYSQL_RES* results;
    MYSQL_ROW row;
    results = mysql_store_result(&mysql);

    if (results == NULL)
    {
        // no ID stored in db with these keys
        cerr.flush() << "MySqlDatastore::receiveID - no data in database for ID with dbTag, cTag: ";
        cerr.flush() << dbTag << ", " << commitTag << endln;
        return -4;
    }

    row = mysql_fetch_row(results);

    if (row == NULL)
    {
        // no ID stored in db with these keys
        cerr.flush() << "MySqlDatastore::receiveID - no data in database for ID with dbTag, cTag: ";
        cerr.flush() << dbTag << ", " << commitTag << endln;
        mysql_free_result(results);
        return -5;
    }

    // place the results into the IDs double array
    int* data = (int*)row[0];

    for (int i = 0; i < theID.sz; i++)
    {
        theID.data[i] = data[i];
    }


    // free the MYSQL_RES structure
    mysql_free_result(results);

    return 0;
}


