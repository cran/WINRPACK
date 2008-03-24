`XWINdata` <-
function(fn, stasel=stasel, PLOT=FALSE)
  {
    if(missing(stasel) ) { stasel=NULL }
    if(missing(PLOT) ) { PLOT=FALSE }
    
    ##   chins = getwinR(fn)
    
    chans = readwinch(fn)
    chins = chans$code

    NSTA = length(chins)
    
    infile = fn
    
    ## x = rep(0,length=1000000)
    
    DATIM =  rep(0,length=4)
    n=rep(0,length=NSTA)
    dt=0.025000
    sec = 0
 

    barfa = .C("CALL_GETWINN",PACKAGE = "WINRPACK",
      infile,
      as.integer(n),
      NSTA,
      chins
      )
    
    nums = unlist(barfa[2])

  ###  print(cbind(chans$code, chans$sta, chans$comp, nums))

 ###    return(0)
###   temporary stop

    chins = chans$code
    GIVE = list()
    k = 0
    if(!is.null(stasel) )
      { 
        m2 = match(stasel, chins)
        wsel = which(!is.na(m2))
      } else {
        wsel = 1:length(chins)
        m2 = 1:length(chins)
      }

  ###  print(wsel)
  ###  print(m2)
    
    if(length(wsel)<1) {
       print(paste(sep=' ', fn, "wsel=0 nums=", nums))

      return(0)
    }
    
    

    for(I in 1:length(wsel))
      {
        i = m2[wsel[I]]

    ###   print(paste(sep=' ', chans$sta[i],  chans$comp[i], nums[i]))

      if(nums[i]<1) next

        
        infile = fn 
        x = rep(0,length=nums[i])
        DATIM =  rep(0,length=6)
        n=0
        sr = nums[i]
       
        barfa = .C("CALL_GETWINT",PACKAGE = "WINRPACK",
          infile,
          as.double(x),
          as.integer(sr),
          as.integer(DATIM),
          chins[i]
          )

        
###  here the n is the sample per second (sample rate)
        sr = as.numeric(unlist(barfa[3]))

        if(sr<=0) next
        y = unlist(barfa[2])*chans$sense[i]
       ### print(sr)
        delta = 1/(sr)
        

        DATIM = as.numeric(unlist(barfa[4]))
        nzyear = DATIM[1]
        if (nzyear > 70 ) {
          nzyear = nzyear+1900;
        } else {
          nzyear =  nzyear+2000;
        }
        
        if(PLOT==TRUE)
          {
            x = seq(from=0, by=delta, length=length(y))
            plot(x, y, type='l')
            locator()
          }
        
        
        
       jd  = getjul(nzyear,DATIM[2], DATIM[3] )
            
        t1 = 0
        t2 = delta*(nums[i]-1)
        
        aunits = "m/s"
        
        tstart = list(yr=nzyear, jd=jd , mo=DATIM[2] , dom=DATIM[3], hr=DATIM[4], mi=DATIM[5], sec=DATIM[6],
          msec=0, dt=delta, t1=t1,
          t2=t2, off=0)
        
        
        
        k = k +1
        GIVE[[k]] = list(fn=fn, sta=chans$sta[i],  comp=chans$comp[i], dt=delta, DATTIM=tstart, N=nums[i], units=aunits , amp=y)
               
      }
    
    invisible(GIVE)
  }

