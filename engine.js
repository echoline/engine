// based on tutorial at http://www.netmagazine.com/tutorials/build-your-own-html5-3d-engine

function Scene3D(canvas)
{
	this.matrix=new Matrix3D();
	this.rotation={"x":0,"y":0,"z":0};
	this.scale=1;
	this.focalLength=1000;
	this.context=canvas.getContext("2d");
	this.sceneWidth=canvas.width;
	this.sceneHeight=canvas.height;
	this.orig3D=[];
	this.points3D=[];
	this.points2D=[];
	this.points=[];
	this.polygons=[];
	this.polygonsDone=[];
	this.shown=[];
	this.order=[];
	this.order2=[];
	this.numPoints=0;
	this.items=[];
	this.itemids=[];
	this.origin={"x":0,"y":0,"z":0};
	this.health = 1;

	this.render=function()
	{
        	var halfWidth=this.sceneWidth*0.5;
	        var halfHeight=this.sceneHeight*0.5;
 
	        for(var i=0;i<this.items.length;i++)
	        {
			this.items[i].updateTranslations();
			this.polygonsDone[i]=true;
		}

		this.matrix.identity();
		this.matrix.translate(-this.origin.x, -this.origin.y,
					-this.origin.z);
		this.matrix.rotateX(this.rotation.x);
		this.matrix.rotateY(this.rotation.y);
		this.matrix.rotateZ(this.rotation.z);
		this.matrix.translate(0, 0, -this.focalLength * 0.3);
		this.matrix.rotateX(-0.0618);

		var transformed=this.matrix.transformArray(this.points3D);

        	for(var i=0;i<this.numPoints;i++)
	        {
        	        var i3=i*3;
	                var i2=i*2;
 
        	        var ax=transformed[i3];
                	var ay=transformed[i3+1];
	                var az=transformed[i3+2];

	        	var scale=this.focalLength/(az+this.focalLength);
			var x = ax * scale;
			var y = ay * scale;

			this.order[i] = i;
			this.order2[i] = az;

		        this.points2D[i2] = x + halfWidth;
        		this.points2D[i2+1] = y + halfHeight;

			if ((az > -this.focalLength) &&
			    (Math.abs(x) <= halfWidth) &&
			    (Math.abs(y) <= halfHeight)) {
				this.shown[i] = true;
				var o = this.points[i].belongs;
				if (o.constructor == Polygon3D) {
					this.polygonsDone[o.polygonIdx] = false;
				}
			} else {
				this.shown[i] = false;
			}
	        }
 
		// sort by depth.
        	for(var i=0;i<this.numPoints;i++) {
        		for(var j=0;j<this.numPoints;j++) {
				if (this.order2[this.order[i]] > this.order2[this.order[j]]) {
					var tmp = this.order[i];
					this.order[i] = this.order[j];
					this.order[j] = tmp;
				}
			}
		}

	        for(var i=0;i<this.items.length;i++)
			if (this.items[i].updateShown)
				this.items[i].updateShown();

	        this.context.fillStyle='#048';
	        this.context.fillRect(0, 15, this.sceneWidth, this.sceneHeight-15);
	        this.context.fillStyle='#ace';
	        this.context.fillRect(0, 0, this.sceneWidth, 15);
	        this.context.save();
 
		for(var i=0;i<this.numPoints;i++) {
			var o = this.points[this.order[i]].belongs;
			if (o.constructor == Line3D) {
				var idx = this.points[this.order[i]].sceneIdx;

				if (scene.shown[idx]) {
					var here = this.points[this.order[i]].objIdx;
					var to = (here+1)%o.points.length;
					var w = this.focalLength - this.order2[idx];
					w /= this.focalLength;
					w *= o.lineWidth;
					if (w < 0.3)
						w = 0.3;
		
					this.context.beginPath();
					this.context.strokeStyle=o.colour;

					this.context.lineWidth = w;
					this.context.lineTo(o.points[here].x2D, o.points[here].y2D); 
					this.context.lineTo(o.points[to].x2D, o.points[to].y2D); 
					this.context.stroke();
				}
			} else if (o.constructor == Polygon3D) {
				if (!this.polygonsDone[o.sceneIdx]) {
					this.context.lineWidth = 1;
					o.render(this.context);
				}
			}
		}

	        this.context.restore();
		this.context.strokeStyle = '#F00';
		this.context.lineWidth=3;
		this.context.beginPath();
		this.context.lineTo (25, this.sceneHeight-15);
		this.context.lineTo (25 + (this.sceneWidth-50) * this.health, this.sceneHeight-15);
		this.context.stroke();
	}


	this.setupPoint=function(p)
	{
		var x = p.x;
		var y = p.y;
		var z = p.z;
		var returnVal=this.numPoints;
		
		this.points2D[this.points2D.length]=0;
		this.points2D[this.points2D.length]=0;
		
		this.points[this.points.length]=p;

		this.points3D[this.points3D.length]=x;
		this.points3D[this.points3D.length]=y;
		this.points3D[this.points3D.length]=z;

		this.orig3D[this.orig3D.length]=x;
		this.orig3D[this.orig3D.length]=y;
		this.orig3D[this.orig3D.length]=z;

		this.shown[this.shown.length]=true;

		this.order[this.order.length]=0;
		this.order2[this.order2.length]=0;

		this.numPoints++;

		return returnVal;
	}

	this.addItem=function(item, id)
	{
        	this.items[this.items.length]=item;
		this.itemids[id]=item;
	        item.addToScene(this);
	}
}

