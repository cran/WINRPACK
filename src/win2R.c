/* @(#)win2sac.c 1.2 95/12/15 22:17:44 */
/* Format conversion from WIN format to SAC format */
/* program dewin  1994.4.11-4.20  urabe */
/* modified for kazan-system in NIED 1997.7.9 fujita */
/* modified by Ueda 2005.3.8 */
/* adopted to little endian processors by Ueda 2005.5.13 */
/* modified by Ueda 2005.7.22 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif



#include	<stdio.h>
#include	<signal.h>
#include        <math.h>
#include        <string.h>

#include <stdlib.h>
#include <ctype.h>

 
#ifdef WIN32
#include <winsock2.h>
#include <io.h>
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#endif




/*
#ifdef HAVE_BSD_NETWORKING
#  include <netdb.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#endif


#ifdef HAVE_NETINET_IN_H
#ifdef _WIN32
# include <winsock2.h>
#else
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
#endif

#include <arpa/inet.h> 
#include <netinet/in.h> 


*/





/*
     Some  systems  require  the  inclusion   of   <netinet/in.h>   instead   of
       <arpa/inet.h>.


unsigned long int ntohl(unsigned long int netlong);
unsigned short int ntohs(unsigned short int netshort);

*/
    

/* #include <machine/endian.h> */

/* unsigned long ntohl(unsigned long netlong); */


#define  TRUE 1
#define  FALSE 0

#define DEBUG 0

int buf[1000];

int time1[6];


#define FREE_BOY char*

int NUMMALLOC = 0;
int REMALLOC = 0;

#define JM_ADD 0

#define XmapNewArray(T, N) \
        (T *)calloc((size_t)(N), sizeof(T))


int time_cmp(int *t1,int *t2);
unsigned int read_data(unsigned char **ptr,FILE *fp);
int read_one_sec(unsigned char *ptr,int  sys_ch, int *abuf);
int bcd_dec(int *dest, unsigned   char *sour);
void adj_time(int *tm);


 
 int stfind(int chno,char  kstnm[5], char kcmp[5] ,float  *cmpaz,
          float  *cmpinc,float  *stla,float  *stlo,float  *stel,
	    float  *sense, char *infile);



/************************************************************/
/* FUNC DEF */ int jgetline(FILE *input, char s[], int lim)
  {
  int c = 0, i;

  for(i = 0; (i < lim - 1) && ((c = fgetc(input)) != EOF) && (c != '\n'); i++)
     s[i] = c;

  if(c == '\n') s[i++] = c;

  s[i] = '\0';

  return(i);
  }


/** FUNC DEF  **/ void free_cmat(char **m, int row_first, int row_last, int col_first, int col_last)
{
        free((FREE_BOY) (m[row_first]+col_first-JM_ADD));
        free((FREE_BOY) (m+row_first-JM_ADD));
}

/************************************************************/
/* FUNC DEF */ char  **alloc_cmat(long row_first, long row_last, long col_first,
                   long col_last)
  {
  long i, nrow = row_last - row_first + 1, ncol = col_last - col_first + 1;
  char **m;

  /* allocate pointers to rows */

  m = XmapNewArray(char *, (nrow + JM_ADD));
  m += JM_ADD;
  m -= row_first;

  /* allocate rows and set pointers to them */

  m[row_first] = XmapNewArray(char, (nrow * ncol + JM_ADD));
  m[row_first] += JM_ADD;
  m[row_first] -= col_first;

  for(i = row_first + 1; i <= row_last; i++) m[i] = m[i - 1] + ncol;

  /* return pointer to array of pointers to rows */

  return(m);
  }
/************************************************************/

/* FUNC DEF */ int getchantbl(char *infile, char **chlist, int kount)
{
  FILE *f_chan;
  int i,  k;
  char line[1000];
  char string[100];

  
  f_chan = fopen(infile,"r");
  
  
  k = 0;
  while( (i = jgetline(f_chan, line, 1000)) > 0 )
    {
      
      /* fprintf(stderr,"%s", line);  */
      if(line[0] == '#') { continue; }
      if( isalpha(line[15]) == 0) { continue; }
      
      sscanf(line, "%s",  string);
      
      /* fprintf(stderr, "%s\n", string );  */
      
      strcpy(chlist[k], string);
      k++;
    }
  
  
  /*
    for(k=0; k< kount; k++)
    {
    fprintf(stderr, "%d %s\n", k, chlist[k] );
    }
  */
  
  
  fclose(f_chan);
  return(k);
}

