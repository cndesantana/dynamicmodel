#!/usr/bin/perl

# PARAMS: <seed>
# # # PARAMS: <AverIndInTime...dat>  <FoodWeb...net>

# $IndsFile=$ARGV[0];
# $FWFile=$ARGV[1];
$seed=$ARGV[0];
$IndsFile=sprintf("%s%d%s","AverIndInTime_seed_",$seed,".dat");
$FWFile="FoodWeb_seed_".$seed."_.net";
# die $FWFile;

open(IndsIn, "tail -1 $IndsFile |");

$line=<IndsIn>;
chomp($line);
@CantInds=split(/\s+/,$line);   #el primer elemento es el tiempo... el resto son las cantidades de inds de cada especie a ese tiempo...
# map {print "$_\n"} @CantInds;
close(IndsIn);

open(FWin, $FWFile);
$line=<FWin>; #encabezado...
$line=~/\*Vertices (\d+)/;
# print "$1\n";
$cantSpecies=$1;
for(my $ix=1; $ix<= $cantSpecies; $ix++)
{
	$line=<FWin>;
	chomp($line);
	@F=split(/\s+/,$line);
# 	map {print "$_:"} @F; print "\n";
 print "$F[2] $CantInds[$ix]\n" if $CantInds[$ix];   #no imprimo los que no tienen individuos...
}



