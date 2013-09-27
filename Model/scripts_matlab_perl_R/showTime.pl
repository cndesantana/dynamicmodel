#!/usr/bin/perl

while(<>)
{
chomp;
@F=split; 
print $F[0]*5,"\n";
}
