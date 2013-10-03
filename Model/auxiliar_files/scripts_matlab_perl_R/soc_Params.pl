#!/usr/bin/perl

# Params: <SOC_Parameters..... files list>

# shift(@ARGV);
# foreach $elem (@ARGV) {print "$elem\n"} 
# die "hasta aca\n";

$cantFiles=@ARGV;

# abro el primer archivo...

open(fin,"tail -1 $ARGV[0] |");   #Abro el archivo a traves de un pipe que filtra con "tail -1"... asi solo obtengo la ultima linea de c/archivo...
#                                   (parametros en todos los sitios a tiempo maximo)
$_=<fin>;
close(fin); 
chomp;
@F=split;
map{
	$_=~/\{(.*)\}/;
	@Pars=split(/,/,$1);
	push @bp,min(max($Pars[0],0),1);
	push @dp,min(max($Pars[1],0),1);
	push @ndp,min(max($Pars[3],0),1);
	push @n,max($Pars[4],0);
} @F;


# abro el resto de los archivos... promediando cada sitio...
shift(@ARGV);
foreach $fileName (@ARGV){
	open(fin,"tail -1 $fileName |");   #Abro el archivo a traves de un pipe que filtra con "tail -1"... asi solo obtengo la ultima linea de c/archivo...
	#                                   (parametros en todos los sitios a tiempo maximo)
	$_=<fin>;
	close(fin); 
	chomp;
	@F=split;
	for($ix=0; $ix<=$#F; $ix++){
		$F[$ix]=~/\{(.*)\}/;
		@Pars=split(/,/,$1);
		$bp[$ix]+=min(max($Pars[0],0),1);
		$dp[$ix]+=min(max($Pars[1],0),1);
		$ndp[$ix]+=min(max($Pars[3],0),1);
		$n[$ix]+=max($Pars[4],0);
	} @F;
}


# imprimo c/sitio y calculo el promedio a lo largo de todos los sitios...
foreach $elem (@bp) {$promAtSite=$elem/$cantFiles; print $promAtSite,"\n"; $bp_acum+=$promAtSite} print "\n";
foreach $elem (@dp) {$promAtSite=$elem/$cantFiles; print $promAtSite,"\n"; $dp_acum+=$promAtSite} print "\n";
foreach $elem (@ndp) {$promAtSite=$elem/$cantFiles; print $promAtSite,"\n"; $ndp_acum+=$promAtSite} print "\n";
foreach $elem (@n) {$promAtSite=$elem/$cantFiles; print $promAtSite,"\n"; $n_acum+=$promAtSite}

$cantSites=@bp;

# finalmente se imprimen los promedios totales... (todas las realizaciones y todos los sitios...)
print STDERR "<bp, dp, ndp, n>: ",$bp_acum/$cantSites," ",$dp_acum/$cantSites," ",$ndp_acum/$cantSites," ",$n_acum/$cantSites,"\n";

# foreach $elem (@bp) {print $elem/$cantFiles,"\n"} print "\n";
# foreach $elem (@dp) {print $elem/$cantFiles,"\n"} print "\n";
# foreach $elem (@ndp) {print $elem/$cantFiles,"\n"}



sub max{
 my ($x,$y)=@_;
 return $x if ($x>=$y);
 return $y;
}

sub min{
 my ($x,$y)=@_;
 return $x if ($x<=$y);
 return $y;
}


