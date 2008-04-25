\name{readwinch}
\alias{readwinch}
\title{ Read WIN channel file }
\description{
  Read WIN channel file
}
\usage{
readwinch(fn, wigdir = "/home/lees/Site/Fuji/WIG")
}
\arguments{
  \item{fn}{file name}
  \item{wigdir}{name of directory where waveforms reside}
}

\value{
  List:
  \item{code}{code for id of event}
  \item{sta}{station name}
  \item{comp}{component}
  \item{sense}{sensitivity}
  \item{lat}{LAT}
  \item{lon}{LON}
  \item{z}{depth}
  
}

\author{Jonathan M. Lees<jonathan.lees@unc.edu>}

\keyword{misc}
