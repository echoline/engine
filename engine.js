// based on tutorial at http://www.netmagazine.com/tutorials/build-your-own-html5-3d-engine

function Scene3D(canvas)
{
	this.matrix=new Matrix3D();
	this.rotationX=0;
	this.rotationY=0;
	this.scale=1.5;
	this.focalLength=1000;
	this.context=canvas.getContext("2d");
	this.sceneWidth=canvas.width;
	this.sceneHeight=canvas.height;
	this.orig3D=[];
	this.points3D=[];
	this.points2D=[];
	this.numPoints=0;
	this.items=[];

	this.render=function()
	{
        	var halfWidth=this.sceneWidth*0.5;
	        var halfHeight=this.sceneHeight*0.5;
 
        	this.matrix.identity();
	        this.matrix.scale(this.scale, this.scale, this.scale);
        	this.matrix.rotateX(this.rotationX);
	        this.matrix.rotateY(this.rotationY);
//        	this.matrix.translate(0, 0, 0);

	        for(var i=0;i<this.items.length;i++)
	        {
			this.items[i].updateTranslations();
		}

	        var transformed=this.matrix.transformArray(this.points3D);
 
        	for(var i=0;i<this.numPoints;i++)
	        {
        	        var i3=i*3;
	                var i2=i*2;
 
        	        var x=transformed[i3];
                	var y=transformed[i3+1];
	                var z=transformed[i3+2];
 
        	        var scale=this.focalLength/(z+this.focalLength);
 
	                this.points2D[i2]=x*scale+halfWidth;
        	        this.points2D[i2+1]=y*scale+halfHeight;
	        }
 
	        this.context.fillStyle="white";
	        this.context.fillRect(0, 0, this.sceneWidth, this.sceneHeight);
	        this.context.save();
 
	        for(var i=0;i<this.items.length;i++)
	        {
        	        this.items[i].render(this.context);
        	}
	 
	        this.context.restore();
	}


	this.setupPoint=function(x, y, z)
	{
		var returnVal=this.numPoints;
		
		this.points2D[this.points2D.length]=0;
		this.points2D[this.points2D.length]=0;
		
		this.orig3D[this.orig3D.length]=x;
		this.orig3D[this.orig3D.length]=y;
		this.orig3D[this.orig3D.length]=z;

		this.points3D[this.points3D.length]=x;
		this.points3D[this.points3D.length]=y;
		this.points3D[this.points3D.length]=z;

		this.numPoints++;

		return returnVal;
	}

	this.addItem=function(item)
	{
        	this.items[this.items.length]=item;
	        item.addToScene(this);
	}
}

function Point3D(xVal, yVal, zVal)
{
	var _x=xVal!=undefined?xVal:0;
	var _y=yVal!=undefined?yVal:0;
	var _z=zVal!=undefined?zVal:0;
	var myScene=null;
	var xIdx;
	var yIdx;
	var zIdx;
	var xIdx2D;
	var yIdx2D;

	this.setupWithScene=function(scene)
	{
		myScene=scene;

		var idx=scene.setupPoint(_x, _y, _z);
		var i3=idx*3;
		var i2=idx*2;

		xIdx=i3;
		yIdx=i3+1;
		zIdx=i3+2;

		xIdx2D=i2;
		yIdx2D=i2+1;
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

	this.origX=function()
	{
		if (myScene!=null)
			return myScene.orig3D[xIdx];

		return null;
	}

	this.origY=function()
	{
		if (myScene!=null)
			return myScene.orig3D[yIdx];

		return null;
	}

	this.origZ=function()
	{
		if (myScene!=null)
			return myScene.orig3D[zIdx];

		return null;
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

	this.addToScene=function(scene)
	{
		for(var i=0;i<this.points.length;i++)
		{
			this.points[i].setupWithScene(scene);
		}
	}

	this.addPoint=function(point)
	{
		this.points[this.points.length]=point;
	}

	this.render=function(context)
	{
		context.beginPath();
		context.strokeStyle=this.colour;

		for(var i=0;i<this.points.length;i++)
		{
			context.lineTo(this.points[i].x2D, this.points[i].y2D); 
		}

		context.stroke();
	}
}

function Object3D(o) {
	o.matrix=new Matrix3D();
	o.rotationX=0;
	o.rotationY=0;
	o.scale=1;
	o.position=new Point3D();

	o.updateTranslations=function()
	{
		this.matrix.identity();
		this.matrix.scale(this.scale, this.scale, this.scale);
		this.matrix.rotateX(this.rotationX);
		this.matrix.rotateY(this.rotationY);
		this.matrix.translate(this.position.x, this.position.y,
					this.position.z);

		var r = [];

		for (var i=0;i<this.points.length;i++)
		{
			r[i*3] = this.points[i].origX();
			r[i*3+1] = this.points[i].origY();
			r[i*3+2] = this.points[i].origZ();
		}

	        var transformed=this.matrix.transformArray(r);
		var l = transformed.length / 3;

		for (var i=0;i<l;i++)
		{
			this.points[i].x = transformed[i*3];
			this.points[i].y = transformed[i*3+1];
			this.points[i].z = transformed[i*3+2];
		}
	}

	return o;
}
