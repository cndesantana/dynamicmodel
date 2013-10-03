#!/usr/bin/perl

#Params: <list of .dat files contining temporal bihavior of species at different sites>

print "@ arrange(1,3,0.1,0.1,0.1)\n";
foreach $par (@ARGV) 
{
  open(fin,$par);
  $par=~/^species_(\d+)_(\d+)/;
  $sp=$1-1;
  $site=$2-1;
  while(<fin>)
  {
    chomp;
    @F=split;
    print "@ g$site.s$sp point $F[0], $F[1]\n";
  }
}
$nSites=$site+1;
$nSpecies=$sp+1;
$site=0;
print "@ focus g$site\n";
$s1=$site+1;
print "@ title \"SITE $s1\"\n";
print "@ xaxis label \"Time\"\n";
print "@ yaxis label \"# of individuals\"\n";
print "@ autoscale\n";

for($site=1;$site<$nSites;$site++)
{
  print "@ focus g$site\n";
  $s1=$site+1;
  print "@ title \"SITE $s1\"\n";
  print "@ xaxis label \"Time\"\n";
  print "@ autoscale\n";
}
print "@ legend on\n";
print "@ legend 0.10470164, 0.9\n";
for($sp=0;$sp<$nSpecies;$sp++)
{
  $sp1=$sp+1;
  print "@ s$sp legend \"Species_$sp1\"\n";
}

