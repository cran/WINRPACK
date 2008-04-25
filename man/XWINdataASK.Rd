\name{XWINdataASK}
\alias{XWINdataASK}
\title{Extract win format to ASCII}
\description{
  Extract win format to ASCII
}
\usage{
XWINdataASK(fn, stasel = stasel, PLOT = FALSE)
}
\arguments{
  \item{fn}{file name}
  \item{stasel}{selection of stations}
  \item{PLOT}{logical wheaterh to plot}
}

\value{
  \item{List}{stuff}

   List of of seismic traces (time series with headers):
  \item{fn}{original file name}
  
  \item{sta}{station name}
  \item{comp}{compnent}

  \item{dt}{delta t in seconds}
  \item{DATTIM}{time list}
  \item{yr}{year}
  \item{jd}{julian day}
  \item{mo}{month}
  \item{dom}{day of month}
  \item{hr}{hour}
  \item{mi}{minute}
  \item{sec}{sec}
  \item{msec}{milliseconds}
  \item{dt}{delta t in seconds}
  \item{t1}{time start of trace}
  \item{t2}{time end of trace}
  \item{off}{off-set}
  
  \item{N}{number of points in trace}
  \item{units}{units}
  \item{amp}{vector of trace values}

  
  
}

\author{Jonathan M. Lees<jonathan.lees@unc.edu>}

\keyword{misc}
