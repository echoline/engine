<?php

function cactus($x, $y, $z, $id) {
	$M_2_PI = M_PI*2;
	$M_4_PI = M_PI*4;
	$M_PI_2 = M_PI/2;
	$M_PI_4 = M_PI/4;

	echo '{"id":"cactus' . $id . '","p":[' . $x . ',' . $y . ',' . $z;
	echo '],"r":[0,0,0],"c":"#8d8","ps":[[0,0,0]';

	$up = -rand(2,5);
	$br = 10;

	for ($ry = 0; $ry < $M_4_PI; $ry += ((((rand() % 360) + 270) / 360) * M_PI)) {
		$up -= rand(2,5);
		echo ',[0,' . $up . ',0]';

		$br = rand(5,$br);

		//for ($rx = 0; $rx <= $M_PI_2; $rx += $M_PI_4) {
			$rx = $M_PI_2;
			echo ',[' . (cos($ry) * sin($rx) * $br) . ',' .
				(cos($rx) * $br + $up - $br) . ',' .
				(sin($ry) * sin($rx) * $br) . ']';
		//}

		echo ',[' . (cos($ry) * $br) . ',' .
			($up - $br - rand(5,8)) . ',' .
			(sin($ry) * $br) . ']';

			$rx = $M_PI_2;
			echo ',[' . (cos($ry) * sin($rx) * $br) . ',' .
				(cos($rx) * $br + $up - $br) . ',' .
				(sin($ry) * sin($rx) * $br) . ']';
			//		for ($rx = $M_PI_2; $rx >= 0; $rx -= $M_PI_4) {
			$rx = 0;
			echo ',[' . (cos($ry) * sin($rx) * $br) . ',' .
				(cos($rx) * $br + $up - $br) . ',' .
				(sin($ry) * sin($rx) * $br) . ']';
//		}
	}
	$up -= rand(13,21);
	echo ',[0,' . $up . ',0]';

	echo ']}';
}

function oasis($x, $y, $z, $id, $r) {
	$M_2_PI = M_PI*2;
	$M_PI_8 = M_PI/8;

	echo '{"id":"oasis' . $id . '","p":[' . $x . ',' . $y . ',' . $z;
	echo '],"r":[0,0,0],"c":"#f00","f":"#fa4","ps":[[0,0,' . $r . ']';

	for ($i = 0; $i <= $M_2_PI; $i += $M_PI_8) {
		echo ',[' . (sin($i) * $r) . ',0,' . (cos($i) * $r) . ']';
	}

	echo '],"type":"polygon"}';

}

function clock($x, $y, $z, $id, $firstpacket) {
	$M_2_PI = M_PI*2;
	$M_PI_12 = M_PI/12;

	if ($firstpacket) {

		echo '{"id":"clock' . $id . '","p":[' . $x . ',' . $y . ',' . $z . '],"r":[0,0,0],"ps":[';

		for ($i = 0; $i < $M_2_PI; $i += $M_PI_12) {
			echo '[';
			echo sin($i) * 10;
			echo ',';
			echo cos($i) * 10;
			echo ',0],';
		}

		echo '[0,10,0]],"type":"polygon","f":"#ccc"},';
	}

	echo '{"id":"minute' . $id . '","p":[' . $x . ',' . $y . ',' . $z . ']';
	echo ',"r":[0,0,';

	$h = date('i');
	$h /= 60.0;
	$h *= $M_2_PI;
	echo $h;

	echo '],"ps":[[0,0,0],[0,-7,0]]},';

	echo '{"id":"second' . $id . '","p":[' . $x . ',' . $y . ',' . $z . ']';
	echo ',"r":[0,0,';

	$h = date('s');
	$h /= 60.0;
	$h *= $M_2_PI;
	echo $h;

	echo '],"ps":[[0,0,0],[0,-7,0]],"c":"#800"}';
}

