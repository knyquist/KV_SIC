/* Kristofor Nyquist 2012 April 7th 
An SIC step fitting program using method by Kalafut and Visscher
Kalafut et. al. (2008)
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "InputTrace.h"
#include "SICalgorithm.h"

/* this main function takes argument, which is the dataset file (see README)
   I chose to do it this way, because it makes it easy to use a scripting 
   language such as awk to loop through many different data files and output
   fits in a single line */
int main(int argc, char *argv[] )
{
  /* Get the trace information */
  int numberofpoints = GetTraceLength(argv[1]);
  double** arr = InputTrace(argv[1], numberofpoints);
  double* time;     time     = (double*) malloc(numberofpoints*sizeof(double));
  double* force;    force    = (double*) malloc(numberofpoints*sizeof(double));
  double* position; position = (double*) malloc(numberofpoints*sizeof(double));
  for(int i=0;i<numberofpoints;i++) { //dump data to time and position arrays
    time[i] = arr[0][i];
    force[i] = arr[1][i];
    position[i] = arr[2][i];
  }
  
  /* Initialize with No Step SIC calculation */
  // step_fit object is defined in header. 
  step_fit this_fit; this_fit = InitializeFitToZeros(this_fit,1);
  this_fit = NoStepSIC(position,
		       numberofpoints);
  
  /* Start Adding Steps, terminate when SIC no longer minimized */
  int n_dwells = 1; step_fit final_fit;
  step_fit prev_fit; prev_fit = InitializeFitToZeros(prev_fit,n_dwells);
  int* step_indices; step_indices = (int*) malloc(numberofpoints*sizeof(int));
  for(int i=0;i<numberofpoints;i++) step_indices[i] = 0;
  do {
    // the last iteration's fit becomes the previous fit
    prev_fit = InitializeFitToZeros(prev_fit,n_dwells-1);
    prev_fit = MakeFitOneDwellBigger(prev_fit,n_dwells);
    for(int i=0;i<n_dwells;i++) {
      prev_fit.means[i]=this_fit.means[i];
      if(i!=n_dwells) prev_fit.step_locations[i] = this_fit.step_locations[i];
      }
    prev_fit.SIC = this_fit.SIC;
    prev_fit.chisq = this_fit.chisq;
    step_indices = UpdateStepIndices(prev_fit,n_dwells,step_indices);
    // resize this_fit's lists to allow space for the next fit
    //error at line below
    this_fit = MakeFitOneDwellBigger(this_fit,n_dwells+1);
    this_fit = SetFitToZeros(this_fit,n_dwells+1); //populate with zeros
    this_fit = AddStepSIC(position,
			  numberofpoints,
			  step_indices,
			  n_dwells,
			  prev_fit);
    n_dwells+=1;
  }
  while(this_fit.SIC < prev_fit.SIC);
  //output the fit!
  //fit is output to screen, which you can dump to file using ">" symbol
  //when you run the executable. See README for tips on how to run.
  printf("%f %f\n", time[0], prev_fit.means[0]);
  for(int i=0;i<n_dwells-2;i++) {
    printf("%f %f\n",time[prev_fit.step_locations[i]],prev_fit.means[i]);
    printf("%f %f\n",time[prev_fit.step_locations[i]],prev_fit.means[i+1]);
  }
  printf("%f %f\n",time[numberofpoints-1],prev_fit.means[n_dwells-2]);

  return 0;
}


