#!/usr/bin/perl

# PARAMS:  <site/col(1...)> <lista de archivos output_species_...>
# Imprime la serie temportal de todas las especies correspondientes a esa corrida en el sitio correspondiente.

$site=$ARGV[0];

for($file=1; $file<=$#ARGV; $file++){
#  print "$ARGV[$file]\n ";
  open(data,$ARGV[$file]);
  while(<data>){
    chomp($_);
    @F=split(/\s/,$_);
    $mat[$.][$file]=$F[$site-1];
  }
  close(data);
#   print "$ARGV[$file]\n ";
#   print "$#mat\n";
#   exit;
}

for($t=1;$t<=$#mat;$t++){
  print "$t ";
  for($file=1; $file<=$#ARGV; $file++){
    print $mat[$t][$file]," ";
  }
  print "\n";
}