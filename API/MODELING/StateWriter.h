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
// PURPOSE:           API
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Jose Abell, Boris Jeremic
//
// DATE:              Feb 2014
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////




class StateWriter
{
    public:

        static bool isOutputBinary;

        //=====================================================================================================================
        // Constructor
        //=====================================================================================================================
        StateWriter(string ModelName_, string StageName_, bool isTheAnalysisDynamic_):
            ModelName(ModelName_),
            StageName(StageName_),
            nodal_displacements_file(),
            nodal_velocities_file(),
            nodal_accelerations_file(),
            element_stresses_file(),
            element_strains_file(),
            element_end_forces_file()
        {
            //isOutputBinary = isOutputBinary_;
            isTheAnalysisDynamic = isTheAnalysisDynamic_;

            // Form filenames
            string nodal_displacements_filename = ModelName + "_" + StageName + "_Nodes_Generalized_Displacements.feioutput";
            string element_stresses_filename    = ModelName + "_" + StageName + "_Element_Stresses.feioutput";
            string element_strains_filename     = ModelName + "_" + StageName + "_Element_Strains.feioutput";
            string element_end_forces_filename  = ModelName + "_" + StageName + "_Element_End_Forces.feioutput";
            //string element_end_forces_filename  = ModelName + "_" + StageName + "_Element_End_Forces.feioutput";

            ios_base::openmode mode;

            //Open file output streams (ofstream)
            if (isOutputBinary)
            {
                mode = ios::out | ios::binary;
            }
            else
            {
                mode = ios::out;
            }

            nodal_displacements_file.open(nodal_displacements_filename, mode);
            element_stresses_file.open(element_stresses_filename, mode);
            element_strains_file.open(element_strains_filename, mode);
            element_end_forces_file.open(element_end_forces_filename, mode);

            //Check that the files are open correctly
            if (not nodal_displacements_file.is_open())
            {
                cerr << "StateWriter::StateWriter -> ERROR Opening file. " << nodal_displacements_filename << endl;
            }
            if (not element_stresses_file.is_open())
            {
                cerr << "StateWriter::StateWriter -> ERROR Opening file. " << element_stresses_file << endl;
            }
            if (not element_strains_file.is_open())
            {
                cerr << "StateWriter::StateWriter -> ERROR Opening file. " << element_strains_file << endl;
            }
            if (not element_end_forces_file.is_open())
            {
                cerr << "StateWriter::StateWriter -> ERROR Opening file. " << element_end_forces_file << endl;
            }

            if (isTheAnalysisDynamic)
            {
                string nodal_velocities_filename    = ModelName + "_" + StageName + "_Nodes_Generalized_Velocities.feioutput";
                string nodal_accelerations_filename = ModelName + "_" + StageName + "_Nodes_Generalized_Accelerations.feioutput";
                nodal_velocities_file.open(nodal_velocities_filename, mode);
                nodal_accelerations_file.open(nodal_accelerations_filename, mode);
            }
        }

        //=====================================================================================================================
        // Destructor
        //=====================================================================================================================
        ~StateWriter()
        {
            if (nodal_displacements_file.is_open())
            {
                nodal_displacements_file.close();
            }
            if (nodal_velocities_file.is_open())
            {
                nodal_velocities_file.close();
            }
            if (nodal_accelerations_file.is_open())
            {
                nodal_accelerations_file.close();
            }
            if (element_stresses_file.is_open())
            {
                element_stresses_file.close();
            }
            if (element_strains_file.is_open())
            {
                element_strains_file.close();
            }
            if (element_end_forces_file.is_open())
            {
                element_end_forces_file.close();
            }
        }


        //=====================================================================================================================
        // Write out stuff for this step
        //=====================================================================================================================
        void write_static_analysis_output()
        {
            double currentTime = theDomain.getCurrentTime();

            // Write node data
            // ----------------------------------------------------------
            int Nnodes         = theDomain.getNumNodes();
            NodeIter &theNodes = theDomain.getNodes();
            Node *node;

            //"Header" for this line
            write_header_time_and_number_of_data(nodal_displacements_file, currentTime, Nnodes);

            //Write nodal data
            while ((node = theNodes()) != 0 )
            {
                int tag = node->getTag();
                int num_dof = node->getNumberDOF();
                const Vector &disp = node->getDisp();

                write_tag_ndata_and_vector(nodal_displacements_file, tag, num_dof, disp);
            }

            //End this line
            end_this_entry(nodal_displacements_file);


            // Write element data
            // ----------------------------------------------------------
            write_element_output(currentTime);
        }

