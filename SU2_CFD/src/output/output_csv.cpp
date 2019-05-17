#include "../../include/output/output.hpp"

void COutput::WriteSurface_CSV(CConfig *config, CGeometry *geometry){
  
  /*--- Routine to write the surface CSV files (ASCII). We
   assume here that, as an ASCII file, it is safer to merge the
   surface data onto the master rank for writing for 2 reasons:
   (a) as a surface file, the amount of data should be much less
   than the volume solution, and (b) writing ASCII files in parallel
   requires serializing the IO calls with barriers, which ruins
   the performance at moderate to high rank counts. ---*/
  
  unsigned short iVar;
  
  int iProcessor, nProcessor = size;
  
  unsigned long iPoint, index;
  unsigned long Buffer_Send_nVertex[1], *Buffer_Recv_nVertex = NULL;
  unsigned long nLocalVertex_Surface = 0, MaxLocalVertex_Surface = 0;
  
  string filename = config->GetFilename(SurfaceFilename, ".csv");
  
  ofstream Surf_file;
  Surf_file.precision(15);
  
  /*--- Find the max number of surface vertices among all
   partitions so we can set up buffers. The master node will handle
   the writing of the CSV file after gathering all of the data. ---*/
  
  nLocalVertex_Surface   = nSurf_Poin_Par;
  Buffer_Send_nVertex[0] = nLocalVertex_Surface;
  if (rank == MASTER_NODE) Buffer_Recv_nVertex = new unsigned long[nProcessor];
  
  /*--- Communicate the number of local vertices on each partition
   to the master node with collective calls. ---*/
  
  SU2_MPI::Allreduce(&nLocalVertex_Surface, &MaxLocalVertex_Surface, 1,
                     MPI_UNSIGNED_LONG, MPI_MAX, MPI_COMM_WORLD);
  
  SU2_MPI::Gather(&Buffer_Send_nVertex, 1, MPI_UNSIGNED_LONG,
                  Buffer_Recv_nVertex,  1, MPI_UNSIGNED_LONG,
                  MASTER_NODE, MPI_COMM_WORLD);
  
  /*--- Allocate buffers for send/recv of the data and global IDs. ---*/
  
  su2double *bufD_Send = new su2double[MaxLocalVertex_Surface*GlobalField_Counter];
  su2double *bufD_Recv = NULL;
  
  unsigned long *bufL_Send = new unsigned long [MaxLocalVertex_Surface];
  unsigned long *bufL_Recv = NULL;
  
  /*--- Load send buffers with the local data on this rank. ---*/
  
  index = 0;
  for (iPoint = 0; iPoint < nSurf_Poin_Par; iPoint++) {
    
    /*--- Global index values. ---*/
    
    bufL_Send[iPoint] = Renumber2Global[iPoint];
    
    /*--- Solution data. ---*/
    
    for (iVar = 0; iVar < GlobalField_Counter; iVar++){
      bufD_Send[index] = Parallel_Surf_Data[iVar][iPoint];
      index++;
    }
    
  }
  
  /*--- Only the master rank allocates buffers for the recv. ---*/
  
  if (rank == MASTER_NODE) {
    bufD_Recv = new su2double[nProcessor*MaxLocalVertex_Surface*GlobalField_Counter];
    bufL_Recv = new unsigned long[nProcessor*MaxLocalVertex_Surface];
  }
  
  /*--- Collective comms of the solution data and global IDs. ---*/
  
  SU2_MPI::Gather(bufD_Send, (int)MaxLocalVertex_Surface*GlobalField_Counter, MPI_DOUBLE,
                  bufD_Recv, (int)MaxLocalVertex_Surface*GlobalField_Counter, MPI_DOUBLE, MASTER_NODE, MPI_COMM_WORLD);
  
  SU2_MPI::Gather(bufL_Send, (int)MaxLocalVertex_Surface, MPI_UNSIGNED_LONG,
                  bufL_Recv, (int)MaxLocalVertex_Surface, MPI_UNSIGNED_LONG, MASTER_NODE, MPI_COMM_WORLD);
  
  /*--- The master rank alone writes the surface CSV file. ---*/
  
  if (rank == MASTER_NODE) {
    
    /*--- Open the CSV file and write the header with variable names. ---*/
    
    Surf_file.open(filename.c_str(), ios::out);
    Surf_file << "\"Point\",";
    for (iVar = 0; iVar < Variable_Names.size()-1; iVar++) {
      Surf_file << "\"" << Variable_Names[iVar] << "\",";
    }
    Surf_file << "\"" << Variable_Names[Variable_Names.size()-1] << "\"" << endl;
    
    /*--- Loop through all of the collected data and write each node's values ---*/
    
    for (iProcessor = 0; iProcessor < nProcessor; iProcessor++) {
      for (iPoint = 0; iPoint < Buffer_Recv_nVertex[iProcessor]; iPoint++) {
        
        /*--- Current index position for global index access. ---*/
        
        index  = iProcessor*MaxLocalVertex_Surface + iPoint;
        
        /*--- Write global index values. ---*/
        
        Surf_file << bufL_Recv[index] << ", ";
        
        /*--- Reset index for solution data access. ---*/
        
        index  = (iProcessor*MaxLocalVertex_Surface*GlobalField_Counter +
                  iPoint*GlobalField_Counter);
        
        /*--- Write the solution data for each field variable. ---*/
        
        for (iVar = 0; iVar < GlobalField_Counter; iVar++){
          Surf_file << scientific << bufD_Recv[index + iVar];
          if (iVar != GlobalField_Counter -1) Surf_file << ", ";
        }
        Surf_file << endl;
        
      }
    }
    
    /*--- Close the file. ---*/
    
    Surf_file.close();
    
  }
  
  /*--- Free temporary memory. ---*/
  
  if (rank == MASTER_NODE) {
    delete [] bufL_Recv;
    delete [] bufD_Recv;
    delete [] Buffer_Recv_nVertex;
  }
  delete [] bufL_Send;
  delete [] bufD_Send;
  
}