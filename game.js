var mouseX;
var mouseY;
var cookie;
var firstpacket = true;

function onMouseMove(evt)
{
	var rect = canvas.getBoundingClientRect();

	mouseX = (evt.clientX - rect.left);
	mouseY = (evt.clientY - rect.top);
}

function onMouseOut(evt)
{
	mouseX = 400;
	mouseY = 200;
}

function onInit()
{
	cookie = getCookie('userhash');
	if (cookie == null) {
		var now = new Date();
		var time = now.getTime();

		time += 43200 * 365 * 1000;
		now.setTime(time);

		document.cookie='userhash=' + Math.round(Math.random()*1000000) + ';expires=' + now.toGMTString();
	}

	canvas=document.getElementById("mainCanvas");
	canvas.addEventListener ("mousemove", onMouseMove);
	canvas.addEventListener ("mouseout", onMouseOut);
 
	scene=new Scene3D(canvas);
	scene.origin.y = -15;
	scene.origin.z = -2400;

	mouseX = 400;
	mouseY = 200;

	setInterval(onRender, 33);

	update();
}

function update()
{
	var xhr = new XMLHttpRequest();
	xhr.onreadystatechange = function() {
		if (xhr.readyState === 4) {
			var stuff = JSON.parse(xhr.responseText);
			for (var i=0;i<stuff.s.length;i++) {
				if (scene.itemids[stuff.s[i].id] == undefined) {
					var ps = stuff.s[i].ps;
					var l = null;

					if (stuff.s[i].type=='polygon') {
						l = new Polygon3D();
					} else {
						l = new Line3D();
					}

					for (var j=0;j<ps.length;j++) {
						var p = new Point3D();
						p.x = ps[j][0];
						p.y = ps[j][1];
						p.z = ps[j][2];
						l.addPoint(p);
					}

					var o = Object3D(l);
					scene.addItem(o, stuff.s[i].id);

					o.position.x = stuff.s[i].p[0];
					o.position.y = stuff.s[i].p[1];
					o.position.z = stuff.s[i].p[2];

					o.rotation.x = stuff.s[i].r[0];
					o.rotation.y = stuff.s[i].r[1];
					o.rotation.z = stuff.s[i].r[2];
				}

				var o = scene.itemids[stuff.s[i].id];

				if (stuff.s[i].c)
					o.colour = stuff.s[i].c;

				if (stuff.s[i].f)
					o.fillColour = stuff.s[i].f;

				if (stuff.s[i].p && (stuff.s[i].p.length==3)) {
					o.positionv.x = (stuff.s[i].p[0] - o.position.x) / 10.0;
					o.positionv.y = (stuff.s[i].p[1] - o.position.y) / 10.0;
					o.positionv.z = (stuff.s[i].p[2] - o.position.z) / 10.0;
				}

				if (stuff.s[i].w)
					o.lineWidth = stuff.s[i].w;

				if (stuff.s[i].r.length == 3) {
//					o.rotationv.x = (stuff.s[i].r[0] - o.rotation.x) / 10;
//					o.rotationv.y = (stuff.s[i].r[1] - o.rotation.y) / 10;
//					o.rotationv.z = (stuff.s[i].r[2] - o.rotation.z) / 10;
					o.rotation.x = stuff.s[i].r[0];
					o.rotation.y = stuff.s[i].r[1];
					o.rotation.z = stuff.s[i].r[2];
				}

				scene.itemids[stuff.s[i].id] = o;

				if (stuff.u) {
					scene.origin.x = stuff.u[0][0];
					scene.origin.z = stuff.u[0][2];

					scene.rotation.y = stuff.u[1][1];
				}
				if (stuff.h) {
					scene.health = stuff.h;
				}
			}
			if (scene.health > 0)
				setTimeout("update()", 333);
		}
	};
	xhr.open ('POST', 'update', true);
	xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	var cookie = getCookie('userhash');

	if (cookie != null) {
		var ry = scene.rotation.y;
		var x = scene.origin.x;// + Math.sin(ry) * scene.focalLength;
		var z = scene.origin.z;// - Math.cos(ry) * scene.focalLength;
		xhr.send ('cookie=' + encodeURIComponent(cookie)
			+ '&p=' + encodeURIComponent('[' + x
			+ ',0,' + z + ']')
			+ '&r=' + encodeURIComponent('[0,' + ry 
			+ ',0]') + '&f=' + firstpacket);

		firstpacket = false;
	}
}

function onRender()
{
	for(var i=0;i<scene.items.length;i++)
		if (scene.items[i].think)
	       		scene.items[i].think();

	var x = mouseX - 400;
	var y = mouseY - 200;
	scene.rotation.y -= Math.pow(x / 6400, 3) * 250;

	var speed = y / 16;
	if (Math.abs(speed) > 2)
		goForward(speed);

	scene.render();
}

function goForward(speed)
{
	scene.origin.x += Math.sin(scene.rotation.y) * speed;
	scene.origin.z -= Math.cos(scene.rotation.y) * speed;
}

function getCookie(c_name)
{
	var c_value = document.cookie;
	var c_start = c_value.indexOf(" " + c_name + "=");
	if (c_start == -1)
	{
		c_start = c_value.indexOf(c_name + "=");
	}

	if (c_start == -1)
	{
		c_value = null;
	}
	else
	{
		c_start = c_value.indexOf("=", c_start) + 1;
		var c_end = c_value.indexOf(";", c_start);
		if (c_end == -1)
		{
			c_end = c_value.length;
		}
		c_value = unescape(c_value.substring(c_start,c_end));
	}

	return c_value;
}
