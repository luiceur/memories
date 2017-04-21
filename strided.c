#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

void run(double* restrict a, double* restrict b, double* restrict c,
	 int stride, int N){
  for (int i=0; i<N; ++i)
    c[i*stride] = a[i*stride] + b[i*stride];
  
}
double get_host_current_time()
{
  static int start = 0, startu = 0;
  struct timeval tval;
  double result;
  
  if (gettimeofday(&tval, NULL) == -1)
    result = -1.0;
  else if(!start) {
    start = tval.tv_sec;
    startu = tval.tv_usec;
    result = 0.0;
  }
  else
    result = (double) (tval.tv_sec - start) + 1.0e-6*(tval.tv_usec - startu);
  
  return result;
}

int main(int argc, char *argv[]){

  size_t N = 5000000;
  double *x,*y,*z;
  clock_t start,end;  
  double elapsed;
  MPI_Init(&argc, &argv);
  posix_memalign((void**)&x,64,32*N*sizeof(double));
  posix_memalign((void**)&y,64,32*N*sizeof(double));
  posix_memalign((void**)&z,64,32*N*sizeof(double));

  
  for (int i=0; i< 32*N; i++){
    x[i]=1.0;
    y[i]=2.0;
    z[i]=3.0;
  }

  //warm up
  run(x,y,z,1,N);
  
  printf("#stride      time              GBi/s\n");
  for (int strided=1; strided <= 32; strided++){
    

    double t0 = MPI_Wtime();
    for(int j=0; j < 20;j++){
      run(x,y,z,strided,N);
      
    }
    double t1 = MPI_Wtime();
    //end = get_host_current_time();;
    elapsed = t1-t0;
  
    printf(" %d  --   %f    ---    %f\n",strided,
	   elapsed,(20.*3.*sizeof(double)*N)/(elapsed*1e9) );
  }

  MPI_Finalize();
  
  return 0;
}

