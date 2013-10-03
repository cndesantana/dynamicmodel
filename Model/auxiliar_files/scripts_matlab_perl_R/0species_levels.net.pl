#!/usr/bin/perl

# PARAMS:  <Num of Species>  <Num of levels>  ///<Density of links>       ////////<Top predators> <Basic preys>
# Genera el archivo 0species.net con Num especies ensamblando por niveles troficos...
# la cantidad de especies en cada nivel trofico descenderá linealmente a medida que se sube...

$species=$ARGV[0];
$levels=$ARGV[1];

$N=$species;
$k=$levels;
$n1=1;
$a=2*($N-$k*$n1)/($k*($k-1));
print "a: $a\n";
die "a<1!!! reduzca el numero de niveles" if ($a<1);

# # Con este código se pueden ver todas las posibilidades de composiciones de especies en las capas troficas...
# while($a>=1)
# {
#  print "a=$a : ";
#  for($i=1; $i<=$k; $i++)
#  {
#  	print int($n1+($i-1)*$a)," ";
#  }
#  print "\n";
# 	$n1++;
# 	$a=2*($N-$k*$n1)/($k*($k-1));
# }
# exit(1);
# # Con este código se pueden ver todas las posibilidades de composiciones de especies en las capas troficas...



open($fout,">","0species.net");

print $fout "*Vertices $species\n";
for(my $ix=1; $ix<=$species; $ix++){
	print $fout "$ix $ix 0 0 0 0 10\n";
}

print $fout "*Arcs\n";

# armo los niveles troficos (nivel 1 esta en el top de la piramide)
@net=();
$s=0;
for(my $i=1;$i<=$k; $i++)
{
 @newLevel=();
	$ni=int($n1+$a*($i-1));
	for($s_inLevel=1; $s_inLevel<=$ni; $s_inLevel++)
	{
		$s++;
		push @newLevel,$s;
	}
	push @net,[@newLevel];
}

# imprimo los links por niveles...

for $k ( 0 .. $#net-1 ) #nivel trofico
{
	for $ix_s ( 0 .. $#{$net[$k]} ) #indice de especie
	{
				$predador=$net[$k][$ix_s];   #predador
				print "$predador ";
# 				armo la lista de posibles presas...
				@presas=();
				for $k_aux ( $k+1 .. $#net) #los niveles inferiores...
				{
					push @presas,@{$net[$k_aux]};
				}
# 				foreach $p (@presas){print "$p "}
# 				print "\n";
# 				exit(1);
				foreach $p (@presas)
				{
					print $fout "$predador $p\n" if (rand()<0.75);  #agrego link predador presa con probabilidad 0.5
				}
				
	}
	print "\n";
}
















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
