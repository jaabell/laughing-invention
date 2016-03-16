
///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell
// DATE:
// UPDATE HISTORY:     March 2016
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////

#include "MachineTopology.h"

MachineTopology::MachineTopology(Channel** channels_):
    rank(0),
    nprocesses(0),
    time_to_neighbors(0),
    channels(channels_)
{
    MPI_Comm_size (MPI_COMM_WORLD, &nprocesses);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);

    time_to_neighbors = new Vector*[MachineTopology_Max_Send_Pow2];
    for (int send_size_log2 = 0; send_size_log2 < MachineTopology_Max_Send_Pow2; send_size_log2++)
    {
        time_to_neighbors[send_size_log2] = new Vector(nprocesses); // Include self
    }
}

void MachineTopology::detect_topology()
{
    if (rank == 0)
    {
        cout << "Detecting machine topology\n";
        cout << "==========================\n";
        cout << "nprocesses = " << nprocesses << endl;
        cout << "rank       = " << rank << endl;
    }
    for (int send_size_log2 = 0; send_size_log2 < MachineTopology_Max_Send_Pow2; send_size_log2++)
    {
        int send_size = 1 << send_size_log2;
        if (rank == 0)
        {
            cout << "send_size = " << send_size << endl;
            for (int i = 0; i < nprocesses; i++)
            {

            }
        }
    }
}