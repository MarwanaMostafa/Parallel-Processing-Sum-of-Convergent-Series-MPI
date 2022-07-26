# Parallel-Processing-Sum-of-Convergent-Series-MPI
Riemann zeta function

## How To Run :
  1-you must have a virtual machine

  2-to compile code : mpicc –o __OutPut.out__ __FileName.c__
  
  3-to run : mpiexec -n __numberOfProcesses__ __OutPut.out__

## How TO Work (Logic):
  
  1- Take N(100,1000,10000,....) from User and Number Of Process 
  
  2- Divide N on Number Of Process (Master Process which do Divide) 
  
  3- Each process calculate Riemann zeta function for each range 
  
  4- Each process send to Master Process final result And add them to each other using __MPI_Reduce__
  
  5- calculate Speed and Performance for  testing 
  
  
## See Also:

  * [Histogram Usin MPI and OPENMP](https://github.com/MarwanaMostafa/Parallel-Processing_Histogram_MPI-and-OPENMP)

  * [K-means-Clustering Using OpenMp](https://github.com/MarwanaMostafa/Parallel-Processing_K-means-Clustering_OpenMp)

  * [Divide Data by standard function Using MPI](https://github.com/MarwanaMostafa/Parallel-Processing-Sum-of-Convergent-Series-MPI/tree/main/Divide%20Data%20By%20Standard%20Function)