function pyramid($x, $y, $z, $id) {
	echo '{"id":"pyramid_a' . $id . '","p":[' . $x . ',' . $y . ',' . $z . '],"r":[0,-50,0],"ps":[[300,0,300],[300,0,-300],[0,-300,0]],"type":"polygon","f":"#dda","c":"#dda"},';
	echo '{"id":"pyramid_b' . $id . '","p":[' . $x . ',' . $y . ',' . $z . '],"r":[0,-50,0],"ps":[[-300,0,-300],[300,0,-300],[0,-300,0]],"type":"polygon","f":"#dda","c":"#dda"},';
	echo '{"id":"pyramid_c' . $id . '","p":[' . $x . ',' . $y . ',' . $z . '],"r":[0,-50,0],"ps":[[300,0,300],[-300,0,300],[0,-300,0]],"type":"polygon","f":"#dda","c":"#dda"},';
	echo '{"id":"pyramid_d' . $id. '","p":[' . $x . ',' . $y . ',' . $z . '],"r":[0,-50,0],"ps":[[-300,0,-300],[300,0,-300],[0,-300,0]],"type":"polygon","f":"#dda","c":"#dda"}';
}

function player($x, $y, $z, $a, $b, $c, $id, $color) {
//	echo '{"ps":[[5,-10,5],[5,-10,-5],[-5,-10,-5],[-5,-10,5],[5,-10,5],[5,0,5],[-5,0,5],[-5,-10,5],[-5,0,5],[-5,0,-5],[-5,-10,-5],[-5,0,-5],[5,0,-5],[5,-10,-5],[5,0,-5],[5,0,5]],';
	echo '{"ps":[[0,0,0],[0,-10,0]],';
	echo '"c":"' . $color . '","p":[' . $x . ',' . $y . ',' . $z . '],';
	echo '"r":[' . $a . ',' . $b . ',' . $c . '],"id":"player' . $id . '"}';
}

function flower($x, $y, $z, $id, $firstpacket) {
	$M_2_PI = M_PI*2;
	$M_PI_4 = M_PI/4;
	$M_PI_8 = M_PI/8;
	$M_PI_9 = M_PI/9;

	echo '{"ps":[';

	for ($i = 0; $i < $M_2_PI; $i += $M_PI_8) {
		echo '[';
		echo cos($i) * 150;
		echo ',0,';
		echo sin($i) * 150;
		echo '],';
	}

	echo '[150,0,0]],"p":[' . $x . ',' . $y . ',' . $z . '],';
	echo '"r":[0,0,0],"id":"fring' . $id . '","c":"#00F","f":"#06A","type":"polygon"},';

/*	echo '{"ps":[';

	for ($i = 0; $i < $M_2_PI; $i += $M_PI_8) {
		echo '[';
		echo cos($i) * 75;
		echo ',0,';
		echo sin($i) * 75;
		echo '],';
	}

	echo '[75,0,0]],"p":[' . $x . ',' . $y . ',' . $z . '],';
	echo '"r":[0,0,0],"id":"fpond' . $id . '","c":"#383","f":"#06a","type":"polygon"}';
*/
	echo '{';
	echo '"ps":[';

	for ($j = M_PI; $j <= $M_2_PI; $j += $M_PI_9) {
		for ($i = 0; $i <= M_PI; $i += $M_PI_8) {
			echo '[';
			echo sin($i) * cos($j) * 75;
			echo ',';
			echo sin($i) * sin($j) * 75;
			echo ',';
			echo cos($i) * 75;
			echo '],';
		}
		$j += $M_PI_9;
		for ($i = M_PI; $i >= 0; $i -= $M_PI_8) {
			echo '[';
			echo sin($i) * cos($j) * 75;
			echo ',';
			echo sin($i) * sin($j) * 75;
			echo ',';
			echo cos($i) * 75;
			echo '],';
		}
	}
	echo '[0,0,75]],';
	echo '"c":"' . '#338' . '",';
	echo '"p":[' . $x . ',' . $y . ',' . $z . '],';
	echo '"r":[0,0,0],"id":"fctr' . $id . '"}';
}

?>
