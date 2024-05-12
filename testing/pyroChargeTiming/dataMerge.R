#Purpose: Plotting and statistics of pyro initiator testing data
#Date: 2024-05-10
#Author: Tom Powell
library(tidyverse)
library(ggplot2)
aoi_start <- 4500000
aoi_end <- 17000000
dataDirectory <- "C:/Users/Tom Powell/Documents/pyroTestCSVs"
setwd(dataDirectory)
#this is required to prevent column size mismatch errors.
fakeCol = seq(1, 20000000)
data <- data.frame(fakeCol)
file_count = 0
nameAcc = c()
for (csvFile in list.files(pattern="\\.csv", ignore.case=TRUE)) {
  dataTemp <- read.csv(paste(dataDirectory,csvFile, sep="/"), skip=5)
  ctname <- paste("t", file_count, "t", sep="")
  cvname <- paste("t", file_count, "v", sep="")
  data <- cbind(data, dataTemp$Second)
  data <- cbind(data, dataTemp$Volt)
  nameAcc <- c(nameAcc, ctname, cvname)
  file_count = file_count+1
  print(paste("Processed file",file_count))
} 
nameAcc <- append("id", nameAcc)
colnames(data) <- nameAcc
dataTrim <- data[seq(aoi_start,aoi_end),]
write.csv(dataTrim, "processed.csv", row.names=FALSE)