\name{myscan1}
\alias{myscan1}

\title{Scan a character string}
\description{
  Scan a character string
}
\usage{
myscan1(string, what = character(0), ...)
}

\arguments{
  \item{string}{character String}
  \item{what}{list to scan}
  \item{\dots}{other parameters}
}
\details{
  Uses C-like functions for scanning
}
\value{
 list of objects
}

\author{Jonathan M. Lees<jonathan.lees@unc.edu>}

\seealso{scan, strsplit}
\examples{

h = "this is a joke"
myscan1(h, what="")


}


\keyword{misc}