/* FUNC DEF */ int getchans(char *infile, char **chlist, int kount)
{
  FILE *f_chan;
  int i,  k;
  char line[1000];
  char string[5];

  
 f_chan = fopen(infile,"r");

 

     for(k=0; k< kount; k++)
     {
	if( (i = jgetline(f_chan, line, 1000))<1)
	{

	   fprintf(stderr,"ERROR: %s", line);
	   continue;
	}
 	/* fprintf(stderr,"%s", line);  */

	  /* sscanf(line, "%s",  string); */

	strncpy(string, line, 5);

	/* fprintf(stderr, "%s\n", string );  */
 
	 
	strncpy(chlist[k], string, 5);
	 
     }
     

     /*
     for(k=0; k< kount; k++)
       {
	 fprintf(stderr, "%d %s\n", k, chlist[k] );
       }
     */
 

     fclose(f_chan);
     return(k);
}
/*******************************************************/

/** FUNC DEF */ void CALL_GETWINN(char **fn, int *n,  int *Nsta,  char **chin)
{

 
  int i,j,k, isec,  mainsize,sr,sr_save,
    time2[6],time3[6];
  int loop;
  static  unsigned char *mainbuf;

  int npts;
  int sysch;
  FILE *f_main;

  int MYK;
 
  MYK = *Nsta;
  /* fprintf(stderr, "GETWINN %d %s\n", MYK, fn[0]);  */


  for(loop=0; loop<MYK; loop++) 
    { 
       /* fprintf(stderr, "GETWINN %d %s\n", loop, chin[loop]);  */ 
      sysch=strtol(chin[loop],0,16);

      if((f_main=fopen(fn[0],"r"))==NULL){
	fprintf(stderr, "can't open file: %s\n", fn[0]);
	return;
      }
      isec=sr_save=i=0;
      npts = 0;

      while((mainsize=read_data(&mainbuf,f_main))) 
	{
	  if((sr=read_one_sec(mainbuf,sysch,buf))==0) 
	  { 
	      /* fprintf(stderr,"skip\n");  */
	     continue;
	  }
	  bcd_dec(time3,mainbuf+4);
	  
#if DEBUG
	  fprintf(stdout,"%02d%02d%02d.%02d%02d%02d\n",
		  time3[0],time3[1],time3[2],time3[3],time3[4],time3[5]);
#endif
	  
	  if(sr_save==0) {
	    bcd_dec(time1,mainbuf+4);
	    
	    /*  fprintf(stderr,"%02d%02d%02d.%02d%02d%02d -> ",  */
	    /* 	   	      time1[0],time1[1],time1[2],time1[3],time1[4],time1[5]);  */
	    
	  }
	  if(sr_save) {
	    time2[5]++;
	    adj_time(time2);
	    while(time_cmp(time2,time3)<0) {
	      k=0;
	      for(j=0;j<sr_save;j++) {
		
		npts++;
	      }
	      
	      i++;
	      time2[5]++;
	      adj_time(time2);
	    }
	  }
	  
	  for(j=0;j<sr;j++) {
     
	    npts++;
	  }

	  i++;
	  sr_save=sr;
	  bcd_dec(time2,mainbuf+4);
	  isec++;

	    /* free((unsigned char *)mainbuf);  */
	}
      /* fprintf(stderr, "GETWINN %d %s NUMMALLOC=%d REMALLOC=%d\n", loop, chin[loop], NUMMALLOC, REMALLOC);   */
      /* fprintf(stderr, "GETWINN %d %s npts=%d\n",  loop, chin[loop], npts);  */
      n[loop] = npts;
      fclose(f_main);
      
      mainbuf = (unsigned char *)0;   
      free(mainbuf);   
    }

 
}



