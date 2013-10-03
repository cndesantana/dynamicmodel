library(igraph)
path<-"/Users/charlessantana/PhD/FoodWebModel/foodwebmodel_2011"
webs<-c("rede")
for(w in 1:length(webs)){
        g1<-read.graph(paste(path,webs[w],sep=""),format="pajek")
        vert<-V(g1)
        degdist_in<-function(v){return(degree(g1,mode="in",v))}
        dist_in<-sapply(vert,degdist_in)
        basal<-which(dist_in==0)
        vert<-V(g1)+1
        distance<-function(j){shortest.paths(g1,v=i-1,mode="out")[j]}
        distancematrix<-list()
        for(l in 1:length(basal)){i<-basal[l];distancematrix<-rbind(distancematrix,(sapply(1:30,distance)))}
        levels<-array(0,dim=30)
        for(i in 1:30){levels[i]<-max(as.numeric(distancematrix[which(as.numeric(distancematrix[,i])!=Inf),i]))}
        hist(levels)
}
path<-"/Users/charlessantana/PhD/FoodWebModel/foodwebmodel_2011/"
webs<-c("rede")
for(w in 1:length(webs)){
        g1<-read.graph(paste(path,webs[w],sep=""),format="pajek")
        vert<-V(g1)
        degdist_in<-function(v){return(degree(g1,mode="in",v))}
        dist_in<-sapply(vert,degdist_in)
        basal<-which(dist_in==0)
        vert<-V(g1)+1
        distance<-function(j){shortest.paths(g1,v=i-1,mode="out")[j]}
        distancematrix<-list()
        for(l in 1:length(basal)){i<-basal[l];distancematrix<-rbind(distancematrix,(sapply(1:30,distance)))}
        levels<-array(0,dim=30)
        for(i in 1:30){levels[i]<-max(as.numeric(distancematrix[which(as.numeric(distancematrix[,i])!=Inf),i]))}
        hist(levels)
}
path
paste(path,webs[1])
path<-"/Users/charlessantana/PhD/FoodWebModel/foodwebmodel_2011/"
webs<-c("rede")
for(w in 1:length(webs)){
        g1<-read.graph(paste(path,webs[w],".net",sep=""),format="pajek")
        vert<-V(g1)
        degdist_in<-function(v){return(degree(g1,mode="in",v))}
        dist_in<-sapply(vert,degdist_in)
        basal<-which(dist_in==0)
        vert<-V(g1)+1
        distance<-function(j){shortest.paths(g1,v=i-1,mode="out")[j]}
        distancematrix<-list()
        for(l in 1:length(basal)){i<-basal[l];distancematrix<-rbind(distancematrix,(sapply(1:30,distance)))}
        levels<-array(0,dim=30)
        for(i in 1:30){levels[i]<-max(as.numeric(distancematrix[which(as.numeric(distancematrix[,i])!=Inf),i]))}
        hist(levels)
}
path<-"/Users/charlessantana/PhD/FoodWebModel/foodwebmodel_2011/"
webs<-c("rede")
for(w in 1:length(webs)){
        g1<-read.graph(paste(path,webs[w],".net",sep=""),format="pajek")
        vert<-V(g1)
        degdist_in<-function(v){return(degree(g1,mode="in",v))}
        dist_in<-sapply(vert,degdist_in)
        basal<-which(dist_in==0)
        vert<-V(g1)+1
        distance<-function(j){shortest.paths(g1,v=i-1,mode="out")[j]}
        distancematrix<-list()
        for(l in 1:length(basal)){i<-basal[l];distancematrix<-rbind(distancematrix,(sapply(1:30,distance)))}
        levels<-array(0,dim=30)
        for(i in 1:30){levels[i]<-max(as.numeric(distancematrix[which(as.numeric(distancematrix[,i])!=Inf),i]))}
        hist(levels,col="red")
}
path<-"/Users/charlessantana/PhD/FoodWebModel/foodwebmodel_2011/"
webs<-c("rede")
for(w in 1:length(webs)){
        g1<-read.graph(paste(path,webs[w],".net",sep=""),format="pajek")
        vert<-V(g1)
        degdist_in<-function(v){return(degree(g1,mode="in",v))}
        dist_in<-sapply(vert,degdist_in)
        basal<-which(dist_in==0)
        vert<-V(g1)+1
        distance<-function(j){shortest.paths(g1,v=i-1,mode="out")[j]}
        distancematrix<-list()
        for(l in 1:length(basal)){i<-basal[l];distancematrix<-rbind(distancematrix,(sapply(1:30,distance)))}
        levels<-array(0,dim=30)
        for(i in 1:30){levels[i]<-max(as.numeric(distancematrix[which(as.numeric(distancematrix[,i])!=Inf),i]))}
        hist(levels,col="red",freq=FALSE)
}
path<-"/Users/charlessantana/PhD/FoodWebModel/foodwebmodel_2011/"
webs<-c("rede")
for(w in 1:length(webs)){
        g1<-read.graph(paste(path,webs[w],".net",sep=""),format="pajek")
        vert<-V(g1)
        degdist_in<-function(v){return(degree(g1,mode="in",v))}
        dist_in<-sapply(vert,degdist_in)
        basal<-which(dist_in==0)
        vert<-V(g1)+1
        distance<-function(j){shortest.paths(g1,v=i-1,mode="out")[j]}
        distancematrix<-list()
        for(l in 1:length(basal)){i<-basal[l];distancematrix<-rbind(distancematrix,(sapply(1:30,distance)))}
        levels<-array(0,dim=30)
        for(i in 1:30){levels[i]<-max(as.numeric(distancematrix[which(as.numeric(distancematrix[,i])!=Inf),i]))}
        h<-hist(levels,col="red",freq=FALSE)
}
h
h$intensities
sum(h$intensities)
sum(h$density)
sum(h$density)/2
plot(h$density/2)
barplot(h$density/2)
        h<-hist(levels,col="red",freq=FALSE,nclass=4)
