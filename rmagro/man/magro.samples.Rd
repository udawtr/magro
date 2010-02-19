\name{magro.samples}
\alias{magro.samples}
\title{Generate posterior samples in mcmc.list format}
\description{
  This is a wrapper function for MAGRO - MCMC Compiler.
}
\usage{
magro.samples(model, n.iter, n.burnin, list.data=list(), inits=list(), thin = 1, n.chain = 3, mt = 1, monitor=c())
}
\arguments{
  \item{model}{the model definition file in BUGS language}
  \item{monitor}{a character vector giving the names of variables
    to be monitored}
  \item{n.iter}{number of iterations to monitor}
  \item{n.burnin}{number of iterations to burn-in}
  \item{n.chain}{number of chain}
  \item{thin}{thinning interval for monitors}
  \item{mt}{if mt was set to 1, magro runs multi-threding mode, but set to 0, runs single-thread-mode}
  \item{list.data}{data}
  \item{inits}{initiale parameters}
}
\value{
  An \code{mcmc.list} object.
}
\author{Wataru Uda}
\keyword{models}
