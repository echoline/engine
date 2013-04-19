var dx = 0.0382;
var dy = 0.0618;

function onMouseMove(evt)
{
        canvas=document.getElementById("mainCanvas");

	var hx = canvas.width;
	var hy = canvas.height;
	var rect = canvas.getBoundingClientRect();

	dx = (evt.clientX - rect.left) - hx;
	dx /= hx * 10;
	dy = (evt.clientY - rect.top) - hy;
	dy /= hy * 10;
}

window.onload = function onInit()
{
        canvas=document.getElementById("mainCanvas");
	canvas.addEventListener ("mousemove", onMouseMove);
 
        scene=new Scene3D(canvas);

	var cube=new Cube3D();
	cube.think=function() {
		this.rotationX += dy;
		this.rotationY += dx;
	}
        scene.addItem(cube);
        
	var cube=new Cube3D();
	cube.think=function() {
		this.rotationX -= dy;
		this.rotationY += dx;
	}
	cube.position.x -= 50;
        scene.addItem(cube);
        
	var cube=new Cube3D();
	cube.think=function() {
		this.rotationX += dy;
		this.rotationY -= dx;
	}
	cube.position.x += 50;
        scene.addItem(cube);
        
        setInterval(onRender, 33);
}

function Cube3D() {
        var cube=new Line3D();

	cube.addPoint(new Point3D(-10,-10,-10));
	cube.addPoint(new Point3D(-10,-10,10));
	cube.addPoint(new Point3D(10,-10,10));
	cube.addPoint(new Point3D(10,10,10));
	cube.addPoint(new Point3D(-10,10,10));
	cube.addPoint(new Point3D(-10,10,-10));
	cube.addPoint(new Point3D(10,10,-10));
	cube.addPoint(new Point3D(10,-10,-10));
	cube.addPoint(new Point3D(-10,-10,-10));
	cube.addPoint(new Point3D(10,10,10));
	cube.addPoint(new Point3D(10,10,-10));
	cube.addPoint(new Point3D(-10,-10,10));
	cube.addPoint(new Point3D(-10,10,10));
	cube.addPoint(new Point3D(10,-10,-10));
	cube.addPoint(new Point3D(10,-10,10));
	cube.addPoint(new Point3D(-10,10,-10));
	cube.addPoint(new Point3D(-10,-10,-10));

	var obj = new Object3D(cube);

	return obj;
}       
 
function onRender()
{
        for(var i=0;i<scene.items.length;i++)
		if (scene.items[i].think)
	       	        scene.items[i].think()

        scene.render();
}