barplot(h$density/2)
h
        h<-hist(levels,col="red",freq=FALSE,nclass=5)
barplot(h$density/2)
h
h$densities/2
h$density/2
        h<-hist(levels,col="red",freq=FALSE,nclass=4)
h$density/2
        h<-hist(levels,col="red",freq=FALSE,nclass=5)
h$density/2
1000/15/
        h<-hist(levels,col="red",freq=FALSE,nclass=4)
h$density/2
        h<-hist(levels,col="red",freq=FALSE,nclass=5)
h$density/2
100/15/(30*0.2)
1000/15/(30*0.2)
1000/15/(30*(h$density/2)[6])
2000/15/(30*(h$density/2)[4])
4000/15/(30*(h$density/2)[2])
8000/15/(30*(h$density/2)[1])
levels
nind<-c(89,30,15,11)
levels+1
nind[levels+1]
for(i in 1:30)nind[levels+1][i]
for(i in 1:30){cat(nind[levels+1][i],sep="\n")}
for(i in 1:30){cat(paste(i,i,nind[levels+1][i],sep=" "),sep="\n")}
for(i in 1:30){cat(paste(i,i,0,0,0,0,nind[levels+1][i],sep=" "),sep="\n")}
library(igraph)
path<-"/Users/charlessantana/PhD/FoodWebModel/foodwebmodel_2011/"
webs<-c("fwf1","fwf2","fwf3")
w<-1
        g1<-read.graph(paste(path,webs[w],".net",sep=""),format="pajek")
        vert<-V(g1)
        degdist_in<-function(v){return(degree(g1,mode="in",v))}
        dist_in<-sapply(vert,degdist_in)
        basal<-which(dist_in==0)
        vert<-V(g1)+1
        distance<-function(j){shortest.paths(g1,v=i-1,mode="out")[j]}
        distancematrix<-list()
        for(l in 1:length(basal)){i<-basal[l];distancematrix<-rbind(distancematrix,(sapply(1:30,distance)))}
        levels<-array(0,dim=30)
        for(i in 1:30){levels[i]<-max(as.numeric(distancematrix[which(as.numeric(distancematrix[,i])!=Inf),i]))}
        h<-hist(levels,col="red",freq=FALSE)
basal
g12
g1
dist_in
degdist_in<-function(v){return(degree(g1,mode="in",v))}
degdist_in(1)
degdist_in(0)
dist_in
        g1<-read.graph(paste(path,webs[w],".net",sep=""),format="pajek")
        vert<-V(g1)
        degdist_in<-function(v){return(degree(g1,mode="in",v))}
        dist_in<-sapply(vert,degdist_in)
        basal<-which(dist_in==0)
basal
        vert<-V(g1)+1
        distance<-function(j){shortest.paths(g1,v=i-1,mode="out")[j]}
        distancematrix<-list()
        for(l in 1:length(basal)){i<-basal[l];distancematrix<-rbind(distancematrix,(sapply(1:30,distance)))}
        levels<-array(0,dim=30)
