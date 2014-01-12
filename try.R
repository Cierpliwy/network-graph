#!/usr/bin/env Rscript
require(plotrix)
source("graphs.R")

generate <- function(n) {
    res <- NULL

    for (i in 1:n) {
        res <- c(res, system("./network < in0 | grep Resu | cut -d ' ' -f 3", intern=TRUE))
    }

    as.numeric(res)
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

confidenceTest <- function(conf, prec, iter=200, nodes=15, edges=27, 
        genSeed=1203238904, maxIter=1000000) {
    rArr <- c(0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,0.99)
    passed=TRUE
   
    cat(">>> CONFIDENCE TEST <<<\n")
    cat("Each parameters will be generate ", iter,"samples\n")
    cat("Nodes: ", nodes, " edges: ", edges,"\n")
    cat("Generation seed: ", genSeed, " maximum iterations: ", maxIter,"\n")
    cat("Confidence: ", conf, " precision: ", prec,"\n")
    cat("Link reliability: ", rArr,"\n\n")
    for(r in rArr){
        input <- genInputString(nodes,edges,0,genSeed,0,r,r, 5, maxIter, conf, prec,1)
                              
        data <- NULL
        for (i in 1:iter) {
            data <- c(data, system(paste0("echo \"",input,"\" | ./network | grep Result: | cut -d ' ' -f 3"), intern=TRUE))
        }
        data <- as.numeric(data)
        cat("Calculating ",iter," samples, link reliability ",r,"\n")
        rconf = confidence(data, prec)
        
        #cat("Give condidence=", conf," Real confidence=",rconf,"\n")
        if(conf > rconf) {
            passed = FALSE;
            
            cat("FAILED : conf", conf,"real_conf=", rconf, " link reliability", r, "\n")
        } else {
            cat("PASSED : conf=", conf,"real_conf=", rconf,"\n")
        }
    }
    
    if(passed) {
        cat("+++ Confidence test PASSED\n");
    } else {
        cat("--- Confidence test FAILED\n");
    }
}

confidence <- function(vals, w) { 
    avg <- mean(vals)

    good <-0
    for(j in 1:length(vals)) {
        if((avg > vals[j]-w) && (avg < vals[j]+w)) {
            good<-good+1
        }
    }

    good/length(vals)
}

intervals <- function(vals) {
    t.test(vals, conf.level = 0.99)
    t.test(vals, conf.level = 0.95)
    t.test(vals, conf.level = 0.90)
}

