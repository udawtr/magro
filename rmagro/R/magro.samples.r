magro.samples <- function(model, n.iter, n.burnin, list.data=list(), inits=list(), 
thin=1, n.chain=3, mt=1, monitor=c())
{
	library(coda)
	
	updates <- n.iter - n.burnin
	burnin <- n.burnin
	
	names <- attr(list.data, "names")
	first = TRUE
	for(name in names)
	{
		write(paste(name, "<-"), "data.txt", append=!first)
		first = FALSE
		s  = paste(deparse(list.data[[name]], width.cutoff=50, control="all"))
		write(s, "data.txt", append=!first)
	}
	
	names <- attr(inits, "names")
	first = TRUE
	for(name in names)
	{
		write(paste(name, "<-"), "inits.txt", append=!first)
		first = FALSE
		s  = paste(deparse(inits[[name]], width.cutoff=50, control="all"))
		write(s, "inits.txt", append=!first)
	}
	
	s <- ""
	l <- length(monitor)
	for( m in monitor)
	{
		s <- paste(s, ",", m, sep="")
	}
	cmd= paste("magro ", model, " ", n.iter, " ", n.burnin, " -data=data.txt -init=inits.txt",
			   " -m=",substring(s,2), " -thin=", thin," -c=",n.chain, sep="")
	system(cmd)
	
	nvar <- length(monitor)
	niter = updates/thin
	var.i <- matrix(NA, nrow = niter, ncol = nvar)
	ans.ch <- vector("list", n.chain)
	ans <- vector("list", n.chain)
	t <- read.delim("CODAindex.txt", header=FALSE)
	for(ch in 1:n.chain)
	{
		tc <- read.delim(paste("CODAchain",ch,".txt", sep=""), header=FALSE)
		for(j in 1:nvar) {
			vname <- t[j,1]
			begin <- t[j,2]
			end <- t[j,3]
			values <- tc[,2]
			var.i[,j] <- values[begin:end]
		}
		ans.ch[[ch]] <- var.i
		ans.ch[[ch]] <- as.matrix(data.frame(ans.ch[[ch]]))
		colnames(ans.ch[[ch]]) <- monitor
		ans[[ch]] <- mcmc(ans.ch[[ch]], start = n.burnin, thin = thin)
	}
	
	x <- mcmc.list(ans)
	return(x)
}
