load("/atomdisk0/csantana/prueba/outputs/dtr/ensemblemeandtr_spring.Rdat");
load("/atomdisk0/csantana/prueba/outputs/dtr/ensemblemeandtr_winter.Rdat");
load("/atomdisk0/csantana/prueba/outputs/dtr/ensemblemeandtr_autum.Rdat");
load("/atomdisk0/csantana/prueba/outputs/dtr/ensemblemeandtr_summer.Rdat");

dtr_perkins<-array(NA,dim=c(4,4,1445));

seasonslist<-list(ensemblemeancalibdtr_winter, ensemblemeancalibdtr_spring, ensemblemeancalibdtr_summer, ensemblemeancalibdtr_autum);
names(seasonslist)<-c("Winter","Spring","Summer","Autum")
MAXLIST<-c(max(seasonslist$Winter),max(seasonslist$Spring),max(seasonslist$Summer),max(seasonslist$Autum));
MINLIST<-c(min(seasonslist$Winter),min(seasonslist$Spring),min(seasonslist$Summer),min(seasonslist$Autum));

#########################################

	MAXIMUM<-MAXLIST[1];
	MINIMUM<-MINLIST[1];
	for(isite in 1:1445)
	{
		present_seasondist<-density(seasonslist$Winter[1,isite,],from=MINIMUM,to=MAXIMUM); 
		sum_present<-sum(present_seasondist$y);
   		for(idec in 1:4)
   		{
   	          future_seasondist<-density(seasonslist$Winter[idec+1,isite,],from=MINIMUM,to=MAXIMUM); 
   	          sum_future<-sum(future_seasondist$y);
  	          dtr_perkins_presfut<-0.0;
  	          for(i in 1:length(present_seasondist$y))
  	          {
  	              dtr_perkins_presfut<-c(dtr_perkins_presfut,min((future_seasondist$y[i])/sum_future,(present_seasondist$y[i])/sum_present));
  	          }
  	          dtr_perkins[1,idec,isite]<-sum(dtr_perkins_presfut);
	    }
	}


	MAXIMUM<-MAXLIST[2];
	MINIMUM<-MINLIST[2];
	for(isite in 1:1445)
	{
		present_seasondist<-density(seasonslist$Spring[1,isite,],from=MINIMUM,to=MAXIMUM); 
		sum_present<-sum(present_seasondist$y);
   		for(idec in 1:4)
   		{
   	          future_seasondist<-density(seasonslist$Spring[idec+1,isite,],from=MINIMUM,to=MAXIMUM); 
   	          sum_future<-sum(future_seasondist$y);
  	          dtr_perkins_presfut<-0.0;
  	          for(i in 1:length(present_seasondist$y))
  	          {
  	              dtr_perkins_presfut<-c(dtr_perkins_presfut,min((future_seasondist$y[i])/sum_future,(present_seasondist$y[i])/sum_present));
  	          }
  	          dtr_perkins[2,idec,isite]<-sum(dtr_perkins_presfut);
	    }
	}

	MAXIMUM<-MAXLIST[3];
	MINIMUM<-MINLIST[3];
	for(isite in 1:1445)
	{
		present_seasondist<-density(seasonslist$Summer[1,isite,],from=MINIMUM,to=MAXIMUM); 
		sum_present<-sum(present_seasondist$y);
   		for(idec in 1:4)
   		{
   	          future_seasondist<-density(seasonslist$Summer[idec+1,isite,],from=MINIMUM,to=MAXIMUM); 
   	          sum_future<-sum(future_seasondist$y);
  	          dtr_perkins_presfut<-0.0;
  	          for(i in 1:length(present_seasondist$y))
  	          {
  	              dtr_perkins_presfut<-c(dtr_perkins_presfut,min((future_seasondist$y[i])/sum_future,(present_seasondist$y[i])/sum_present));
  	          }
  	          dtr_perkins[3,idec,isite]<-sum(dtr_perkins_presfut);
	    }
	}


	MAXIMUM<-MAXLIST[4];
	MINIMUM<-MINLIST[4];
	for(isite in 1:1445)
	{
		present_seasondist<-density(seasonslist$Autum[1,isite,],from=MINIMUM,to=MAXIMUM); 
		sum_present<-sum(present_seasondist$y);
   		for(idec in 1:4)
   		{
   	          future_seasondist<-density(seasonslist$Autum[idec+1,isite,],from=MINIMUM,to=MAXIMUM); 
   	          sum_future<-sum(future_seasondist$y);
  	          dtr_perkins_presfut<-0.0;
  	          for(i in 1:length(present_seasondist$y))
  	          {
  	              dtr_perkins_presfut<-c(dtr_perkins_presfut,min((future_seasondist$y[i])/sum_future,(present_seasondist$y[i])/sum_present));
  	          }
  	          dtr_perkins[4,idec,isite]<-sum(dtr_perkins_presfut);
	    }
	}

#############################################

save(dtr_perkins,file="/atomdisk0/csantana/prueba/outputs/dtr/dtr_perkins_seasons.Rdat");

seasonnames<-c("Winter","Spring","Summer","Autum");
yearsini<-c(2011,2031,2051,2071);
yearsend<-c(2030,2050,2070,2090);

