#!/usr/bin/perl

$cantSites=9;

print "*Vertices $cantSites\n";
for($ixSite=1;$ixSite<=$cantSites;$ixSite++)
{
  print "$ixSite $ixSite 1000\n";
}
print "*Edges\n";
for($ixSite=1;$ixSite<$cantSites;$ixSite++)
{
  print "$ixSite ",$ixSite+1," 10\n";
}
print "$cantSites 1 10\n";