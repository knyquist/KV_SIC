/* written by Kristofor Nyquist 
   use, modify, or scowl at freely */

#include "InputTrace.h"
#include <stdio.h>
#include <stdlib.h>

/* separate function to determine the number of rows of the dataset
   important for allowing me to initialize array of proper size to
   input the data. There are better ways to do this */
int GetTraceLength(char tracename[]) {
  FILE * fi;//(f)ile of (i)nput trace
  fi = fopen(tracename, "r");
  
  int numberoflines = 0; //pass through file once to get the size of arrays
  double temp = 0.;
  while (fscanf(fi, "%lf %lf %lf", &temp, &temp, &temp) == 3) {
    numberoflines+=1;}
  fclose(fi);
  return numberoflines;
}

/* function inputs the dataset. This function has vars named to deal 
   with tweezer  datasets, which will include time, force, and position
   generally in separate columns. Keep this in mind for generating fake
   datasets. The step fitting method doesn't care what the y axis signal
   is (force, position, fluorescence, etc). */
double** InputTrace(char tracename[],int numberoflines) {
  //from initial scan through file, initialize the time and position arrays
  //c lang doesn't do variable sized arrays without this pointer trickery
  double* time;     time     = (double*) malloc(numberoflines*sizeof(double));
  double* force;    force    = (double*) malloc(numberoflines*sizeof(double));
  double* position; position = (double*) malloc(numberoflines*sizeof(double));
  
  FILE * fi;
  fi = fopen(tracename, "r");
  int i = 0;
  while(i<numberoflines) {
    fscanf(fi, "%lf %lf %lf", &time[i], &force[i], &position[i]);
    i += 1;}
  fclose(fi);
  static double* a[3];
  for(int i=0;i<3;i++) {a[i] = (double*) malloc(numberoflines*sizeof(double));}
  for(i=0;i<numberoflines;i++) {
    a[0][i] = time[i];
    a[1][i] = force[i];
    a[2][i] = position[i];
  }
  return a;
}
