`readwinch` <-
function(fn, wigdir="/home/lees/Site/Fuji/WIG")
  {
    if(missing(wigdir)) { wigdir="/home/lees/Site/Fuji/WIG" }
    
    chf = paste(sep='.', fn, "ch");

    if( !file.exists(chf)) { return(NULL) }

    A = scan(file=chf, what="", sep="\n", quiet=TRUE)
    
    stach1 = scan(file=chf, what=list(cid='', a1='', a2='', stname='', comp=''), flush=TRUE, quiet=TRUE)

    ww1 = which(stach1$comp=="U")
    B = A[ww1]
    B = B[nchar(B)>75]
     sdata = myscan1(B, what=list(cid='', a1='', a2='', stname='', comp='',
                             z6='',z7='',z8='',z9='', z10='', z11='',z12='',z13='',z14='',z15='',z16='' ), flush=TRUE)
  
    nam = sdata$stname

    vu   = as.numeric(sdata$z8)
    adc  = as.numeric(sdata$z13)
    gain = as.numeric(sdata$z12)
    sense=adc/10.0^(gain/20.0)/vu;
    lat = sdata$z14
    lon = sdata$z15
    zee = sdata$z16
    
    msta = match(stach1$stname, nam)

    SENSE = as.numeric(sense[msta])
    LAT = as.numeric(lat[msta])
    LON = as.numeric(lon[msta])
    ZEE = as.numeric(zee[msta])
    
    return(list(code=stach1$cid,  sta=stach1$stname, comp=stach1$comp, sense=SENSE, lat=LAT, lon=LON, z=ZEE))

  }

