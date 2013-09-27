#!/usr/bin/perl

# PARAMS:  <Num of Species>  <Num of links interbloque>  ///<Density of links>       ////////<Top predators> <Basic preys>
# Genera el archivo 0species.net con Num especies ensamblando bloques 1-2

$species=$ARGV[0];
$linksInterBloque=$ARGV[1];
# $densidad=$ARGV[1];
# $Top=$ARGV[2];
# $Basic=$ARGV[3];

open($fout,">","0species.net");

print $fout "*Vertices $species\n";
for(my $ix=1; $ix<=$species; $ix++){
	print $fout "$ix $ix 0 0 0 0 10\n";
}

print $fout "*Arcs\n";

$cantBloques=int($species/3);
$cantSpSueltas=$species%3;

# imprimo los links intrabloques...
for(my $bloque=1; $bloque<=$cantBloques; $bloque++){
 $sp1=($bloque-1)*3+1;
 $sp2=($bloque-1)*3+2;
 $sp3=($bloque-1)*3+3;
	print $fout "$sp1 $sp2\n";
	print $fout "$sp1 $sp3\n";
}

# ahora imprimo los links interbloques...
for(my $bloque=1; $bloque<=$cantBloques; $bloque++){
 for(my $links=1; $links<=$linksInterBloque; $links++){
			$sp_from=($bloque-1)*3+int(rand(3))+1;  #elijo especie al azar dentro del bloque...
			
			$bloque_target=int(rand($cantBloques))+1;
			$sp_target=($bloque_target-1)*3+int(rand(3))+1; #elijo especie al azar dentro del bloque target...
			
			print $fout "$sp_from $sp_target\n";
	}
}


#ahora linkeo las especies fuera de bloques...

for(my $sp=$cantBloques*3+1; $sp<=$species; $sp++){
	for(my $links=1; $links<=$linksInterBloque; $links++){
		$sp_from=$sp;
		$sp_target=int(rand($species-$cantSpSueltas))+1;
		print $fout "$sp_from $sp_target\n";
	}
}

close($fout);
