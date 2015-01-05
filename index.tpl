	<script type="text/javascript" src="Matrix3D.js"></script>
	<script type="text/javascript" src="engine.js"></script>
	<script type="text/javascript" src="game.js"></script>
	<canvas id="mainCanvas" style="width:800px; border:1px solid #aaa;"></canvas>
	<script type="text/javascript">
	<!--

		function teleport() {
			var x = document.getElementById('xinput');
			var z = document.getElementById('zinput');

			scene.origin.x = parseFloat(x.value);
			scene.origin.z = parseFloat(z.value);

			x.value = '';
			z.value = '';

			window.teleporting = false;
		}

		window.onload = function() {
			onInit();
			window.teleporting = false;

			setInterval(function() {
				if (!window.teleporting) {
					document.getElementById('xinput').value = scene.origin.x;
					document.getElementById('zinput').value = scene.origin.z;
				}
			}, 100);

			setInterval(function() {
				var xhr = new XMLHttpRequest();
				xhr.onreadystatechange = function() {
					if (xhr.readyState == 4) {
						document.getElementById('messages').innerHTML = xhr.responseText;
					}
				};
				xhr.open('POST', 'messages', true);
				xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
				xhr.send('p=' + scene.origin.x + ',0,' + scene.origin.z + '&cookie=' + getCookie('userhash'));
			}, 1000);
		}

		function say() {
			var xhr = new XMLHttpRequest();
			xhr.open('POST', 'messages', true);
			xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
			xhr.send('p=' + scene.origin.x + ',0,' + scene.origin.z + '&cookie=' + getCookie('userhash') + '&message=' + document.getElementById('message').value);
			document.getElementById('message').value = '';
		}
	// -->
	</script>
	x:<input id="xinput" onfocus="window.teleporting=true"></input> z:<input id="zinput" onfocus="window.teleporting=true"></input>
	<input type="button" value="Teleport" onclick="teleport()" autofocus="autofocus"></input><br/>
	<div id="messages"></div>
	<input type="text" style="width:700px" id="message"></input>
	<input type="button" value="Say" onclick="say()"></input>
