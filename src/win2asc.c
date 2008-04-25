/* @(#)win2sac.c 1.2 95/12/15 22:17:44 */
/* Format conversion from WIN format to SAC format */
/* program dewin  1994.4.11-4.20  urabe */
/* modified for kazan-system in NIED 1997.7.9 fujita */
/* modified by Ueda 2005.3.8 */
/* adopted to little endian processors by Ueda 2005.5.13 */
/* modified by Ueda 2005.7.22 */
#include	<stdio.h>
#include	<signal.h>
#include        <math.h>
#include        <string.h>

#include <stdlib.h>


#define  TRUE 1
#define  FALSE 0

#define DEBUG 0


int buf[1000];
char winfile[150];
int time1[6];

#define JM_ADD 0
#define XmapNewArray(T, N) \
        (T *)calloc((size_t)(N), sizeof(T))

/*******************************************************/
/*******************************************************/
/*******************************************************/



int jgetline(FILE *input, char s[], int lim);

char  **alloc_cmat(long row_first, long row_last, long col_first,
                   long col_last);


int getchans(char *infile, char **chlist, int kount);
int getchantbl(char *infile, char **chlist, int kount);

int getwin(char *infile, FILE *f_main, int  sysch, double *wiggle);

void CALL_GETWINN(char **fn, int *n,  int *Nsta,  char **chin);

int CALL_GETWINT(char **fn, double *x, int *n, int *DATIM,  char **chin);


void free_cmat(char **m, int row_first, int row_last, int col_first, int col_last);

int main(int argc,char **argv)
{
  FILE *f_main, *f_chan, *f_list;
    int sysch;

char chanfile[200];
 /* char *chantemp; */
 char **chlist;
 int j, k, kount, i, MYKAY;

char line[1000];

char winlist[1000];

 int *npertrace;

 char  **dofile, **dotrace;

 int DATIM[6];

 double *mywig, SUM, SUM2;
 
  static char ver[]="@(#)drive_win2R adapted to raw and trg data by H.Ueda and J. M. Lees";


  fprintf(stderr,"%s\n",ver);
  if(argc < 2) {
    fprintf(stderr,"usage: drive_win2R winfile\n");
    exit(1);
  }
  
  
  strcpy(winlist,argv[1]);
  
  if((f_list=fopen(winlist,"r"))==NULL)
    {
      perror(argv[0]);
      exit(1);
    }
 MYKAY  = 0;
  while(fscanf(f_list, "%s", winfile)>0)
    {
      i++; 
      /* fprintf(stderr, "%d %s\n", i, winfile);  */
      /*     } */
      
      /*   exit(0); */
      
      
      /* strcpy(winfile,argv[1]); */

      /****   all this does is check to see if this file exists and is readable  ***/
      
      if((f_main=fopen(winfile,"r"))==NULL)
	{
	  perror(argv[0]);
	  fprintf(stderr, "%s\n", winfile);
	  exit(1);
	}
      fclose(f_main);

      /* sysch=strtol(argv[2],0,16); */
      
      
      strcpy(  chanfile, winfile);
      strcat(  chanfile, ".ch");
      /*  fprintf(stderr,"%s %s\n", winfile, chanfile ); */
      
      
      
      if((f_chan=fopen(chanfile,"r"))==NULL)
	{
	  fprintf(stderr, "CANNOT OPEN %d %s\n", i, chanfile);
	  /****   hear need to open a generic channel file, read and set up the matrix of ID's  ****/
	  
	  fclose(f_main);
	  continue; 

	  /**** exit(0);   *****/
	  
	}
      else
	{
	  /************    channel file exists, so read in the number of channels  **/
	  
	  kount = 0;
	  
	  while( (i = jgetline(f_chan, line, 1000)) != 0)
	    {
	      kount++;
	    } 
	  fclose(f_chan);
	  fprintf(stderr, "%s : number of chans = %d K=%d\n", chanfile, kount , MYKAY);
	  fprintf(stdout, "%s : number of chans = %d K=%d\n", chanfile, kount , MYKAY);
	  fflush(stdout);
	  
	  
	  chlist = alloc_cmat(0, kount , 0, 5);
 
	  j = getchans(chanfile, chlist, kount);
	  MYKAY++;
	  /* fprintf(stderr,"K=%d  number of channels= %d\n", j, MYKAY); */
	  
	  
	}

      dotrace = alloc_cmat(0, 1 , 0, 5);
      
      dofile = alloc_cmat(0, 1 , 0, 150);
      
      strcpy(dofile[0], winfile);
      
      npertrace =  (int  *)malloc(kount*sizeof(int));

      
      
      CALL_GETWINN( dofile , npertrace , &kount , chlist );
      
 
#if 1
      for(k=0; k< kount; k++)
	{
	 /*  fprintf(stderr, "k = %d syscha=%s length=%d ", k, chlist[k], npertrace[k] ); */
	  sysch=strtol(chlist[k],0,16);
	  
	  if(npertrace[k]>0)
	    {
	      mywig =  (double  *)malloc(npertrace[k]*sizeof(double));
	      j = npertrace[k];
	      strcpy(dotrace[0], chlist[k]);
	      CALL_GETWINT(dofile, mywig, &j, DATIM, dotrace );
	      
	      /****  to show that this is really the trace, do some calculation?*/
	      SUM= 0; SUM2=0;
	      j = npertrace[k];
	      
	      /*  for(i=0; i<j; i++) { SUM += mywig[i]; SUM2 += mywig[i]*mywig[i] ; } */
	      
	      
	      free(mywig);
	      
	      /* fprintf(stderr, "SUM = %f  SUM2 = %f\n", SUM, SUM2); */
	    }
	}
#endif

      free_cmat( chlist, 0, kount , 0, 5);
      free_cmat( dotrace, 0, 1 , 0, 5);
      free_cmat(dofile,  0, 1 , 0, 150);
      free(npertrace);
      
      
    }
  
 
  exit(1);
}
