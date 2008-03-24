`XWINdataASK` <-
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
 

   

  ###  print(cbind(chans$code, chans$sta, chans$comp, nums))

 ###    return(0)
###   temporary stop

    
    GIVE = list()
    k = 0
    if(!is.null(stasel) )
      { 
        m2 = match(stasel, chins)
        wsel = which(!is.na(m2))
      }
    else
      {
        m2= 1:length(chins)
        wsel = 1:length(chins)
      }

  ###  print(wsel)
  ###  print(m2)
    
    if(length(wsel)<1) {
       print(paste(sep=' ', fn, "wsel=0 nums=", nums))

      return(NULL)
    }
    
    

    for(I in 1:length(wsel))
      {
        i = m2[wsel[I]]

    ###   print(paste(sep=' ', chans$sta[i],  chans$comp[i], nums[i]))

     ###  if(nums[i]<1) next

        
     
       
        cmd = paste(sep=" ", "win2asc1" , fn, chins[i], "> HO")

        SOUT = system(cmd)
        ##  print(SOUT)
        D1 = scan(file="HO", what=0, n=7, quiet=TRUE)
        
        if(length(D1)<7)
          {
            print(paste(sep=" ", "BAD win2asc1 output", fn, chins[i], chans$sta[i],  chans$comp[i]));
            next
          }
        
        DATIM = D1[1:6]
        sr = D1[7]

        ay = scan(file="HO", skip=1, what=0, quiet=TRUE)
        nums = length(ay)
###  here the n is the sample per second (sample rate)
        if(is.null(sr) | !is.numeric(sr) )
          {
            print(paste(sep=" ", "BAD win2asc1 output NO sr", fn, chins[i], chans$sta[i],chans$comp[i] ));
            next;

          }

        
        if(sr<=0) next

        
        y = ay*chans$sense[i]
       ### print(sr)
        delta = 1/(sr)
        

       
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
        t2 = delta*(nums[I]-1)
        
        aunits = "m/s"
        
        tstart = list(yr=nzyear, jd=jd , mo=DATIM[2] , dom=DATIM[3], hr=DATIM[4], mi=DATIM[5], sec=DATIM[6],
          msec=0, dt=delta, t1=t1,
          t2=t2, off=0)
        
        
        
        k = k+1
        GIVE[[k]] = list(fn=fn, sta=chans$sta[i],  comp=chans$comp[i], dt=delta, DATTIM=tstart, N=nums, units=aunits , amp=y)
               
      }
    
    invisible(GIVE)
  }