library(beanplot)
library(colorRamps)
load("/atomdisk0/csantana/prueba/Data/Observed_Data/coords_estcena_pr.Rdat")
source("/atomdisk0/csantana/prueba/scripts_Victor/Rscripts/Paper/legend_function.R")

yearsini<-c(1981,2011,2031,2051,2071)
yearsend<-c(2000,2030,2050,2070,2090)

color<-function(x){
	MAX<-0.5;MIN<-0;
	if(x > MAX){x <- MAX}
	if(x < MIN){x <- MIN}
	colors<-c(blue2green(20)[1:15],cyan2yellow(40)[25:40],blue2green2red(40)[31:40]);
	pos<-round(40*(x-MIN)/(MAX-MIN))	
	return(colors[pos+1])
}

for(iseason in 1:4)
{
	for(idec in 1:4)
	{	
		png(paste("/hosts/atomdisk0/csantana/prueba/outputs/figures/paper/dtrdist_changes_",seasonnames[iseason],"_2000_",yearsend[idec+1],".png",sep=""));
		plot(coords_estcena, col=sapply(1-dtr_perkins[iseason,idec,],color), pch=19, main= paste(seasonnames[iseason]," - 2000:",yearsend[idec+1],sep=""),xlab="",ylab="");
		estimate.legend.size("outbottom",legend=c(5*(0:10)),cex=0.8, col=sapply(0.05*(0:10),color),pch=20);
		place.legend("outbottom",legend=c(5*(0:10)),cex=0.8, col=sapply(0.05*(0:10),color),pch=20,title="Changes of Daily Temperature Range (%)");
		dev.off()
		postscript(paste("/hosts/atomdisk0/csantana/prueba/outputs/figures/paper/dtrdist_changes_",seasonnames[iseason],"_2000_",yearsend[idec+1],".eps",sep=""));
		plot(coords_estcena, col=sapply(1-dtr_perkins[iseason,idec,],color), pch=19, main= paste(seasonnames[iseason]," - 2000:",yearsend[idec+1],sep=""),xlab="",ylab="");
		estimate.legend.size("outbottom",legend=c(5*(0:10)),cex=0.8, col=sapply(0.05*(0:10),color),pch=20);
		place.legend("outbottom",legend=c(5*(0:10)),cex=0.8, col=sapply(0.05*(0:10),color),pch=20,title="Changes of Daily Temperature Range (%)");
		dev.off()
	}
}



##########


load("/hosts/atomdisk0/csantana/prueba/outputs/precip/ensemblemean_drydays_seasons.Rdat");

seasonnames<-c("Winter","Spring","Summer","Autum");
yearsini<-c(2011,2031,2051,2071);
yearsend<-c(2030,2050,2070,2090);

library(beanplot)
library(colorRamps)
load("/atomdisk0/csantana/prueba/Data/Observed_Data/coords_estcena_pr.Rdat")
source("/atomdisk0/csantana/prueba/scripts_Victor/Rscripts/Paper/legend_function.R")

yearsini<-c(1981,2011,2031,2051,2071)
yearsend<-c(2000,2030,2050,2070,2090)

