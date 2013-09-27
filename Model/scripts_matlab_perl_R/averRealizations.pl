#!/usr/bin/perl

# Params: <AverIndInTime....dat>
$cantFiles=@ARGV;
# die $cantFiles,"\n";
for($argIx=0; $argIx<=$#ARGV; $argIx++){
# 	print $ARGV[$argIx],"\n";
	
	open($inf,"<",$ARGV[$argIx]);
	@datos=<$inf>;
	close($inf);
	
	for($t=0; $t<=$#datos; $t++){
		chomp($datos[$t]);
		@F=split(/ /,$datos[$t]);
# 		foreach (@F) {print $_,":"} print "\n"; die "prue\n";
		for($col=1; $col<=$#F; $col++){
			$acum[$t][$col-1]+=$F[$col] if $F[$col]>0;
		}
	}
}

$tMax=$#acum;
$cantSP=$#{$acum[0]}+1;
# die "$tMax $cantSP\n";
for($t=0; $t<=$tMax; $t++){
	print "$t ";
	for($sp=0; $sp<$cantSP; $sp++){
		print $acum[$t][$sp]/$cantFiles," ";
	}
	print "\n";
}

