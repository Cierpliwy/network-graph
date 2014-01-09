#!/usr/bin/env Rscript
require(plotrix)

generate <- function(n) {
    res <- NULL

    for (i in 1:n) {
        res <- c(res, system("./network < in0 | grep Resu | cut -d ' ' -f 2", intern=TRUE))
    }

    as.real(res)
}

plot_intervals <- function(n, w) {
    res = generate(n)

    L <- res-w
    U <- res+w

    plotCI(res:length(res), res, ui=U, li=L)
}

confidence <- function() {
    wArr <- c(10,1,0.1,0.1)
    for ( i in wArr) {
        i
    }
}
