#!/usr/bin/env Rscript
require(ggplot2)

simpleBarGraph <- function(xvals, yvals, xlabel, ylabel, title) {
    frame = data.frame(x=xvals, y=yvals, mean = mean(yvals))
    xbreaks = customBreaks(min(xvals), max(xvals))
    ybreaks = customBreaks(min(yvals), max(yvals))

    azx = 96.3
    ggplot(frame) +
    geom_bar(aes(x,y), stat="identity", fill="#00ffff", colour="#333333") +
    scale_x_continuous(breaks = xbreaks) +
    scale_y_continuous(breaks = ybreaks, labels = paste0(ybreaks, " %")) +
    coord_cartesian(ylim = c(min(yvals),max(yvals))) + 
    xlab(xlabel) +
    ylab(ylabel) +
    geom_hline(aes(yintercept=mean), colour="#990000", linetype="dashed") +
    ggtitle(title)
}

skip <- function(delta) {
    10^ceiling(log(delta,10)-1)
}

customBreaks <- function(min, max)
{
    seq(min, max, skip(max-min))
}

calcValues <- function(n)
{
    vals <- rep(0,n)
    for(i in 1:n) {
        cat(paste0("Calculating... ",i,"/",n,"\n"))
        out <- system("./network < in1", intern = TRUE)
        vals[i] <- unlist(strsplit(out[21], " "))[3]
    }

    simpleBarGraph(1:n, as.numeric(vals), "Numer testu", 
                   "Niezawodność sieci", "Przykładowy tytuł")
}
