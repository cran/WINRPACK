`get1WINPICK` <-
function(fn)
{


   if( !file.exists(fn)) { return(NULL) }

  A = scan(file=fn, sep="\n", what="", quiet = TRUE)
  cflags = substr(A, 1,2)

  w1 = which(cflags=="#s")
  if(length(w1)<2) return(NULL)

  
                                        # check to see if this is the reference time card

  yflag = substr(A[w1],6,6)
  w2 = which(yflag=="/")

  r1 = unlist(strsplit(A[w1[w2]], split=' '))
  r2 = as.numeric(unlist(strsplit(r1[2], split="/")))
  r3 = as.numeric(unlist(strsplit(r1[3], split=":")))
  if(r2[1]<10) { ryr = r2[1]+2000 } else { ryr = r2[1]+1900 }

  refjd = getjul(ryr,r2[2] , r2[3])
  REFT = list(yr=ryr, jd=refjd, mo=r2[2], dom=r2[3], hr=r3[1], mi=r3[2], sec=0)


  
  w3 = w1[-w2]

  nw = nchar(A[w3])

  scards = A[w3]
   scards = scards[nw>10]
  
 ## A[w3]


  ###########   this code does not work because the files are not consistant
####  stnames  =   substr(A[w3], 4, 6);

####  pol = substr(A[w3],9, 9);

####  ppick = as.numeric(substr(A[w3],11,18))

####  perr = as.numeric(substr(A[w3],19,24))
####  spick = as.numeric(substr(A[w3],25,32))
####  serr = as.numeric(substr(A[w3],33, 38))

  sdata = myscan1(scards, what=list(aflag='', stnames='', pol='', ppick=0, perr=0, spick=0, serr=0), flush=TRUE)
  
####sdata$ppick[sdata$ppick==0 & sdata$perr==0]
    good = 1:length(sdata$ppick)
  
   wbadp = sdata$ppick==0 & sdata$perr==0

   sdata$ppick[wbadp] = NA
   sdata$perr[wbadp] = NA

   wbads = sdata$spick==0 & sdata$serr==0

   sdata$spick[wbads] = NA
   sdata$serr[wbads] = NA

sdata$pol[sdata$pol=="."] = "_"
   
   
  ###  good = which(!is.na(sdata$ppick))
     
 ### sdata$stnames = sdata$stnames[good]


   STA = list(tag="", name=sdata$stnames[good], comp="", c3="", ppol=sdata$pol[good], parr=sdata$ppick[good], pflg=0,
     perr=sdata$perr[good], pres=0, sarr=sdata$spick[good], sflg=0, serr=sdata$serr[good], sres=0 )

##########  find hypocenter card

  wf =  w3[length(w3)]+1

  if(wf > length(A))
    {
      fcard = NULL
      LOC = NULL
    }
  else
    {
      fcard = A[wf]
    }
  if(!exists("fcard")) { fcard=NULL }

  if(is.null(fcard) ) { LOC = NULL  }
  else
    {
      evyy = as.numeric(substr(fcard,5,6));
  
      evmo = as.numeric(substr(fcard,8,9));
      evda = as.numeric(substr(fcard,11,12));
      evhr = as.numeric(substr(fcard,17,18));
      evmin = as.numeric(substr(fcard,20,21));
      evsec = as.numeric(substr(fcard,23,30));

      evlat = as.numeric(substr(fcard,33,41));
      evlon = as.numeric(substr(fcard,43,52));
      evz = as.numeric(substr(fcard,54,61));
      evmag = as.numeric(substr(fcard,62,67));


      if(evyy < 10)
        {
          evyy = evyy+2000;
          
        }
      else
        {
          
          evyy = evyy+1900;
          
        }

      jd = getjul(evyy, evmo, evda)
      
      LOC = list(yr=evyy, jd=jd, mo=evmo, dom=evda, hr=evhr, mi=evmin, sec=evsec, 
        lat=evlat, lon=evlon, z=evz, mag=evmag)
    }
  MC = NULL
  LIP = NULL
  E=NULL
  ACARD = NULL

   idline = A[1]
   
   idsplit = unlist(strsplit(idline, split=" "))

   id1 = idsplit[2]
   id2 = idsplit[3]
   picker  = idsplit[4]

	if(is.null(id1)) id1 = NA
	if(is.null(id2)) id2 = NA
	if(is.null(picker)) picker = NA


 
##f   0  2  6    13 55   19.100   41.54094  143.90092  238.267   4.7
#c...|....1....|....2....|....3....|....4....|....5....|....6....|....7.$
  return(list(PF=A, AC=ACARD, LOC=LOC, MC=MC, REFT=REFT, STA=STA, LIP=LIP, E=E , infile=fn, winID1=id1, LEVEL=id2, PICKER=picker))
}

