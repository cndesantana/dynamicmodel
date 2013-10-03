#!/usr/bin/perl

# Params: <fwnf> <SpeciesPopFile>

open(fwnf,$ARGV[0]);
$line=<fwnf>;
$line=~/\*Vertices (\d+)/;
$spNum=$1;
# die $1;
for($ix=1; $ix<=$spNum+1; $ix++){$line=<fwnf>;}
# Ahora empiezan los arcos...
while(<fwnf>){
	chomp;
	@F=split;
	$p=$F[0]; $h=$F[1];
	$Preys[$p][$h]=1;
}
close(fwnf);

open(spPopFile,$ARGV[1]);
@spNum=<spPopFile>;
close(spPop);

map {chomp; $totPop+=$_;} @spNum;
$numOfSp=@spNum;


for($ix=1; $ix<=$numOfSp; $ix++){
	for($jx=1; $jx<=$numOfSp; $jx++){
	 $predadores[$ix]+=$Preys[$jx][$ix]*$spNum[$jx];
	}
}

for($ix=1; $ix<=$numOfSp; $ix++){
# 	print $ix,"\t ",$spNum[$ix-1]," [PREYS]: ";
	for($jx=1; $jx<=$numOfSp; $jx++){
# 	 print "$jx " if($Preys[$ix][$jx]);
  $sp_presas[$ix]++ if($Preys[$ix][$jx]);
  $sp_predadores[$ix]++ if($Preys[$jx][$ix]);
	 $pres=$Preys[$ix][$jx]*$spNum[$jx];
		$presas[$ix]+=$pres;
		$PreysTIMESnotPredsOfPreys[$ix]+=$pres/$totPop*(1-$predadores[$jx]/$totPop);
	}
	$PreysTIMESnotPredsOfPreys[$ix]/=$sp_presas[$ix] if($sp_presas[$ix]);
# 	print "\n";
}

for($ix=1; $ix<=$numOfSp; $ix++){
	$Bp[$ix]=($PreysTIMESnotPredsOfPreys[$ix]+(1-$predadores[$ix]/$totPop))/2;	
# 	print "$ix\t $spNum[$ix-1]\t $Bp[$ix]\n";
	print "$Bp[$ix]\t ",$spNum[$ix-1]/$totPop,"\n";
# 	print "$Bp[$ix]\t ",$spNum[$ix-1]/$totPop*(1+$sp_presas[$ix]),"\n";
}