color<-function(x){
	MAX<-25;MIN<-0;
	if(x > MAX){x <- MAX}
	if(x < MIN){x <- MIN}
	colors<-c(blue2green(20)[1:15],cyan2yellow(40)[25:40],blue2green2red(40)[31:40]);
	pos<-round(40*(x-MIN)/(MAX-MIN))	
	return(colors[pos+1])
}


	for(idec in 1:4)
	{	
		png(paste("/hosts/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_changes_autum_2000_",yearsend[idec+1],".png",sep=""));
		colors<-sapply(ensemblemean_drydays_decadesautum[idec+1,]/20.-ensemblemean_drydays_decadesautum[1,]/20.,color);
		plot(coords_estcena, col=colors, pch=19, main= paste("Autum - 2000:",yearsend[idec+1],sep=""),xlab="",ylab="");
		estimate.legend.size("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20);
		place.legend("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20,title="Changes of Number of Dry Days (days)");
		dev.off()
		postscript(paste("/hosts/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_changes_autum_2000_",yearsend[idec+1],".eps",sep=""));
		plot(coords_estcena, col=colors, pch=19, main= paste("Autum - 2000:",yearsend[idec+1],sep=""),xlab="",ylab="");
		estimate.legend.size("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20);
		place.legend("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20,title="Changes of Number of Dry Days (days)");
		dev.off()
		
		png(paste("/hosts/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_changes_winter_2000_",yearsend[idec+1],".png",sep=""));
		colors<-sapply(ensemblemean_drydays_decadeswinter[idec+1,]/20.-ensemblemean_drydays_decadeswinter[1,]/20.,color);
		plot(coords_estcena, col=colors, pch=19, main= paste("Winter - 2000:",yearsend[idec+1],sep=""),xlab="",ylab="");
		estimate.legend.size("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20);
		place.legend("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20,title="Changes of Number of Dry Days (days)");
		dev.off()
		postscript(paste("/hosts/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_changes_winter_2000_",yearsend[idec+1],".eps",sep=""));
		plot(coords_estcena, col=colors, pch=19, main= paste("Winter - 2000:",yearsend[idec+1],sep=""),xlab="",ylab="");
		estimate.legend.size("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20);
		place.legend("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20,title="Changes of Number of Dry Days (days)");
		dev.off()
		
		png(paste("/hosts/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_changes_summer_2000_",yearsend[idec+1],".png",sep=""));
		colors<-sapply(ensemblemean_drydays_decadessummer[idec+1,]/20.-ensemblemean_drydays_decadessummer[1,]/20.,color);
		plot(coords_estcena, col=colors, pch=19, main= paste("Summer - 2000:",yearsend[idec+1],sep=""),xlab="",ylab="");
		estimate.legend.size("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20);
		place.legend("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20,title="Changes of Number of Dry Days (days)");
		dev.off()
		postscript(paste("/hosts/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_changes_summer_2000_",yearsend[idec+1],".eps",sep=""));
		plot(coords_estcena, col=colors, pch=19, main= paste("Summer - 2000:",yearsend[idec+1],sep=""),xlab="",ylab="");
		estimate.legend.size("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20);
		place.legend("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20,title="Changes of Number of Dry Days (days)");
		dev.off()

		png(paste("/hosts/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_changes_spring_2000_",yearsend[idec+1],".png",sep=""));
		colors<-sapply(ensemblemean_drydays_decadesspring[idec+1,]/20.-ensemblemean_drydays_decadesspring[1,]/20.,color);
		plot(coords_estcena, col=colors, pch=19, main= paste("Spring - 2000:",yearsend[idec+1],sep=""),xlab="",ylab="");
		estimate.legend.size("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20);
		place.legend("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20,title="Changes of Number of Dry Days (days)");
		dev.off()
		postscript(paste("/hosts/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_changes_spring_2000_",yearsend[idec+1],".eps",sep=""));
		plot(coords_estcena, col=colors, pch=19, main= paste("Spring - 2000:",yearsend[idec+1],sep=""),xlab="",ylab="");
		estimate.legend.size("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20);
		place.legend("outbottom",legend=c(2.5*(0:10)),cex=0.8, col=sapply(2.5*(0:10),color),pch=20,title="Changes of Number of Dry Days (days)");
		dev.off()
	}





##########


load("/hosts/atomdisk0/csantana/prueba/outputs/tasmax/percent95_winter.Rdat")
load("/hosts/atomdisk0/csantana/prueba/outputs/tasmax/percent95_summer.Rdat")
load("/hosts/atomdisk0/csantana/prueba/outputs/tasmax/percent95_spring.Rdat")
load("/hosts/atomdisk0/csantana/prueba/outputs/tasmax/percent95_autum.Rdat")

ensemblemean_tasmax95_winter<-array(NA,dim=c(5,1445))
calc_ensemblemeanwinter<-function(isite)
{
	mean(tasmax95_winter[idec,-8,isite])
}
for(idec in 1:5)
{
		ensemblemean_tasmax95_winter[idec,]<-sapply(1:1445,calc_ensemblemeanwinter)
}


ensemblemean_tasmax95_autum<-array(NA,dim=c(5,1445))
calc_ensemblemeanautum<-function(isite)
{
	mean(tasmax95_autum[idec,-8,isite])
}
for(idec in 1:5)
{
		ensemblemean_tasmax95_autum[idec,]<-sapply(1:1445,calc_ensemblemeanautum)
}

ensemblemean_tasmax95_summer<-array(NA,dim=c(5,1445))
calc_ensemblemeansummer<-function(isite)
{
	mean(tasmax95_summer[idec,-8,isite])
}
for(idec in 1:5)
{
		ensemblemean_tasmax95_summer[idec,]<-sapply(1:1445,calc_ensemblemeansummer)
}

ensemblemean_tasmax95_spring<-array(NA,dim=c(5,1445))
calc_ensemblemeanspring<-function(isite)
{
	mean(tasmax95_spring[idec,-8,isite])
}
for(idec in 1:5)
{
		ensemblemean_tasmax95_spring[idec,]<-sapply(1:1445,calc_ensemblemeanspring)
}

save(ensemblemean_tasmax95_winter,ensemblemean_tasmax95_autum,ensemblemean_tasmax95_summer,ensemblemean_tasmax95_spring,file="/hosts/atomdisk0/csantana/prueba/outputs/tasmax/ensemblemean_tasmax95_seasons.Rdat")


load("/hosts/atomdisk0/csantana/prueba/outputs/dtr/meandtr_seasons.Rdat")
load("/hosts/atomdisk0/csantana/prueba/outputs/tasmin/ensemblemean_tasmin05_seasons.Rdat")
load("/hosts/atomdisk0/csantana/prueba/outputs/tasmax/ensemblemean_tasmax95_seasons.Rdat")

color<-function(x){
	MAX<-3;MIN<--1;
	if(x > MAX){x <- MAX}
	if(x < MIN){x <- MIN}
	colors<-c(blue2green(20)[1:15],cyan2yellow(40)[22:40],blue2green2red(30)[23:30]);
	pos<-round(40*(x-MIN)/(MAX-MIN))	
	return(colors[pos+1])
}

library(beanplot)
library(colorRamps)
load("/atomdisk0/csantana/prueba/Data/Observed_Data/coords_estcena_pr.Rdat")
source("/atomdisk0/csantana/prueba/scripts_Victor/Rscripts/Paper/legend_function.R")

yearsini<-c(1981,2011,2031,2051,2071)
yearsend<-c(2000,2030,2050,2070,2090)


for(idec in 1:4){
	
png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_winter_changes_1981_",yearsini[idec+1], yearsend[idec+1],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter Changes"), col=sapply(ensemblemean_tasmin05_winter[idec+1,]-ensemblemean_tasmin05_winter[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Minimum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_winter_changes_1981_",yearsini[idec+1], yearsend[idec+1],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter Changes"), col=sapply(ensemblemean_tasmin05_winter[idec+1,]-ensemblemean_tasmin05_winter[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Minimum Temperature (oC)");
dev.off()
png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_autum_changes_1981_",yearsini[idec+1], yearsend[idec+1],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum Changes"), col=sapply(ensemblemean_tasmin05_autum[idec+1,]-ensemblemean_tasmin05_autum[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Minimum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_autum_changes_1981_",yearsini[idec+1], yearsend[idec+1],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum Changes"), col=sapply(ensemblemean_tasmin05_autum[idec+1,]-ensemblemean_tasmin05_autum[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Minimum Temperature (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_summer_changes_1981_",yearsini[idec+1], yearsend[idec+1],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer Changes"), col=sapply(ensemblemean_tasmin05_summer[idec+1,]-ensemblemean_tasmin05_summer[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Minimum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_summer_changes_1981_",yearsini[idec+1], yearsend[idec+1],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer Changes"), col=sapply(ensemblemean_tasmin05_summer[idec+1,]-ensemblemean_tasmin05_summer[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Minimum Temperature (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_spring_changes_1981_",yearsini[idec+1], yearsend[idec+1],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring Changes"), col=sapply(ensemblemean_tasmin05_spring[idec+1,]-ensemblemean_tasmin05_spring[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Minimum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_spring_changes_1981_",yearsini[idec+1], yearsend[idec+1],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring Changes"), col=sapply(ensemblemean_tasmin05_spring[idec+1,]-ensemblemean_tasmin05_spring[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Minimum Temperature (oC)");
dev.off()






png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/dtr_winter_changes_1981_",yearsini[idec+1], yearsend[idec+1],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter Changes"), col=sapply(meandtr_winter[idec+1,]-meandtr_winter[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Daily Temperature Range (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/dtr_winter_changes_1981_",yearsini[idec+1], yearsend[idec+1],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter Changes"), col=sapply(meandtr_winter[idec+1,]-meandtr_winter[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Daily Temperature Range (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/dtr_autum_changes_1981_",yearsini[idec+1], yearsend[idec+1],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum Changes"), col=sapply(meandtr_autum[idec+1,]-meandtr_autum[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Daily Temperature Range (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/dtr_autum_changes_1981_",yearsini[idec+1], yearsend[idec+1],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum Changes"), col=sapply(meandtr_autum[idec+1,]-meandtr_autum[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Daily Temperature Range (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/dtr_summer_changes_1981_",yearsini[idec+1], yearsend[idec+1],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer Changes"), col=sapply(meandtr_summer[idec+1,]-meandtr_summer[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Daily Temperature Range (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/dtr_summer_changes_1981_",yearsini[idec+1], yearsend[idec+1],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer Changes"), col=sapply(meandtr_summer[idec+1,]-meandtr_summer[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Daily Temperature Range (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/dtr_spring_changes_1981_",yearsini[idec+1], yearsend[idec+1],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring Changes"), col=sapply(meandtr_spring[idec+1,]-meandtr_spring[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Daily Temperature Range (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/dtr_spring_changes_1981_",yearsini[idec+1], yearsend[idec+1],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring Changes"), col=sapply(meandtr_spring[idec+1,]-meandtr_spring[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Daily Temperature Range (oC)");
dev.off()







png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmax95_winter_changes_1981_",yearsini[idec+1], yearsend[idec+1],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter Changes"), col=sapply(ensemblemean_tasmax95_winter[idec+1,]-ensemblemean_tasmax95_winter[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Maximum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmax95_winter_changes_1981_",yearsini[idec+1], yearsend[idec+1],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter Changes"), col=sapply(ensemblemean_tasmax95_winter[idec+1,]-ensemblemean_tasmax95_winter[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Maximum Temperature (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmax95_autum_changes_1981_",yearsini[idec+1], yearsend[idec+1],".png",sep=""));
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum Changes"), col=sapply(ensemblemean_tasmax95_autum[idec+1,]-ensemblemean_tasmax95_autum[1,],color));
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Maximum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmax95_autum_changes_1981_",yearsini[idec+1], yearsend[idec+1],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum Changes"), col=sapply(ensemblemean_tasmax95_autum[idec+1,]-ensemblemean_tasmax95_autum[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Maximum Temperature (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmax95_summer_changes_1981_",yearsini[idec+1], yearsend[idec+1],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer Changes"), col=sapply(ensemblemean_tasmax95_summer[idec+1,]-ensemblemean_tasmax95_summer[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Maximum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmax95_summer_changes_1981_",yearsini[idec+1], yearsend[idec+1],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer Changes"), col=sapply(ensemblemean_tasmax95_summer[idec+1,]-ensemblemean_tasmax95_summer[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Maximum Temperature (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmax95_spring_changes_1981_",yearsini[idec+1], yearsend[idec+1],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring Changes"), col=sapply(ensemblemean_tasmax95_spring[idec+1,]-ensemblemean_tasmax95_spring[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Maximum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmax95_spring_changes_1981_",yearsini[idec+1], yearsend[idec+1],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring Changes"), col=sapply(ensemblemean_tasmax95_spring[idec+1,]-ensemblemean_tasmax95_spring[1,],color))
estimate.legend.size("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20);
place.legend("outbottom",legend=c(-1+0.5*(0:8)),cex=0.8, col=sapply(-1+0.5*(0:8),color),pch=20,title="Changes of Maximum Temperature (oC)");
dev.off()

}







##############  T A S M I N    0 5 %



load("percent05_winter.Rdat")
load("percent05_summer.Rdat")
load("percent05_spring.Rdat")
load("percent05_autum.Rdat")

ensemblemean_tasmin05_winter<-array(NA,dim=c(5,1445))
calc_ensemblemeanwinter<-function(isite)
{
	mean(tasmin05_winter[idec,-8,isite])
}
for(idec in 1:5)
{
		ensemblemean_tasmin05_winter[idec,]<-sapply(1:1445,calc_ensemblemeanwinter)
}


ensemblemean_tasmin05_autum<-array(NA,dim=c(5,1445))
calc_ensemblemeanautum<-function(isite)
{
	mean(tasmin05_autum[idec,-8,isite])
}
for(idec in 1:5)
{
		ensemblemean_tasmin05_autum[idec,]<-sapply(1:1445,calc_ensemblemeanautum)
}

ensemblemean_tasmin05_summer<-array(NA,dim=c(5,1445))
calc_ensemblemeansummer<-function(isite)
{
	mean(tasmin05_summer[idec,-8,isite])
}
for(idec in 1:5)
{
		ensemblemean_tasmin05_summer[idec,]<-sapply(1:1445,calc_ensemblemeansummer)
}

ensemblemean_tasmin05_spring<-array(NA,dim=c(5,1445))
calc_ensemblemeanspring<-function(isite)
{
	mean(tasmin05_spring[idec,-8,isite])
}
for(idec in 1:5)
{
		ensemblemean_tasmin05_spring[idec,]<-sapply(1:1445,calc_ensemblemeanspring)
}

save(ensemblemean_tasmin05_winter,ensemblemean_tasmin05_autum,ensemblemean_tasmin05_summer,ensemblemean_tasmin05_spring,file="/hosts/atomdisk0/csantana/prueba/outputs/precip/ensemblemean_tasmin05_seasons.Rdat")

library(beanplot)
library(colorRamps)
load("/atomdisk0/csantana/prueba/Data/Observed_Data/coords_estcena_pr.Rdat")
source("/atomdisk0/csantana/prueba/scripts_Victor/Rscripts/Paper/legend_function.R")

 beanplot(ensemblemean_tasmin05_winter[idec,],ensemblemean_tasmin05_autum[idec,],ensemblemean_tasmin05_summer[idec,],ensemblemean_tasmin05_spring[idec,])


color<-function(x)
{
	MAX<-15;MIN<--10;
	if(x > MAX){x <- MAX}
	if(x < MIN){x <- MIN}
	colors<-blue2green2red(21);
	pos<-round(20*(x-MIN)/(MAX-MIN))	
	return(colors[pos+1])
}

yearsini<-c(1981,2011,2031,2051,2071)
yearsend<-c(2000,2030,2050,2070,2090)

for(idec in 1:5)
{
png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_wintertasmin_,sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_tasmin05_winter[idec,],color))
estimate.legend.size("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20,title="Percent 05 of Minimum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_winter_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_tasmin05_winter[1,],color))
estimate.legend.size("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20,title="Percent 05 of Minimum Temperature (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_autum_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_tasmin05_autum[idec,],color))
estimate.legend.size("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20,title="Percent 05 of Minimum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_autum_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_tasmin05_autum[idec,],color))
estimate.legend.size("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20,title="Percent 05 of Minimum Temperature (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_summer_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_tasmin05_summer[idec,],color))
estimate.legend.size("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20,title="Percent 05 of Minimum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_summer_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_tasmin05_summer[idec,],color))
estimate.legend.size("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20,title="Percent 05 of Minimum Temperature (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_spring_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_tasmin05_spring[idec,],color))
estimate.legend.size("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20,title="Percent 05 of Minimum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/tasmin05_spring_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_tasmin05_spring[idec,],color))
estimate.legend.size("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(0.3*(0:10)),cex=0.8,col=sapply(0.3*(0:10),color),pch=20,title="Percent 05 of Minimum Temperature (oC)");
dev.off()
}





########## D R Y     D A Y S 




load("percent95_winter.Rdat")
load("percent95_summer.Rdat")
load("percent95_spring.Rdat")
load("percent95_autum.Rdat")

ensemblemean_drydays_decadeswinter<-array(NA,dim=c(5,1445))
calc_ensemblemeanwinter<-function(isite)
{
	mean(drydays_decadeswinter[idec,-8,isite])
}
for(idec in 1:5)
{
		ensemblemean_drydays_decadeswinter[idec,]<-sapply(1:1445,calc_ensemblemeanwinter)
}


ensemblemean_drydays_decadesautum<-array(NA,dim=c(5,1445))
calc_ensemblemeanautum<-function(isite)
{
	mean(drydays_decadesautum[idec,-8,isite])
}
for(idec in 1:5)
{
		ensemblemean_drydays_decadesautum[idec,]<-sapply(1:1445,calc_ensemblemeanautum)
}

ensemblemean_drydays_decadessummer<-array(NA,dim=c(5,1445))
calc_ensemblemeansummer<-function(isite)
{
	mean(drydays_decadessummer[idec,-8,isite])
}
for(idec in 1:5)
{
		ensemblemean_drydays_decadessummer[idec,]<-sapply(1:1445,calc_ensemblemeansummer)
}

ensemblemean_drydays_decadesspring<-array(NA,dim=c(5,1445))
calc_ensemblemeanspring<-function(isite)
{
	mean(drydays_decadesspring[idec,-8,isite])
}
for(idec in 1:5)
{
		ensemblemean_drydays_decadesspring[idec,]<-sapply(1:1445,calc_ensemblemeanspring)
}

save(ensemblemean_drydays_decadeswinter,ensemblemean_drydays_decadesautum,ensemblemean_drydays_decadessummer,ensemblemean_drydays_decadesspring,file="/hosts/atomdisk0/csantana/prueba/outputs/precip/ensemblemean_drydays_seasons.Rdat")


color<-function(x)
{
	MAX<-90;MIN<-50;
	if(x > MAX){x <- MAX}
	if(x < MIN){x <- MIN}
	colors<-blue2green2red(201);
	pos<-round(200*(x-MIN)/(MAX-MIN))	
	return(colors[pos+1])
}

load("/atomdisk0/csantana/prueba/Data/Observed_Data/coords_estcena_pr.Rdat")
source("/atomdisk0/csantana/prueba/scripts_Victor/Rscripts/Paper/legend_function.R")

beanplot(ensemblemean_drydays_decadeswinter[1,]/20., ensemblemean_drydays_decadesautum[1,]/20., ensemblemean_drydays_decadessummer[1,]/20., ensemblemean_drydays_decadesspring[1,]/20., ensemblemean_drydays_decadeswinter[1,]/20., ensemblemean_drydays_decadesautum[1,]/20., ensemblemean_drydays_decadessummer[1,]/20., ensemblemean_drydays_decadesspring[1,]/20.)

yearsini<-c(1981,2011,2031,2051,2071)
yearsend<-c(2000,2030,2050,2070,2090)

for(idec in 1:5)
{
png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_winter_",yearsini[idec], yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_drydays_decadeswinter[idec,]/20.,color))
estimate.legend.size("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20);
place.legend("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20,title="Dry days");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_winter_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_drydays_decadeswinter[1,]/20.,color))
estimate.legend.size("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20);
place.legend("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20,title="Dry days");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_autum_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_drydays_decadesautum[idec,]/20.,color))
estimate.legend.size("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20);
place.legend("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20,title="Dry days");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_autum_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_drydays_decadesautum[idec,]/20.,color))
estimate.legend.size("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20);
place.legend("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20,title="Dry days");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_summer_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_drydays_decadessummer[idec,]/20.,color))
estimate.legend.size("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20);
place.legend("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20,title="Dry days");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_summer_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_drydays_decadessummer[idec,]/20.,color))
estimate.legend.size("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20);
place.legend("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20,title="Dry days");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_spring_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_drydays_decadesspring[idec,]/20.,color))
estimate.legend.size("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20);
place.legend("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20,title="Dry days");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/drydays_spring_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_drydays_decadesspring[idec,]/20.,color))
estimate.legend.size("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20);
place.legend("outbottom",legend=c(50+4*(0:10)),cex=0.8,col=sapply(50+4*(0:10),color),pch=20,title="Dry days");
dev.off()
}





######### T O T A L     P R E C I P




load("ensemblemean_totalprecipseasons.Rdat")

library(colorRamps)
load("/atomdisk0/csantana/prueba/Data/Observed_Data/coords_estcena_pr.Rdat")
source("/atomdisk0/csantana/prueba/scripts_Victor/Rscripts/Paper/legend_function.R")

beanplot(ensemblemean_totalprecipwinter[1,]/20.,ensemblemean_totalprecipautum[1,]/20.,ensemblemean_totalprecipsummer[1,]/20.,ensemblemean_totalprecipspring[1,]/20.)

color<-function(x)
{
	MAX<-300;MIN<-0;
	if(x > MAX){x <- MAX}
	if(x < MIN){x <- MIN}
	colors<-blue2green2red(201);
	pos<-round(200*(x-MIN)/(MAX-MIN))	
	return(colors[201-pos])
}

yearsini<-c(1981,2011,2031,2051,2071)
yearsend<-c(2000,2030,2050,2070,2090)


for(idec in 1:5)
{
png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/totalprecip_winter_",yearsini[idec], yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_totalprecipwinter[idec,]/20.,color))
estimate.legend.size("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20);
place.legend("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20,title="Total precipitation (mm)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/totalprecip_winter_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_totalprecipwinter[1,]/20.,color))
estimate.legend.size("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20);
place.legend("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20,title="Total precipitation (mm)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/totalprecip_autum_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_totalprecipautum[idec,]/20.,color))
estimate.legend.size("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20);
place.legend("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20,title="Total precipitation (mm)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/totalprecip_autum_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_totalprecipautum[idec,]/20.,color))
estimate.legend.size("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20);
place.legend("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20,title="Total precipitation (mm)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/totalprecip_summer_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_totalprecipsummer[idec,]/20.,color))
estimate.legend.size("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20);
place.legend("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20,title="Total precipitation (mm)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/totalprecip_summer_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_totalprecipsummer[idec,]/20.,color))
estimate.legend.size("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20);
place.legend("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20,title="Total precipitation (mm)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/totalprecip_spring_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_totalprecipspring[idec,]/20.,color))
estimate.legend.size("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20);
place.legend("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20,title="Total precipitation (mm)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/totalprecip_spring_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemean_totalprecipspring[idec,]/20.,color))
estimate.legend.size("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20);
place.legend("outbottom",legend=c(30*(0:10)),cex=0.8,col=sapply(30*(0:10),color),pch=20,title="Total precipitation (mm)");
dev.off()
}





######### D T R 





load("ensemblemeandtr_winter.Rdat")
load("ensemblemeandtr_summer.Rdat")
load("ensemblemeandtr_spring.Rdat")
load("ensemblemeandtr_autum.Rdat")

library(beanplot)
library(colorRamps)
load("/atomdisk0/csantana/prueba/Data/Observed_Data/coords_estcena_pr.Rdat")
source("/atomdisk0/csantana/prueba/scripts_Victor/Rscripts/Paper/legend_function.R")

meandtr_summer<-array(NA,dim=c(5,1445))
meandtr_winter<-array(NA,dim=c(5,1445))
meandtr_spring<-array(NA,dim=c(5,1445))
meandtr_autum<-array(NA,dim=c(5,1445))

calc_meandtr_summer<-function(isite){return(mean(ensemblemeancalibdtr_summer[idec,isite,]))}
calc_meandtr_winter<-function(isite){return(mean(ensemblemeancalibdtr_winter[idec,isite,]))}
calc_meandtr_autum<-function(isite){return(mean(ensemblemeancalibdtr_autum[idec,isite,]))}
calc_meandtr_spring<-function(isite){return(mean(ensemblemeancalibdtr_spring[idec,isite,]))}

for(idec in 1:5)
{
	meandtr_summer[idec,]<-sapply(1:1445,calc_meandtr_summer)
	meandtr_spring[idec,]<-sapply(1:1445,calc_meandtr_spring)
	meandtr_winter[idec,]<-sapply(1:1445,calc_meandtr_winter)
	meandtr_autum[idec,]<-sapply(1:1445,calc_meandtr_autum)
}

save(meandtr_winter,meandtr_autum,meandtr_summer,meandtr_spring,file="/hosts/atomdisk0/csantana/prueba/outputs/dtr/meandtr_seasons.Rdat")



beanplot(meandtr_winter[1,],meandtr_autum[1,],meandtr_summer[1,],meandtr_spring[1,],meandtr_winter[1,],meandtr_autum[5,],meandtr_summer[5,],meandtr_spring[5,])

color<-function(x)
{
	MAX<-20;MIN<-5;
	if(x > MAX){x <- MAX}
	if(x < MIN){x <- MIN}
	colors<-blue2green2red(101);
	pos<-round(100*(x-MIN)/(MAX-MIN))	
	return(colors[pos+1])
}

yearsini<-c(1981,2011,2031,2051,2071)
yearsend<-c(2000,2030,2050,2070,2090)


for(idec in 1:5)
{
png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/meandtr_winter_",yearsini[idec], yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(meandtr_winter[idec,],color))
estimate.legend.size("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20,title="Daily Temperature Range (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/meandtr_winter_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(meandtr_winter[1,],color))
estimate.legend.size("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20,title="Daily Temperature Range (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/meandtr_autum_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(meandtr_autum[idec,],color))
estimate.legend.size("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20,title="Daily Temperature Range (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/meandtr_autum_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(meandtr_autum[idec,],color))
estimate.legend.size("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20,title="Daily Temperature Range (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/meandtr_summer_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(meandtr_summer[idec,],color))
estimate.legend.size("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20,title="Daily Temperature Range (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/meandtr_summer_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(meandtr_summer[idec,],color))
estimate.legend.size("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20,title="Daily Temperature Range (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/meandtr_spring_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(meandtr_spring[idec,],color))
estimate.legend.size("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20,title="Daily Temperature Range (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/meandtr_spring_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(meandtr_spring[idec,],color))
estimate.legend.size("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(5+1.5*(0:10)),cex=0.8,col=sapply(5+1.5*(0:10),color),pch=20,title="Daily Temperature Range (oC)");
dev.off()
}





############## T A S M A X






load("ensemblemeantasmax_winter.Rdat")
load("ensemblemeantasmax_summer.Rdat")
load("ensemblemeantasmax_spring.Rdat")
load("ensemblemeantasmax_autum.Rdat")

library(beanplot)
library(colorRamps)
load("/atomdisk0/csantana/prueba/Data/Observed_Data/coords_estcena_pr.Rdat")
source("/atomdisk0/csantana/prueba/scripts_Victor/Rscripts/Paper/legend_function.R")

beanplot(ensemblemeantasmax_winter[1,],ensemblemeantasmax_autum[1,],ensemblemeantasmax_summer[1,],ensemblemeantasmax_spring[1,],ensemblemeantasmax_winter[1,],ensemblemeantasmax_autum[5,],ensemblemeantasmax_summer[5,],ensemblemeantasmax_spring[5,])

color<-function(x)
{
	MAX<-35;MIN<-10;
	if(x > MAX){x <- MAX}
	if(x < MIN){x <- MIN}
	colors<-blue2green2red(51);
	pos<-round(50*(x-MIN)/(MAX-MIN))	
	return(colors[pos+1])
}

yearsini<-c(1981,2011,2031,2051,2071)
yearsend<-c(2000,2030,2050,2070,2090)



for(idec in 1:5)
{
png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmax_winter_",yearsini[idec], yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmax_winter[idec,],color))
estimate.legend.size("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20,title="Seasonal Average Maximum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmax_winter_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmax_winter[1,],color))
estimate.legend.size("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20,title="Seasonal Average Maximum Temperature (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmax_autum_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmax_autum[idec,],color))
estimate.legend.size("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20,title="Seasonal Average Maximum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmax_autum_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmax_autum[idec,],color))
estimate.legend.size("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20,title="Seasonal Average Maximum Temperature (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmax_summer_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmax_summer[idec,],color))
estimate.legend.size("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20,title="Seasonal Average Maximum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmax_summer_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmax_summer[idec,],color))
estimate.legend.size("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20,title="Seasonal Average Maximum Temperature (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmax_spring_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmax_spring[idec,],color))
estimate.legend.size("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20,title="Seasonal Average Maximum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmax_spring_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmax_spring[idec,],color))
estimate.legend.size("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20);
place.legend("outbottom",legend=c(10+2.5*(0:10)),cex=0.8,col=sapply(10+2.5*(0:10),color),pch=20,title="Seasonal Average Maximum Temperature (oC)");
dev.off()
}







############## T A S M I N 






load("/atomdisk0/csantana/prueba/outputs/tasmin/ensemblemeantasmin_winter.Rdat")
load("/atomdisk0/csantana/prueba/outputs/tasmin/ensemblemeantasmin_summer.Rdat")
load("/atomdisk0/csantana/prueba/outputs/tasmin/ensemblemeantasmin_spring.Rdat")
load("/atomdisk0/csantana/prueba/outputs/tasmin/ensemblemeantasmin_autum.Rdat")

library(beanplot)
library(colorRamps)
load("/atomdisk0/csantana/prueba/Data/Observed_Data/coords_estcena_pr.Rdat")
source("/atomdisk0/csantana/prueba/scripts_Victor/Rscripts/Paper/legend_function.R")

beanplot(ensemblemeantasmin_winter[1,],ensemblemeantasmin_autum[1,],ensemblemeantasmin_summer[1,],ensemblemeantasmin_spring[1,],ensemblemeantasmin_winter[1,],ensemblemeantasmin_autum[5,],ensemblemeantasmin_summer[5,],ensemblemeantasmin_spring[5,])

color<-function(x)
{
	MAX<-30;MIN<-0;
	if(x > MAX){x <- MAX}
	if(x < MIN){x <- MIN}
	colors<-blue2green2red(21);
	pos<-round(20*(x-MIN)/(MAX-MIN))	
	return(colors[pos+1])
}

yearsini<-c(1981,2011,2031,2051,2071)
yearsend<-c(2000,2030,2050,2070,2090)



for(idec in 1:5)
{
png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmin_winter_",yearsini[idec], yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmin_winter[idec,],color))
estimate.legend.size("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20,title="Seasonal Average Minimum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmin_winter_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Winter (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmin_winter[1,],color))
estimate.legend.size("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20,title="Seasonal Average Minimum Temperature (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmin_autum_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmin_autum[idec,],color))
estimate.legend.size("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20,title="Seasonal Average Minimum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmin_autum_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Autum (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmin_autum[idec,],color))
estimate.legend.size("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20,title="Seasonal Average Minimum Temperature (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmin_summer_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmin_summer[idec,],color))
estimate.legend.size("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20,title="Seasonal Average Minimum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmin_summer_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Summer (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmin_summer[idec,],color))
estimate.legend.size("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20,title="Seasonal Average Minimum Temperature (oC)");
dev.off()

png(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmin_spring_",yearsini[idec],"", yearsend[idec],".png",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmin_spring[idec,],color))
estimate.legend.size("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20,title="Seasonal Average Minimum Temperature (oC)");
dev.off()
postscript(paste("/atomdisk0/csantana/prueba/outputs/figures/paper/ensemblemeantasmin_spring_",yearsini[idec],"", yearsend[idec],".eps",sep=""))
plot(coords_estcena,pch=19,ylab=" ",xlab=" ",main=paste("Spring (",yearsini[idec]," - ", yearsend[idec],")",sep=""),col=sapply(ensemblemeantasmin_spring[idec,],color))
estimate.legend.size("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20);
place.legend("outbottom",legend=c(3*(0:10)),cex=0.8,col=sapply(3*(0:10),color),pch=20,title="Seasonal Average Minimum Temperature (oC)");
dev.off()
}




##########





