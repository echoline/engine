<?php
	if (!isset($_POST['cookie']) || !isset($_POST['p']) || !isset($_POST['r']) || !isset($_POST['f'])) {
		exit(-1);
	}

	$time = time();
	$health = 1;

	$p = $_POST['p'];
	$r = $_POST['r'];

	if ((substr($p,0,1) != '[') ||
	    (substr($p,-1,1) != ']') ||
	    (substr($r,0,1) != '[') ||
	    (substr($r,-1,1) != ']'))
		exit (-2);

	$p = substr($p,1,-1);
	$r = substr($r,1,-1);

	$pos_0 = split(',', $p);
	$rot_0 = split(',', $r);

	if (!is_numeric($pos_0[0]) ||
	    !is_numeric($pos_0[1]) ||
	    !is_numeric($pos_0[2]) ||
	    !is_numeric($rot_0[0]) ||
	    !is_numeric($rot_0[1]) ||
	    !is_numeric($rot_0[2]))
		exit(-3);

	$firstpacket = ($_POST['f'] == 'true');

	require 'login.php';
	require 'objects.php';

?>{"s":[<?php
	//clock(800, -30, -800, 0, $firstpacket);
	//echo ',';

	flower(800,0,-800, 0, $firstpacket);
	echo ',';
	$i = 0;

	clock(800,-30,-800,1,$firstpacket);
	echo ',';

	if ($firstpacket) {
		$i=2;
		cactus(5000,0,5000, $i++);
		echo ',';
		cactus(4200,0,1300, $i++);
		echo ',';
		cactus(2000,0,-1300, $i++);
		echo ',';
		cactus(-2000,0,-1300, $i++);
		echo ',';
		cactus(-2000,0,1300, $i++);
		echo ',';
		cactus(1300,0,2000, $i++);
		echo ',';
		cactus(1300,0,-2000, $i++);
		echo ',';
		cactus(-1300,0,-1300, $i++);
		echo ',';
		oasis(0, 0, 0, $i++, 150);
		echo ',';
		pyramid(8000, 0, 50000, 0, true);
		echo ',';
		pyramid(10000, 0, 50000, 1, true);
		echo ',';
		pyramid(12000, 0, 50000, 2, true);
		echo ',';
	}

	$my_mysql = mysql_connect($my_host, $my_user, $my_pass);
	if (($my_mysql != NULL) && mysql_select_db('game', $my_mysql)) {
		$p = mysql_real_escape_string($pos_0[0] . ',' . $pos_0[1] . ',' .  $pos_0[2]);
		$r = mysql_real_escape_string($rot_0[0] . ',' . $rot_0[1] . ',' .  $rot_0[2]);
		$cookie = mysql_real_escape_string($_POST['cookie']);

		if (!$firstpacket) {
			$results = mysql_query('select water from players where cookie=\'' . $cookie . '\'', $my_mysql);
			$water = mysql_fetch_row ($results);
			if ($water != NULL)
				$water = $water[0];
			$pos = split(',', $p);
			$dist = sqrt($pos[0]*$pos[0] + $pos[2]*$pos[2]);
			if ($dist < 150.0) {
				$water = $time;
			}
			$health = (300 - ($time - $water)) / 300.0;
			if ($health > 0)
				$results = mysql_query('replace into players (time, p, r, cookie, water) values (' . $time . ',\'' . $p . '\',\'' .  $r . '\',\'' . $cookie . '\',' . $water . ')', $my_mysql);
		}

		$results = mysql_query('select p,r,cookie,time from players where cookie!=\'' . $cookie . '\'', $my_mysql);
		if ($results != NULL) {
			$i = 1;
			while ($row = mysql_fetch_row($results)) {
				$pos = split(',', $row[0]);
				$rot = split(',', $row[1]);

				$dx = ($pos[0] - $pos_0[0]);
				$dy = ($pos[1] - $pos_0[1]);
				$dz = ($pos[2] - $pos_0[2]);
				$dist = sqrt($dx*$dx + $dy*$dy + $dz*$dz);

				if ($dist < 30000) {
					if ($time < ($row[3] + 10)) {
						if ($time < ($row[3] + 3)) {
							player($pos[0],0,$pos[2],$rot[0],$rot[1],$rot[2],$i,'#' . substr($row[2], -3, 3));
						} else {
							player($pos[0],100,$pos[2],$rot[0],$rot[1],$rot[2],$i,'#dda');
						}

						echo ',';
					}
				}

				$i++;
			}
		}

		if ($firstpacket) {
			if ($results = mysql_query('select p,r from players where cookie=\'' . $cookie . '\'', $my_mysql)) {
				if ($row = mysql_fetch_row($results)) {
					$p = $row[0];
					$r = $row[1];
				} else {
					$results = mysql_query('replace into players (time, p, r, cookie, water) values (' . $time . ',\'' . $p . '\',\'' .  $r . '\',\'' . $cookie . '\',' . $time . ')', $my_mysql);
				}
			}
		}

		mysql_close ($my_mysql);
	}

	$pos = split(',', $p);
	$rot = split(',', $r);

	player($pos[0],$pos[1],$pos[2],$rot[0],$rot[1],$rot[2],0,'#' . substr($cookie, -3, 3));


?>],"u":<?php

	if ($firstpacket) {
		echo '[[' . $p . '],[' . $r . ']]';

	} else {
		echo 'null';
	}
?>,"h":<?php
	if ($health > 0)
		echo $health;
	else
		echo '0';
?>}
