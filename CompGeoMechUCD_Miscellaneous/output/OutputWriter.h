///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//``This  source code is Copyrighted in U.S., by the The Regents of the University
//of California, for an indefinite period, and anybody caught using it without our
//permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              OutputWriter.h
// CLASS:             OutputWriter
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Abstract Class. Provides a framework for handling output in general.
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Jose Abell, Boris Jeremic
// PROGRAMMER:        Jose Abell
// DATE:              Mon 05 May 2014 10:09:35 AM PDT
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <ID.h>
#include <Vector.h>
#include <Matrix.h>

#ifndef OutputWriter_H
#define OutputWriter_H

class OutputWriter
{
public:
    OutputWriter();
    ~OutputWriter();

    virtual int setTime(float t) = 0;

    // Mesh output
    virtual int writeGlobalMeshData(unsigned int number_of_nodes_in,
                                    unsigned int number_of_elements_in,
                                    unsigned int max_node_tag_in,
                                    unsigned int max_element_tag_in,
                                    unsigned int number_of_dofs_in,
                                    unsigned int number_of_outputs_in,
                                    unsigned int Total_Number_of_Gauss_Points,
                                    unsigned int Total_Number_of_Connectivity_Nodes)=0;

    // virtual int writeNumberOfNodes(unsigned int numberOfNodes_ ) ;
    // virtual int writeNum
    virtual int writeNodeMeshData(int tag,
                                  const Vector &coords,
                                  int ndofs ) = 0;
    virtual int writeElementMeshData(int         tag,
                                     std::string  type,
                                     const        ID &connectivity,
                                     int          materialtag,
                                     const        Matrix &gausscoordinates,
                                     int          length_of_output,
                                     int class_tag) = 0;
    virtual int writeMaterialMeshData(int         tag,
                                      std::string type) = 0;
    // Results for Nodes
    virtual int writeDisplacements(  int nodeTag, const Vector &displacements) = 0;
    virtual int writeVelocities(     int nodeTag, const Vector &velocities) = 0;
    virtual int writeAccelerations(  int nodeTag, const Vector &accelerations) = 0;
    virtual int writeReactionForces( int nodeTag, const Vector &reactionForces) = 0;

    // Results for Elements
    virtual int writeElementOutput(int elementTag, const Vector &output) = 0;


private:

};
#endif   //#ifndef OutputWriter_H