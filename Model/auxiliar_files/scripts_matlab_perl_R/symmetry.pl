#!/usr/bin/perl

#Params: <pajek.net file> <thr>

# Calcula la simetria de los arcos y define Edges si la asimetria no supera el threshold de parametro...

$thr=$ARGV[1];
open(netIn,$ARGV[0]);

$ARGV[0]=~/(.*).net/;
$outFileName=$1."_THR_".$thr.".net";
# print "$outFileName\n"; exit(1);
open(netOut,">$outFileName");

$nodos=1;
while(<netIn>)
{
  if(/^\*Arcs/){$nodos=0}
  if($nodos==1)
  {
    print netOut $_;
  }
  else
  {
    chomp;
    @F=split;
    $arc[$F[0]][$F[1]]=$F[2];
  }
}


for($i=1; $i<$#arc; $i++)
{
  for($j=$i+1; $j<=$#arc; $j++)
  {
    if(max($arc[$i][$j],$arc[$j][$i])>0)
    {
      $asimetria=abs($arc[$i][$j]-$arc[$j][$i])/max($arc[$i][$j],$arc[$j][$i]);
      $asimetria=0.00001 if ($asimetria==0); # porque al Pajek no le gustan los ceros!!!!
      if($asimetria<$thr)
      { 
        $newElem=[$i, $j, $asimetria];
        push @edges, $newElem;
#        print "$i $j $asimetria $newElem $#edges\n";
      }
      else
      {
        if($arc[$i][$j]>$arc[$j][$i])
        {
          push @arcs, [$i, $j, $asimetria];
        }
        else
        {
          push @arcs, [$j, $i, $asimetria];
        }
      }
    }
  }
}

if($#edges)
{
  print netOut "*Edges\n";
  foreach (@edges)
  {
    print netOut "$_->[0] $_->[1] $_->[2]\n";
  }
}

if($#arcs)
{
  print netOut "*Arcs\n";
  foreach (@arcs)
  {
    print netOut "$_->[0] $_->[1] $_->[2]\n";
  }
}

close netOut;
system( ("unix2dos",$outFileName));



sub max {
    if ($_[0]<$_[1]) {return $_[1]} else {return $_[0]};
}