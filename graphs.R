#!/usr/bin/env Rscript
require(ggplot2)

monteCarloGraph <- function(xvals, yvals, xlabel, ylabel, precision, title) {
    m = mean(yvals)
    maxv = m + precision;
    minv = m - precision;
    frame = data.frame(x=xvals, y=yvals, mean = mean(yvals),
                       up=maxv, down=minv)
    xbreaks = customBreaks(min(xvals), max(xvals))
    ybreaks = customBreaks(min(yvals), max(yvals))

    ggplot(frame) +
    geom_bar(aes(x,y), stat="identity", fill="#00ffff", colour="#333333") +
    scale_x_continuous(breaks = xbreaks) +
    scale_y_continuous(breaks = ybreaks, labels = paste0(ybreaks, " %")) +
    coord_cartesian(ylim = c(min(yvals),max(yvals))) + 
    xlab(xlabel) +
    ylab(ylabel) +
    geom_hline(aes(yintercept=mean), colour="#990000", linetype="dashed") +
    geom_hline(aes(yintercept=up), colour="#990000", linetype="dashed") +
    geom_hline(aes(yintercept=down), colour="#990000", linetype="dashed") +
    ggtitle(title)
}

skip <- function(delta) {
    10^ceiling(log(delta,10)-1)
}

customBreaks <- function(min, max)
{
    seq(min, max, skip(max-min))
}

monteCarloTest <- function(n)
{
    vals <- rep(0,n)
    for(i in 1:n) {
        cat(paste0("Calculating... ",i,"/",n,"\n"))
        out <- system("./network < in1", intern = TRUE)
        vals[i] <- unlist(strsplit(out[21], " "))[3]
    }

    monteCarloGraph(1:n, as.numeric(vals), "Numer testu", 
                    "Niezawodność sieci", 0.1, "Przykładowy tytuł")
}

genInputString <- function(nodes, edges, completeness, genseed, algseed, 
                           minrel, maxrel, adjnum, maxi, conf, prec, relcolors)
{
    nl = "\n"
    options(scipen=10)
    paste0(nodes,nl,edges,nl,completeness,nl,genseed,nl,algseed,nl,minrel,nl,
           maxrel,nl,adjnum,nl,maxi,nl,conf,nl,prec,nl,relcolors,nl)
}

reliabilityTest <- function(steps)
{
    results <- c()
    iterations <- c()
    for( i in steps) {
        print(paste0("Calculating step: ",i,"..."))
        input <- genInputString(15,27,0,1912548602,0,i,i, 5, 1000000, 0.95,
                                 0.1,1)
        out <- system(paste0("echo \"",input,"\" | ./network"), intern = TRUE)
        results <- c(results, unlist(strsplit(out[21], " "))[3])
        iterations <- c(iterations, unlist(strsplit(out[22], " "))[3])
    }
    list(results,iterations)
}

reliabilityGraph <- function(steps, results, iterations)
{
    rf1 <- data.frame(x=steps, y=as.numeric(results), text="Niezawodność sieci (%)")
    rf2 <- data.frame(x=steps, y=as.numeric(iterations), text="Iteracje")
   
    rf <- rbind(rf1, rf2)

    ggplot(rf, aes(x,y)) + 
    facet_grid(text ~ ., scale = "free") +
    layer(data = rf1, geom = "line", color="blue") +
    layer(data = rf1, geom = "point", size=2, color="blue") +
    layer(data = rf1, geom = "point", size=1, color="white") +
    layer(data = rf2, geom = "line", color="red") +
    layer(data = rf2, geom = "point", size=2, color="red") +
    layer(data = rf2, geom = "point", size=1, color="white") +
    xlab("Prawdnopodobieństwo połączenia") +
    scale_x_continuous(breaks=seq(0,1,0.1)) +
    geom_area(alpha = 0.2) +
    ylab("")
}
