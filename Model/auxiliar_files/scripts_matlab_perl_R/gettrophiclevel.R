library(igraph)

path<-"/Users/charlessantana/PhD/FoodWebModel/foodwebmodel_2011/"
webs<-c("fwf1","fwf2","fwf3")
for(w in 1:length(webs)){
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
}

