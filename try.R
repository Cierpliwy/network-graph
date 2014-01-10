#!/usr/bin/env Rscript
require(plotrix)

generate <- function(n) {
    res <- NULL

    for (i in 1:n) {
        res <- c(res, system("./network < in0 | grep Resu | cut -d ' ' -f 2", intern=TRUE))
    }

    as.real(res)
}

plot_intervals <- function(vals, w) {
    avg <- mean(vals)

    L <- vals-w
    U <- vals+w

    plotCI(1:length(vals), vals, ui=U, li=L)
}

confidence <- function(vals) {
    wArr <- c(10,1,0.1,0.01)
    
    avg <- mean(vals)
    cat("average : ", avg,"\n")
    
    for ( w in wArr) {
        good <-0
        for(j in 1:length(vals)) {
            if((avg > vals[j]-w) && (avg < vals[j]+w)) {
                good<-good+1
            }
        }
        cat("w=",w," ","a=",good/length(vals),"\n")
        #plot_intervals(vals,w)
        #x <- readLines(file("stdin"),1)
    }
    

}

intervals <- function(vals) {
    t.test(vals, conf.level = 0.99)
    t.test(vals, conf.level = 0.95)
    t.test(vals, conf.level = 0.90)
}


vals = generate(10)
confidence(vals)
intervals(vals)