function Point3D(xVal, yVal, zVal)
{
	var _x=xVal!=undefined?xVal:0;
	var _y=yVal!=undefined?yVal:0;
	var _z=zVal!=undefined?zVal:0;
	var myScene=null;
	var mySceneIdx=null;
	var xIdx;
	var yIdx;
	var zIdx;
	var xIdx2D;
	var yIdx2D;

	this.belongs=null;

	this.setupWithScene=function(scene, b)
	{
		myScene=scene;
		this.belongs=b;

		var idx=scene.setupPoint(this);
		var i3=idx*3;
		var i2=idx*2;

		xIdx=i3;
		yIdx=i3+1;
		zIdx=i3+2;

		xIdx2D=i2;
		yIdx2D=i2+1;

		mySceneIdx = idx;
	}

	this.getSceneIdx=function()
	{
		return mySceneIdx;
	}

	this.getX=function()
	{
		return _x;
	}

	this.setX=function(value)
	{
		if(myScene!=null)
			myScene.points3D[xIdx]=value;

		_x=value;
	}

	this.getY=function()
	{
		return _y;
	}

	this.setY=function(value)
	{
		if(myScene!=null)
			myScene.points3D[yIdx]=value;

		_y=value;
	}

	this.getZ=function()
	{
		return _z;
	}

	this.setZ=function(value)
	{
		if(myScene!=null)
			myScene.points3D[zIdx]=value;

		_z=value;
	}

	this.getX2D=function()
	{
		return myScene.points2D[xIdx2D];
	}

	this.getY2D=function()
	{
		return myScene.points2D[yIdx2D];
	}

	this.__defineGetter__("sceneIdx", this.getSceneIdx);
	this.__defineGetter__("x", this.getX);
	this.__defineGetter__("y", this.getY);
	this.__defineGetter__("z", this.getZ);
	this.__defineSetter__("x", this.setX);
	this.__defineSetter__("y", this.setY);
	this.__defineSetter__("z", this.setZ);
	this.__defineGetter__("x2D", this.getX2D);
	this.__defineGetter__("y2D", this.getY2D);
}

function Line3D()
{
	this.colour="black";
	this.points=[];
	this.lineWidth=2;

	this.addToScene=function(scene)
	{
		for(var i=0;i<this.points.length;i++)
		{
			this.points[i].setupWithScene(scene, this);
		}
	}

	this.addPoint=function(point)
	{
		point.objIdx = this.points.length;
		this.points[this.points.length]=point;
	}

	this.updateShown=function()
	{
		var shown = [];

		for(var i=0;i<this.points.length;i++) {
			shown[i] = false;

		}

		for(var i=0;i<this.points.length;i++)
			if (scene.shown[this.points[i].sceneIdx]) {
				shown[(i-1+this.points.length)%this.points.length] = true;
				shown[i] = true;
				shown[(i+1)%this.points.length] = true;
			}

		for(var i=0;i<this.points.length;i++) {
			if (shown[i]) {
				var idx = this.points[i].sceneIdx;
				scene.shown[idx] = true;
			}
		}
	}

	this.render=null; /*function(context)
	{
		context.beginPath();
		context.strokeStyle=this.colour;

		this.updateShown();

		for(var i=0;i<this.points.length;i++)
		{
			if (scene.shown[this.points[i].sceneIdx]) {
				context.lineTo(this.points[i].x2D, this.points[i].y2D); 
			} else {
				context.stroke();
				context.beginPath();
				context.strokeStyle=this.colour;
			}
		}

		context.stroke();
	}*/
}

function Polygon3D()
{
	this.fillColour="white";
	this.colour="black";
	this.points=[];

	this.addToScene=function(scene)
	{
		this.polygonIdx=scene.polygons.length;
		scene.polygonsDone[scene.polygons.length]=false;
		scene.polygons[scene.polygons.length]=this;

		for(var i=0;i<this.points.length;i++)
		{
			this.points[i].setupWithScene(scene, this);
		}
	}

	this.addPoint=function(point)
	{
		this.points[this.points.length]=point;
	}

	this.render=function(context)
	{
		if (scene.polygonsDone[this.polygonIdx]) {
			return;
		}

		context.beginPath();
		context.fillStyle=this.fillColour;
		context.strokeStyle=this.colour;

		for(var i=0;i<this.points.length;i++) {
			context.lineTo(this.points[i].x2D, this.points[i].y2D); 
		}

		context.closePath();
		context.fill();
		context.stroke();

		scene.polygonsDone[this.polygonIdx] = true;
	}
}

function Object3D(o) {
	o.matrix=new Matrix3D();
	o.rotation = {"x":0,"y":0,"z":0};
	o.rotationv = {"x":0,"y":0,"z":0};
	o.position = {"x":0,"y":0,"z":0};
	o.positionv = {"x":0,"y":0,"z":0};

	o.updateTranslations=function()
	{
		this.position.x += this.positionv.x;
		this.position.y += this.positionv.y;
		this.position.z += this.positionv.z;

		this.rotation.x += this.rotationv.x;
		this.rotation.y += this.rotationv.y;
		this.rotation.z += this.rotationv.z;

		this.matrix.identity();
		this.matrix.rotateX(-this.rotation.x);
		this.matrix.rotateY(-this.rotation.y);
		this.matrix.rotateZ(-this.rotation.z);
		this.matrix.translate(this.position.x,
				this.position.y, this.position.z);

		for (var i=0;i<this.points.length;i++)
		{
			var idx = this.points[i].sceneIdx;
			this.points[i].x = scene.orig3D[idx*3];
			this.points[i].y = scene.orig3D[idx*3+1];
			this.points[i].z = scene.orig3D[idx*3+2];
			this.matrix.transformPoint(this.points[i]);
		}
	}

	o.think=null;

	return o;
}

Sphere3D=function(){};
Sphere3D.prototype = Object.create(Polygon3D.prototype);
