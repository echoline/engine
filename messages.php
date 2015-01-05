<?php
	if (!isset($_POST['cookie']) || !isset($_POST['p'])) {
		echo 0;
		exit(-1);
	}

	$pos = split(',', $_POST['p']);

	if (!is_numeric($pos[0]) ||
	    !is_numeric($pos[1]) ||
	    !is_numeric($pos[2])) {
		echo 1;
		exit(-2);
	}

	require 'login.php';

	$my_mysql = mysql_connect($my_host, $my_user, $my_pass);
	if (($my_mysql != NULL) && mysql_select_db('game', $my_mysql)) {
		$cookie = mysql_real_escape_string($_POST['cookie']);

		if (isset($_POST['message'])) {
			$message = mysql_real_escape_string(htmlentities($_POST['message']));
			$results = mysql_query ('select cookie,p from players where time>' . (time() - 3), $my_mysql);

			if ($results != NULL) {
				while ($row = mysql_fetch_row($results)) {
					$p = split(',', $row[1]);

					$dx = ($p[0] - $pos[0]);
					$dy = ($p[1] - $pos[1]);
					$dz = ($p[2] - $pos[2]);
					$dist = sqrt($dx*$dx + $dy*$dy + $dz*$dz);

					if ($dist < 15000) {
						mysql_query('insert into messages values (\'' . $cookie . '\',\'' . $row[0] . '\',\'' . $message . '\',' . time() . ')', $my_mysql);
					}
				}
			}
		} else {
			$results = mysql_query('select hither,message from (select * from messages where tither=\'' . $cookie . '\' and time>' . (time() - 900) . ' order by time desc limit 5) as blah order by time', $my_mysql);
			if ($results != NULL) {
				while ($row = mysql_fetch_row($results)) {
					$hither = substr($row[0], -3, 3);
					echo '<span style=\'color:#' . $hither . '\'>user' . $hither . '</span>: ' . $row[1] . '<br/>';
				}
			}
		}

		mysql_close($my_mysql);
	}
?>
