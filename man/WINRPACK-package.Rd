\name{WINRPACK-package}
\alias{WINRPACK-package}
\alias{WINRPACK}
\docType{package}
\title{
  Read and convert WIN format to R
}
\description{
  Package reads in WIN format data from Japan and converts to R
}
\details{
  \tabular{ll}{
    Package: \tab WINRPACK\cr
    Type: \tab Package\cr
    Version: \tab 1.0-9\cr
    Date: \tab 2008-04-25\cr
    License: \tab GPL\cr
  }


}

\note{

  \describe{
    \item{get1WINPICK}{Read one WIN pickfile}
    \item{readwinch}{Read WIN channel file}
    \item{XWINdataASK}{Extract win format to ASCII}
    \item{XWINdata}{Extract WIN format}
    \item{getjul}{Get Julian day}
    \item{myscan1}{Scan a character string}
    \item{tojul}{Convert to Julian Day. Used for calculations.}
  }
}

\author{
  Jonathan M. Lees<jonathan.lees\@unc.edu>
  Maintainer:Jonathan M. Lees<jonathan.lees\@unc.edu>

}


\examples{

\dontrun{

###  this example will run in the inst/doc folder of this distribution

###  set the pick file
winpick = "970723.235151.840"

###  read in the pick file
zip = get1WINPICK(winpick)
 fn = zip$winID1
chans = readwinch(fn)

 m1 = match(zip$STA$name, chans$sta)

    zip$STA$lat = chans$lat[m1]
    zip$STA$lon = chans$lon[m1]
    zip$STA$z   = chans$z[m1]

##   read in the waveform data
JH = XWINdata(fn, stasel=NULL, PLOT=FALSE)

require(RSEIS)


###  prepare the structure for RSEIS
KH = prepSEIS(JH)
w1 = which(KH$COMPS=="V")

###  plot in RSEIS
PICK.GEN(KH,  sel = w1, SHOWONLY =TRUE )


}

}




\keyword{ package }
\seealso{
  RSEIS, Rsac
}
