\name{get1WINPICK}
\alias{get1WINPICK}
\title{Read one WIN pickfile}
\description{
  Read one WIN pickfile
}
\usage{
get1WINPICK(fn)
}
\arguments{
  \item{fn}{file name}
}

\value{
  \item{PF}{original pick file}
  \item{AC}{event card (a-card)}
  \item{LOC}{location}
  \item{yr}{year}
  \item{jd}{julian day}
  \item{mo}{month}
  \item{dom}{day or month}
  \item{hr}{hour}
  \item{mi}{minute}
  \item{sec}{sec}
  \item{lat}{lat}
  \item{lon}{lon}
  \item{z}{depth (km)}
  \item{mag}{magnitude}
  \item{MC}{focal mechanism card}
  \item{REFT}{Reference time for phase arrivals}
  \item{yr}{year}
  \item{jd}{julian day}
  \item{mo}{month}
  \item{dom}{day or month}
  \item{hr}{hour}
  \item{mi}{minute}
  \item{sec}{sec}
  \item{STA}{List of stations}
  \item{tag}{id tag for station}
  \item{name}{station name}
  \item{comp}{component}
  \item{c3}{other id tag}
  \item{ppol}{polarity}
  \item{parr}{p-aarival time relative to reference}
  \item{pflg}{flag}
  \item{perr}{p-error}
  \item{pres}{p-residual}
  \item{sarr}{s-arrival time relative to reference }
  \item{sflg}{flag}
  \item{serr}{s-error}
  \item{sres}{s-residual}
  \item{LIP}{error ellipse}
  \item{E}{simple error card}
  \item{infile}{input file name}
  \item{winID1}{WIN ID number}
  \item{LEVEL}{level}
  \item{PICKER}{name of picker}

}

\author{Jonathan M. Lees<jonathan.lees@unc.edu>}

\keyword{misc}