/** FUNC DEF */ int CALL_GETWINT(char **fn, double *x, int *n, int *DATIM,  char **chin)
{

 
  int i,j,k, isec, mainsize,sr,sr_save,
    time2[6],time3[6];
  int loop;
  static unsigned char *mainbuf;

  int npts;
  int sysch;
  FILE *f_main;

  int inJ;
 
  /*  k = *Nsta; */
  /* fprintf(stderr, "GETWINT %d\n", k);  */
  /* for(loop=0; loop<k; loop++)  */
    
  loop = 0;

  inJ = *n;

  /* fprintf(stderr, "GETWINT %d\n", inJ); */

  if( inJ<=0 ) return(0) ;

  /* fprintf(stderr, "GETWINT %d %s\n", loop, chin[loop], );  */
  sysch=strtol(chin[loop],0,16);

  if((f_main=fopen(fn[0],"r"))==NULL){
    fprintf(stderr, "can't open file: %s\n", fn[0]);
    return(0);
  }
  isec=sr_save=i=0;

  
  npts = 0;

  while((mainsize=read_data(&mainbuf,f_main))) {
    if((sr=read_one_sec(mainbuf,sysch,buf))==0) 
    { 
       continue;
    }
    bcd_dec(time3,mainbuf+4);

#if DEBUG
    fprintf(stdout,"%02d%02d%02d.%02d%02d%02d\n",
	    time3[0],time3[1],time3[2],time3[3],time3[4],time3[5]);
#endif

    if(sr_save==0) {
      bcd_dec(time1,mainbuf+4);

      /* fprintf(stderr,"%02d%02d%02d.%02d%02d%02d -> ", */
      /* 	      time1[0],time1[1],time1[2],time1[3],time1[4],time1[5]); */

    }
    if(sr_save) {
      time2[5]++;
      adj_time(time2);
      while(time_cmp(time2,time3)<0) {
	k=0;
	for(j=0;j<sr_save;j++) {
	  if(npts>inJ) { return(0); }

	  x[npts]=(float)k;
	  npts++;
	}

	i++;
	time2[5]++;
	adj_time(time2);
      }
    }

    for(j=0;j<sr;j++) {
      if(npts>inJ) { return(0); }
      x[npts]=(float)buf[j];
      npts++;
    }

    i++;
    sr_save=sr;
    bcd_dec(time2,mainbuf+4);
    isec++;

    /* free((char *)mainbuf);  */
  }
  /* fprintf(stderr, "npts=%d\n", npts); */
  *n = sr;
  for(j=0;j<6;j++) { DATIM[j] = time1[j]; }

  fclose(f_main);

    mainbuf = (unsigned char *)0;    
   free(mainbuf);  


 return(1);
 
}





/*
################# day.c ###################
*/



void adj_time(int *tm)
{
  if(tm[5]==60)
    {
      tm[5]=0;
      if(++tm[4]==60)
	{
	  tm[4]=0;
	  if(++tm[3]==24)
	    {
	      tm[3]=0;
	      tm[2]++;
	      switch(tm[1])
		{
		case 2:
		  if(tm[0]%4==0)
		    {
		      if(tm[2]==30)
			{
			  tm[2]=1;
			  tm[1]++;
			}
		      break;
		    }
		  else
		    {
		      if(tm[2]==29)
			{
			  tm[2]=1;
			  tm[1]++;
			}
		      break;
		    }
		case 4:
		case 6:
		case 9:
		case 11:
		  if(tm[2]==31)
		    {
		      tm[2]=1;
		      tm[1]++;
		    }
		  break;
		default:
		  if(tm[2]==32)
		    {
		      tm[2]=1;
		      tm[1]++;
		    }
		  break;
		}
	      if(tm[1]==13)
		{
		  tm[1]=1;
		  if(++tm[0]==100) tm[0]=0;
		}
	    }
	}
    }
  else if(tm[5]==-1)
    {
      tm[5]=59;
      if(--tm[4]==-1)
	{
	  tm[4]=59;
	  if(--tm[3]==-1)
	    {
	      tm[3]=23;
	      if(--tm[2]==0)
		{
		  switch(--tm[1])
		    {
		    case 2:
		      if(tm[0]%4==0)
			tm[2]=29;else tm[2]=28;
		      break;
		    case 4:
		    case 6:
		    case 9:
		    case 11:
		      tm[2]=30;
		      break;
		    default:
		      tm[2]=31;
		      break;
		    }
		  if(tm[1]==0)
		    {
		      tm[1]=12;
		      if(--tm[0]==-1) tm[0]=99;
		    }
		}
	    }
	}
    }
}
/**************************************************************/
/* FUNC DEF */ int bcd_dec(int *dest, unsigned  char *sour)
{
  int cntr;
  for(cntr=0;cntr<6;cntr++)
    dest[cntr]=((sour[cntr]>>4)&0xf)*10+(sour[cntr]&0xf);
  return(1);
}
/**************************************************************/
/* FUNC DEF */int time_cmp(int *t1,int *t2)
{
  int cntr;
  for(cntr=0;cntr<6;cntr++)
    {
      if(t1[cntr]>t2[cntr]) return 1;
      if(t1[cntr]<t2[cntr]) return -1;
    }
  return 0;
}
/**************************************************************/
/* FUNC DEF */unsigned int mklong(unsigned char *ptr)
{
  unsigned char *ptr1;
  unsigned int  a;
  ptr1=(unsigned char *)&a;
  *ptr1++=(*ptr++);
  *ptr1++=(*ptr++);
  *ptr1++=(*ptr++);
  *ptr1  =(*ptr);
  return a;
}
/*********************#########################*******************/
/*********************#########################*******************/
/*********************#########################*******************/


