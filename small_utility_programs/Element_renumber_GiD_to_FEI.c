///////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (C):     GPL
// PROJECT:           Object Oriented Finite Element Program
// FILE:              Element_renumber_GiD_to_FEI.c
//
// PURPOSE:           convert nodes numbering for 8 node brick from GID to FEI sidekick

// RETURN:
// VERSION:
// LANGUAGE:          C
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Boris Jeremic
// PROGRAMMER:        Boris Jeremic
// DATE:              03 April 2010
// UPDATE HISTORY:    
///////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] )
{ 

   FILE *input_file_pointer;   // pointer to  input file with elements with GiD numbering
   FILE *output_file_pointer;   // pointer to  output file with elements with FEI numbering

   int element_number = 0;
   int node1 = 0;
   int node2 = 0;
   int node3 = 0;
   int node4 = 0;
   int node5 = 0;
   int node6 = 0;
   int node7 = 0;
   int node8 = 0;
   int element_counter = atoi(argv[3]);
   int c;

// check number of command-line arguments 
   if ( argc != 4 ) 
     {
        fprintf(stderr, "Usage: Element_renumber_GiD_to_FEI.c input_GiD_element_file output_FEI_element_file  element_start_number \n" );
        fprintf(stderr, "input_GiD_element_file needs to have only lines with elements\n" );
        exit(1);
     } 

// open input file and check if it exists
   if ( ( input_file_pointer = fopen( argv[ 1 ], "r" ) ) == NULL ) 
     {                                           
        printf( "\n\n something is wrong with input_GiD_element_file !! \n\n\n\n\n" );
        exit(1);
     } 

   if ( ( output_file_pointer = fopen( argv[ 2 ], "w" ) ) == NULL ) 
     {                                           
        printf( "\n\n something is wrong with output_FEI_element_file !! \n\n\n\n\n" );
        exit(1);
     } 

   fprintf(stdout,"\n converting GID to FEI sidekick element node numbering\n\n");
   fprintf(stdout,"input_GiD_element_file  is %s\n",argv[1]);
   fprintf(stdout,"(and this file needs to have only elements, clean up all the other stuff\n\n");
   fprintf(stdout,"output_FEI_element_file  is %s\n",argv[2]);
   fprintf(stdout,"elements numbered starting from  %d\n\n",element_counter);



   while (fscanf( input_file_pointer, "%d  %d %d %d %d %d %d %d %d", 
                                       &element_number, 
                                       &node1,
                                       &node2,
                                       &node3,
                                       &node4,
                                       &node5,
                                       &node6,
                                       &node7,
                                       &node8 ) != EOF)

     {
     
       fprintf(output_file_pointer, "%d  %6d %6d %6d %6d %6d %6d %6d %6d\n",
                                     element_counter,
                                     node4,
                                     node8,
                                     node7,
                                     node3,
                                     node1,
                                     node5,
                                     node6,
                                     node2,
                                     node8);


// testing
// testing         fprintf(stdout,"\n\n\n\n converting from\n");
// testing         fprintf(stdout,"GID  ->  %8d      %6d %6d %6d %6d %6d %6d %6d %6d\n",
// testing                         element_number, 
// testing                         node1,
// testing                         node2,
// testing                         node3,
// testing                         node4,
// testing                         node5,
// testing                         node6,
// testing                         node7,
// testing                         node8 );
// testing         
// testing         fprintf(stdout,"\n to \n");
// testing         
// testing         
// testing         fprintf(stdout,"FEI sidekick     ->  %8d      %6d %6d %6d %6d %6d %6d %6d %6d\n",
// testing                         element_counter,
// testing                         node4,
// testing                         node8,
// testing                         node7,
// testing                         node3,
// testing                         node1,
// testing                         node5,
// testing                         node6,
// testing                         node2,
// testing                         node8);
// testing         
// testing         
// testing         
       
       element_counter++;
     }
//   while (!EOF);  
//  data_m(9*i-4)   ->  4
//  data_m(9*i)     ->  8
//  data_m(9*i-1)   ->  7
//  data_m(9*i-5)   ->  3
//  data_m(9*i-7)   ->  1
//  data_m(9*i-3)   ->  5
//  data_m(9*i-2)   ->  6
//  data_m(9*i-6)   ->  2
 
   return 0; 

} 

 

