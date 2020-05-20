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

	$pos_0 = explode(',', $p);
	$rot_0 = explode(',', $r);

	if (!is_numeric($pos_0[0]) ||
	    !is_numeric($pos_0[1]) ||
	    !is_numeric($pos_0[2]) ||
	    !is_numeric($rot_0[0]) ||
	    !is_numeric($rot_0[1]) ||
	    !is_numeric($rot_0[2]))
		exit(-3);

	$firstpacket = ($_POST['f'] == 'true');

	srand(0);

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
		oasis(0, 0, 0, $i++, 150);
		echo ',';
		oasis(8000, 0, 20000, $i++, 150);
		echo ',';
		oasis(-8000, 0, -20000, $i++, 150);
		echo ',';
		oasis(0, 0, 0, $i++, 150);
		echo ',';
	}

	$my_mysql = mysqli_connect($my_host, $my_user, $my_pass, 'game');
	if ($my_mysql != NULL) {
		$p = mysqli_real_escape_string($my_mysql, $pos_0[0] . ',' . $pos_0[1] . ',' .  $pos_0[2]);
		$r = mysqli_real_escape_string($my_mysql, $rot_0[0] . ',' . $rot_0[1] . ',' .  $rot_0[2]);
		$cookie = mysqli_real_escape_string($my_mysql, $_POST['cookie']);

		if (!$firstpacket) {
			$results = mysqli_query($my_mysql, 'select water from players where cookie=\'' . $cookie . '\'');
			$water = mysqli_fetch_row ($results);
			if ($water != NULL)
				$water = $water[0];
			$pos = explode(',', $p);
			$dist = sqrt($pos[0]*$pos[0] + $pos[2]*$pos[2]);
			if ($dist < 150.0) {
				$water = $time;
			}
			$dist = sqrt(((8000 - $pos[0])*(8000 - $pos[0])) +
				    ((20000 - $pos[2])*(20000 - $pos[2])));
			if ($dist < 150.0) {
				$water = $time;
			}
			$dist = sqrt(((-8000 - $pos[0])*(-8000 - $pos[0])) +
				    ((-20000 - $pos[2])*(-20000 - $pos[2])));
			if ($dist < 150.0) {
				$water = $time;
			}
			$health = (300 - ($time - $water)) / 300.0;
			if ($health > 0)
				$results = mysqli_query($my_mysql, 'replace into players (time, p, r, cookie, water) values (' . $time . ',\'' . $p . '\',\'' .  $r . '\',\'' . $cookie . '\',' . $water . ')');
		}

		$results = mysqli_query($my_mysql, 'select p,r,cookie,time from players where cookie!=\'' . $cookie . '\'');
		if ($results != NULL) {
			$i = 1;
			while ($row = mysqli_fetch_row($results)) {
				$pos = explode(',', $row[0]);
				$rot = explode(',', $row[1]);

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
			if ($results = mysqli_query($my_mysql, 'select p,r from players where cookie=\'' . $cookie . '\'')) {
				if ($row = mysqli_fetch_row($results)) {
					$p = $row[0];
					$r = $row[1];
				} else {
					$results = mysqli_query($my_mysql, 'replace into players (time, p, r, cookie, water) values (' . $time . ',\'' . $p . '\',\'' .  $r . '\',\'' . $cookie . '\',' . $time . ')');
				}
			}
		}

		mysqli_close ($my_mysql);
	}

	$pos = explode(',', $p);
	$rot = explode(',', $r);

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