/* FUNC DEF */ unsigned int read_data(unsigned char **ptr,FILE *fp)
{
#define DEBUG1 0

  static unsigned int size;
  int re;
  int rsize;
  if(fread(&re,4,1,fp)==0) return 0;

  re=(unsigned int)ntohl(re);		/* big endian -> little endian */	

#if DEBUG1 
  fprintf(stderr,"BLOCK SIZE = %d bytes\n",re);
  fprintf(stderr,"PTR = %x\n",*ptr);
#endif

  if(*ptr==0) {
     /* free(*ptr); */ 

    *ptr=(unsigned char *)malloc(size=re*2);
    /* NUMMALLOC++; */
  /* fprintf(stderr,"MALLOC PTR = %x\n",*ptr);   */
	
  } else {

   free(*ptr);


    /* if(re>size) *ptr=(unsigned char *)realloc(*ptr,size=re*2); */


  /* if(re>size) */


   *ptr=(unsigned char *)malloc(size=re*2); 




   /*  REMALLOC++; */

/*  fprintf(stderr,"REALLOC PTR = %x\n",*ptr);  */

  }

#if DEBUG1
  fprintf(stderr,"PTR = %x\n",*ptr);			/* address */
#endif

  *(int *)*ptr=re;

#if DEBUG1
  fprintf(stderr,"PTR = %x\n",*ptr);
#endif

  if((rsize=fread(*ptr+4,1,re-4,fp))==0) return 0;	

#if DEBUG1
  fprintf(stderr,"read size = %d\n",rsize);
#endif

#if DEBUG1
  fprintf(stderr,"%x %x %x %x %x %x %d\n",*(ptr+4),ptr[5],ptr[6],
	  ptr[7],ptr[8],ptr[9],re);
#endif


  return re;
}
/*********************#########################*******************/
/*********************#########################*******************/
/*********************#########################*******************/

/* FUNC DEF */ int read_one_sec(unsigned char *ptr,int  sys_ch, int *abuf)
	       /* input */
	       /* sys_ch = sys*256 + ch */
	       /* output */
{
  int b_size,g_size;
  register int i,s_rate;
  register unsigned char *dp,*pts;
  unsigned int gh;
  unsigned char *ddp;
  short shreg;
  int inreg;

  dp=ptr+10;
  ddp=ptr+mklong(ptr);

  while(1) {
    pts=(unsigned char *)&gh;
    *pts++=(*dp++);
    *pts++=(*dp++);
    *pts++=(*dp++);
    *pts  =(*dp++);
    gh=(unsigned int)ntohl(gh);		/* big -> little */	

    s_rate=gh&0xfff;
    if((b_size=((gh>>12)&0xf))) {
	g_size=b_size*(s_rate-1)+4;
   } else {
	g_size=(s_rate>>1)+4;
	}

    if((gh>>16)==sys_ch) {
      break;
    } else {
      if((dp+=g_size)>=ddp) return 0;
    }
  }
  /* read group */
  pts=(unsigned char *)&abuf[0];
  *pts++=(*dp++);
  *pts++=(*dp++);
  *pts++=(*dp++);
  *pts  =(*dp++);
  if(s_rate==1) return s_rate;
    abuf[0]=(unsigned int)ntohl(abuf[0]);	

/*  fprintf(stderr,"abuf = %d\n",abuf[0]);
*/

  switch(b_size) {
  case 0:
    for(i=1;i<s_rate;i+=2) {
      abuf[i]=abuf[i-1]+((*(char *)dp)>>4);
      abuf[i+1]=abuf[i]+(((char)(*(dp++)<<4))>>4);      /* 4 bit */
    }
    break;
  case 1:
    for(i=1;i<s_rate;i++) {
      abuf[i]=abuf[i-1]+(*(char *)(dp++));              /* 1 byte */ 
	}
    break;
  case 2:
    for(i=1;i<s_rate;i++) {
      pts=(unsigned char *)&shreg;
      *pts++=(*dp++);
      *pts  =(*dp++);
/*      abuf[i]=abuf[i-1]+shreg;
*/
      abuf[i]=abuf[i-1]+(short)ntohs(shreg); 		/* big -> little */
    }
    break;
  case 3:
    for(i=1;i<s_rate;i++) {
      pts=(unsigned char *)&inreg;
      *pts++=(*dp++);
      *pts++=(*dp++);
      *pts  =(*dp++);
/*      abuf[i]=abuf[i-1]+(inreg>>8);
*/
      abuf[i]=abuf[i-1]+((int)ntohl(inreg)>>8);  /* 3 bytes */
    }
    break;
  case 4:
    for(i=1;i<s_rate;i++) {
      pts=(unsigned char *)&inreg;
      *pts++=(*dp++);
      *pts++=(*dp++);
      *pts++=(*dp++);
      *pts  =(*dp++);
/*      abuf[i]=abuf[i-1]+inreg;
*/
      abuf[i]=abuf[i-1]+(unsigned int)ntohl(inreg);       /* 4 bytes */
    }
    break;
  default:
    return 0;	/* bad header */
  }
  
  return s_rate;	/* normal return */
}


