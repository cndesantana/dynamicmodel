#!/usr/bin/perl

# PARAMS:  <Num of Species> <Density of links>       ////////<Top predators> <Basic preys>
# Genera el archivo 0species.net con Num especies y con una densidad de links como se indica por parámetro.
# Los links son incluidos en forma aleatoria...

$species=$ARGV[0];
$densidad=$ARGV[1];
# $Top=$ARGV[2];
# $Basic=$ARGV[3];

open($fout,">","0species.net");

print $fout "*Vertices $species\n";
for(my $ix=1; $ix<=$species; $ix++){
	print $fout "$ix $ix 0 0 0 0 10\n";
}

print $fout "*Arcs\n";
$cantArcs=$densidad*$species;

# me aseguro que todos sean predadores de alguien...
for(my $from=1; $from<=$species; $from++){
 $target=$from;
 while ($target==$from){  #busco una especie Target distinta de la especie From...
		$target=int(rand($species))+1;
	}
	print $fout "$from $target\n";
}

# hasta ahora solo introduje $species links...
# ahora introduzco los links que quedan en forma aleatoria...
for(my $arc=$species+1; $arc<=$cantArcs; $arc++){
	$from=int(rand($species))+1;
 $target=$from;
 while ($target==$from){  #busco una especie Target distinta de la especie From...
		$target=int(rand($species))+1;
	}	
	print $fout "$from $target\n";
}

close($fout);
