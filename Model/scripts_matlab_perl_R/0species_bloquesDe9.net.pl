#!/usr/bin/perl

# PARAMS:  <Num of Species>  <Num of links interbloque>  ///<Density of links>       ////////<Top predators> <Basic preys>
# Genera el archivo 0species.net con Num especies ensamblando bloques de 10

$species=$ARGV[0];
$linksInterBloque=$ARGV[1];

open($fout,">","0species.net");

print $fout "*Vertices $species\n";
for(my $ix=1; $ix<=$species; $ix++){
	print $fout "$ix $ix 0 0 0 0 10\n";
}

print $fout "*Arcs\n";

$cantBloques=int($species/9);
$cantSpSueltas=$species%9;

# imprimo los links intrabloques...
for(my $bloque=1; $bloque<=$cantBloques; $bloque++){
	
	
	$sp1=($bloque-1)*9+1; $sp2=($bloque-1)*9+4; print $fout "$sp1 $sp2\n";
	$sp1=($bloque-1)*9+1; $sp2=($bloque-1)*9+5; print $fout "$sp1 $sp2\n";
	$sp1=($bloque-1)*9+1; $sp2=($bloque-1)*9+6; print $fout "$sp1 $sp2\n";
	$sp1=($bloque-1)*9+1; $sp2=($bloque-1)*9+7; print $fout "$sp1 $sp2\n";
	$sp1=($bloque-1)*9+1; $sp2=($bloque-1)*9+8; print $fout "$sp1 $sp2\n";
	$sp1=($bloque-1)*9+1; $sp2=($bloque-1)*9+9; print $fout "$sp1 $sp2\n";
	
	
	$sp1=($bloque-1)*9+2; $sp2=($bloque-1)*9+4; print $fout "$sp1 $sp2\n";
	$sp1=($bloque-1)*9+2; $sp2=($bloque-1)*9+5; print $fout "$sp1 $sp2\n";
	$sp1=($bloque-1)*9+2; $sp2=($bloque-1)*9+6; print $fout "$sp1 $sp2\n";
	
	$sp1=($bloque-1)*9+3; $sp2=($bloque-1)*9+7; print $fout "$sp1 $sp2\n";
	$sp1=($bloque-1)*9+3; $sp2=($bloque-1)*9+8; print $fout "$sp1 $sp2\n";
	$sp1=($bloque-1)*9+3; $sp2=($bloque-1)*9+9; print $fout "$sp1 $sp2\n";
	
}


# ahora imprimo los links interbloques...
# Elijo los links al azar...
for(my $bloque=1; $bloque<$cantBloques; $bloque++){

			$sp_from=($bloque-1)*9+1; #conecto el gran predador... con el gran predador del bloque target...
			$bloque_target=$bloque+1;
# 			$bloque_target=$bloque;
# 			while($bloque_target<=$bloque){
# 				$bloque_target=int(rand($cantBloques))+1;
# 			}
			$sp_target=($bloque_target-1)*9+1; #conecto el gran predador... con el gran predador del bloque target...
			print $fout "$sp_from $sp_target\n";

   if($linksInterBloque>1)
   {
# 				for(my $links=2; $links<=$linksInterBloque; $links++){
		# 			$sp_from=($bloque-1)*9+int(rand(9))+1;  #elijo especie al azar dentro del bloque...
					$sp_from=($bloque-1)*9+1; #conecto el gran predador... con...
# 					$sp_target=($bloque_target-1)*9+int(rand(9))+1; #elijo especie al azar dentro del bloque target...
# 					$sp_target=($bloque_target-1)*9+1; #conecto el gran predador... con el gran predador del bloque target...
#      $sp_target=($bloque_target-1)*9+1+int(rand(2))+1; #elijo especie al azar una presa base dentro del bloque target...
     $sp_target=($bloque_target-1)*9+2;
     print $fout "$sp_from $sp_target\n";
     
     $sp_target=($bloque_target-1)*9+3;
					print $fout "$sp_from $sp_target\n";
# 				}
		 }
}



#ahora linkeo las especies fuera de bloques...

for(my $sp=$cantBloques*9+1; $sp<=$species; $sp++){
	for(my $links=1; $links<=$linksInterBloque; $links++){
		$sp_from=$sp;
		$sp_target=int(rand($species-$cantSpSueltas))+1;
		print $fout "$sp_from $sp_target\n";
	}
}

close($fout);
