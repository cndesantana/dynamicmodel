### TO PLOT NUMBER OF INDIVIDUALS OF A SPECIES IN EACH SITE

getxmat<-function(x){x%%10}
getymat<-function(x){trunc((x-1)%/%10)}

getcolor<-function(x)
{
	ncolor<-y-z
	pallete<-blue2red(ncolor+1);
	pos<-round((x-z));
	return(pallete[pos+1]);
}

xmat<-sapply(1:100,getxmat);
ymat<-sapply(1:100,getymat)+1;

spe<-read.table("/Users/charlessantana/PhD/FoodWebModel/foodwebmodel_2011/output_species_004_seed_17_real_1_changes_0.dat")
spe<-as.matrix(spe)
size<-length(spe[1,]);
time<-length(spe[,1]);

for(i in 1:time)
{	
	y<-max(spe[i,]);
	z<-min(spe[i,]);
	nowdata<-as.numeric(spe[i,]);
	plot(xmat,ymat,pch=19, col= sapply( nowdata, getcolor), main=paste("Time = ",i,sep=""))
	ani.pause();
}



###### PLOT TOTAL NUMBER OF INDIVIDUALS OF ALL SPECIES IN THE TIME
library(colorRamps)
library(animation)
directory<-c("Results_30bloques9_5000","Results_30bloques9_5000","Results_9species_10000/100_and_1000_Individuals","Results_9species_10000/10000_Individuals","Results_75Dunne_1000");
files<-c("AverIndInTime_50Real_10Ind","AverIndInTime_50Real_1000Ind","AverIndInTime_30Real_100_1000","AverIndInTime_9species_30Real_10000","AverIndInTime_75Dunne_1000");
realizations<-c(50,50,30,30,1);
init_ind<-c(10,1000,1000,10000,100);
nspecies<-c(30,30,9,9,75);

for(k in 1:length(files))
{
	path<-paste("/Users/charlessantana/PhD/FoodWebModel/foodwebmodel_2011_6/",directory[k],"/",sep="");
	ind<-read.table(paste(path,files[k],".dat",sep=""));
	ind<-as.matrix(ind);
	size<-length(ind[1,]);
	limite<-length(ind[,1]);
	maximum<-max(ind);
	postscript(paste(path,files[k],".eps",sep=""))
	plot(ind[,2],main=paste("Individuals in Time in a food web with ",nspecies[k]," species",sep=""),sub=paste("Average of ",realizations[k]," Realizations starting with maximum of ",init_ind[k]," individuals of each species",sep=""),ylab="Number of Individuals",xlab="Time",type="l",ylim=c(0,maximum*1.5),xlim=c(0,limite))
	for(i in 3:size)
	{
		points(ind[,i],col=blue2red(size)[i-1],type="l")
	}
	dev.off();
}