levels
        g1<-read.graph(paste(path,webs[w],".net",sep=""),format="pajek")
        vert<-V(g1)
        degdist_in<-function(v){return(degree(g1,mode="in",v))}
        dist_in<-sapply(vert,degdist_in)
        basal<-which(dist_in==0)
        vert<-V(g1)+1
        distance<-function(j){shortest.paths(g1,v=i-1,mode="out")[j]}
        distancematrix<-list()
        for(l in 1:length(basal)){i<-basal[l];distancematrix<-rbind(distancematrix,(sapply(1:length(V(g1)),distance)))}
        levels<-array(0,dim=length(V(g1)))
        for(i in 1:length(V(g1))){levels[i]<-max(as.numeric(distancematrix[which(as.numeric(distancematrix[,i])!=Inf),i]))}
levels
  h<-hist(levels,col="red",freq=FALSE)
h$density
h
h$density
sum(h$density
)
h$density/2
 1000/15/(75*0.4)
 2000/15/(75*0.5)
 4000/15/(75*0.01)
 4000/15/(75*0.09)
 8000/15/(75*0.01)
nind<-c(711,39,4,2)
for(i in 1:75){cat(paste(i,i,0,0,0,0,nind[levels+1][i],sep=" "),sep="\n")}
w<-2
        g1<-read.graph(paste(path,webs[w],".net",sep=""),format="pajek")
        vert<-V(g1)
        degdist_in<-function(v){return(degree(g1,mode="in",v))}
        dist_in<-sapply(vert,degdist_in)
        basal<-which(dist_in==0)
        vert<-V(g1)+1
        distance<-function(j){shortest.paths(g1,v=i-1,mode="out")[j]}
        distancematrix<-list()
        for(l in 1:length(basal)){i<-basal[l];distancematrix<-rbind(distancematrix,(sapply(1:length(V(g1)),distance)))}
        levels<-array(0,dim=length(V(g1)))
        for(i in 1:length(V(g1))){levels[i]<-max(as.numeric(distancematrix[which(as.numeric(distancematrix[,i])!=Inf),i]))}
        h<-hist(levels,col="red",freq=FALSE)
h$density/2
h
        h<-hist(levels,col="red",freq=FALSE,nclass=5)
h
        h<-hist(levels,col="red",freq=FALSE,nclass=6)
h
barplot(h$density/2)
h$density/2
length(V(g1))
 1000/15/(778*0.05)
 2000/15/(778*0.08)
 4000/15/(778*0.1)
 8000/15/(778*0.71)
nind<-c(1,3,2,2)
for(i in 1:778){cat(paste(i,i,0,0,0,0,nind[levels+1][i],sep=" "),sep="\n")}
levels
 1000/31/(778*0.05)
h$density/2
 1000/31/(778*0.04)
 2000/31/(778*0.05)
 4000/31/(778*0.08)
 8000/31/(778*0.1)
 16000/31/(778*0.71)
nind<-c(1,3,2,2,1)
for(i in 1:778){cat(paste(i,i,0,0,0,0,nind[levels+1][i],sep=" "),sep="\n")}
w<-3
        g1<-read.graph(paste(path,webs[w],".net",sep=""),format="pajek")
        vert<-V(g1)
        degdist_in<-function(v){return(degree(g1,mode="in",v))}
        dist_in<-sapply(vert,degdist_in)
        basal<-which(dist_in==0)
        vert<-V(g1)+1
        distance<-function(j){shortest.paths(g1,v=i-1,mode="out")[j]}
        distancematrix<-list()
        for(l in 1:length(basal)){i<-basal[l];distancematrix<-rbind(distancematrix,(sapply(1:length(V(g1)),distance)))}
        levels<-array(0,dim=length(V(g1)))
        for(i in 1:length(V(g1))){levels[i]<-max(as.numeric(distancematrix[which(as.numeric(distancematrix[,i])!=Inf),i]))}
        h<-hist(levels,col="red",freq=FALSE)
h
h$density/2
length(V(g1))
 1000/31/(145*0.1)
 1000/31/(145*0.36)
 2000/31/(145*0.36)
 4000/31/(145*0.19)
 8000/31/(145*0.18)
 16000/31/(145*0.15)
nind<-c(24,10,5,1,2)
for(i in 1:778){cat(paste(i,i,0,0,0,0,nind[levels+1][i],sep=" "),sep="\n")}
nind
levels
max(levels)
for(i in 1:145){cat(paste(i,i,0,0,0,0,nind[levels+1][i],sep=" "),sep="\n")}
nvert<-length(V(g1))
for(i in 1:nvert){cat(paste(i,i,0,0,0,0,nind[levels+1][i],sep=" "),sep="\n")}
savehistory("/Users/charlessantana/PhD/FoodWebModel/foodwebmodel_2011/script_number_individuals.R")