        void write_dynamic_analysis_output()
        {
            double currentTime = theDomain.getCurrentTime();

            // Write node data
            // ----------------------------------------------------------
            int Nnodes         = theDomain.getNumNodes();
            NodeIter &theNodes = theDomain.getNodes();
            Node *node;

            //"Header" for this line
            write_header_time_and_number_of_data(nodal_displacements_file, currentTime, Nnodes);
            write_header_time_and_number_of_data(nodal_velocities_file, currentTime, Nnodes);
            write_header_time_and_number_of_data(nodal_accelerations_file, currentTime, Nnodes);


            //Write nodal data
            while ((node = theNodes()) != 0 )
            {
                int tag = node->getTag();
                int num_dof = node->getNumberDOF();
                const Vector &disp  = node->getDisp();
                const Vector &vel   = node->getVel();
                const Vector &accel =  node->getAccel();

                write_tag_ndata_and_vector(nodal_displacements_file, tag, num_dof, disp);
                write_tag_ndata_and_vector(nodal_velocities_file, tag, num_dof, vel);
                write_tag_ndata_and_vector(nodal_accelerations_file, tag, num_dof, accel);
            }

            //End this line
            end_this_entry(nodal_displacements_file);
            end_this_entry(nodal_velocities_file);
            end_this_entry(nodal_accelerations_file);


            // Write element data
            // ----------------------------------------------------------
            write_element_output(currentTime);
        }



        //=====================================================================================================================
        // Internals
        //=====================================================================================================================

    private:

        void write_element_output(double &currentTime)
        {
            int Nelems         = theDomain.getNumElements();
            ElementIter &theElements = theDomain.getElements();
            Element *element;
            Response *response;
            Information eleInformation;

            write_header_time_and_number_of_data(element_stresses_file, currentTime, Nelems);
            write_header_time_and_number_of_data(element_strains_file, currentTime, Nelems);
            while ((element = theElements()) != 0 )
            {
                int tag = element->getTag();

                // For stresses
                const char *data_type = "stresses";
                response = element->setResponse(&data_type, 0, eleInformation);
                response->getResponse();
                eleInformation = response->getInformation();
                const Vector &stress = eleInformation.getData();

                write_tag_ndata_and_vector(element_stresses_file, tag , stress.Size(), stress);

                //For strains
                data_type = "strains";
                response = element->setResponse(&data_type, 0, eleInformation);
                response->getResponse();
                eleInformation = response->getInformation();
                const Vector &strains = eleInformation.getData();

                write_tag_ndata_and_vector(element_strains_file, tag , strains.Size(), strains);
            }
            end_this_entry(element_stresses_file);
            end_this_entry(element_strains_file);
        }


        void write_header_time_and_number_of_data(ofstream &s,  double &t, int number_of_data)
        {
            if (isOutputBinary)
            {
                s.write(reinterpret_cast <const char *> (&t), sizeof(double)); // << num_of_data;

            }
            else
            {
                s << t;// << ' ' << num_of_data;
            }
        }

        void write_tag_ndata_and_vector(ofstream &s,  int tag, int ndata, const Vector &v)
        {
            if (isOutputBinary)
            {
                //s << tag << ndata;
                for (int dof = 0; dof < ndata; dof++)
                {
                    double data = v(dof);
                    s.write(reinterpret_cast <const char *> (&data), sizeof(double));
                }
            }
            else
            {
                //s << ' ' << tag << ' ' << ndata;
                for (int dof = 0; dof < ndata; dof++)
                {
                    s << ' ' << v(dof);
                }
            }
        }

        void end_this_entry(ofstream &s)
        {
            if (not isOutputBinary)
            {
                s << endl;
            }
        }

        void write_mesh_info_file()
        {

        }


        // Make StateWriter not copyable.... because ofstream
        StateWriter(const StateWriter &);
        StateWriter &operator=(const StateWriter &);


        //Stuff
        string ModelName;
        string StageName;
        bool isTheAnalysisDynamic;
        ofstream nodal_displacements_file;
        ofstream nodal_velocities_file;
        ofstream nodal_accelerations_file;
        ofstream element_stresses_file;
        ofstream element_strains_file;
        ofstream element_end_forces_file;
};

bool StateWriter::isOutputBinary = false;


int define_output_to_binary(void)
{
    StateWriter::isOutputBinary = true;
    return 0;
}

int define_output_to_text(void)
{
    StateWriter::isOutputBinary = false;
    return 0;
}







