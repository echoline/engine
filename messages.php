<?php
	if (!isset($_POST['cookie']) || !isset($_POST['p'])) {
		echo 0;
		exit(-1);
	}

	$pos = explode(',', $_POST['p']);

	if (!is_numeric($pos[0]) ||
	    !is_numeric($pos[1]) ||
	    !is_numeric($pos[2])) {
		echo 1;
		exit(-2);
	}

	require 'login.php';

	$my_mysql = mysqli_connect($my_host, $my_user, $my_pass, 'game');
	if ($my_mysql != NULL) {
		$cookie = mysqli_real_escape_string($my_mysql, $_POST['cookie']);

		if (isset($_POST['message'])) {
			$message = mysqli_real_escape_string($my_mysql, htmlentities($_POST['message']));
			$results = mysqli_query ($my_mysql, 'select cookie,p from players where time>' . (time() - 3));

			if ($results != NULL) {
				while ($row = mysqli_fetch_row($results)) {
					$p = explode(',', $row[1]);

					$dx = ($p[0] - $pos[0]);
					$dy = ($p[1] - $pos[1]);
					$dz = ($p[2] - $pos[2]);
					$dist = sqrt($dx*$dx + $dy*$dy + $dz*$dz);

					if ($dist < 1500) {
						mysqli_query($my_mysql, 'insert into messages values (\'' . $cookie . '\',\'' . $row[0] . '\',\'' . $message . '\',' . time() . ')');
					}
				}
			}
		} else {
			$results = mysqli_query($my_mysql, 'select hither,message from (select * from messages where tither=\'' . $cookie . '\' and time>' . (time() - 900) . ' order by time desc limit 5) as blah order by time');
			if ($results != NULL) {
				while ($row = mysqli_fetch_row($results)) {
					$hither = substr($row[0], -3, 3);
					echo '<span style=\'color:#' . $hither . '\'>user' . $hither . '</span>: ' . $row[1] . '<br/>';
				}
			}
		}

		mysqli_close($my_mysql);
	}
?>
