#!/usr/bin/perl

# Genera la lista de conexiones entre sitios...
# entorno de 25 sitios...

$l=10;
$l2=$l**2;

for($id=1; $id<=$l2; $id++){
	$x=($id-1)%$l+1;
	$y=int(($id-1)/$l)+1;
# 	print "$id\t ($x, $y)\n";
	for($dx=-2; $dx<=2; $dx++){
		$tgt_x=$x+$dx;
		$tgt_x+=10 if $tgt_x<=0;
		$tgt_x-=10 if $tgt_x>=$l+1;
		for($dy=-2; $dy<=2; $dy++){
			$tgt_y=$y+$dy;
			$tgt_y+=10 if $tgt_y<=0;
			$tgt_y-=10 if $tgt_y>=$l+1;
			
			$tgt_id=($tgt_y-1)*$l+$tgt_x;
			
			print "$id $tgt_id\n" if ($id!=$tgt_id && $tgt_id>=1 && $tgt_id<=100);
		}
	}
}