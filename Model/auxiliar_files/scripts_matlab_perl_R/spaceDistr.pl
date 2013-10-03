#!/usr/bin/perl

# Params: <timeToStudy> <output_species***.dat list>

$time=shift(@ARGV);
foreach $fileName (@ARGV){
	open(fin,$fileName);
	for($t=1;$t<$time;$t++){<fin>}
	$line=<fin>;
	close(fin);
	
	chomp($line);
	@F=split(/ /,$line);
	$acum=0;
	foreach $IndAtST (@F){$acum+=$IndAtST; print "$IndAtST\n"} print "\n";
	print STDERR "TotInds: ",$acum,"\n";
}