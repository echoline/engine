/**
* Matrix3D
* 
* @author Masayuki Daijima (ARCHETYP Inc.)
* http://www.daijima.jp/blog/
* http://twitter.com/daijimachine
*/

function Matrix3D(
	n11, n12, n13, n14, 
	n21, n22, n23, n24, 
	n31, n32, n33, n34, 
	n41, n42, n43, n44
){
	this.n11=n11 || 1;
	this.n12=n12 || 0;
	this.n13=n13 || 0;
	this.n14=n14 || 0;
	this.n21=n21 || 0;
	this.n22=n22 || 1;
	this.n23=n23 || 0;
	this.n24=n24 || 0;
	this.n31=n31 || 0;
	this.n32=n32 || 0;
	this.n33=n33 || 1;
	this.n34=n34 || 0;
	this.n41=n41 || 0;
	this.n42=n42 || 0;
	this.n43=n43 || 0;
	this.n44=n44 || 1;
}

Matrix3D.prototype.clone=function()
{
	return new Matrix3D(this.n11, this.n12, this.n13, this.n14, this.n21, this.n22, this.n23, this.n24, this.n31, this.n32, this.n33, this.n34, this.n41, this.n42, this.n43, this.n44);
}

Matrix3D.prototype.concat=function(m)
{
	var values={};
	
	values.n11=this.n11*m.n11+this.n12*m.n21+this.n13*m.n31+this.n14*m.n41;
	values.n12=this.n11*m.n12+this.n12*m.n22+this.n13*m.n32+this.n14*m.n42;
	values.n13=this.n11*m.n13+this.n12*m.n23+this.n13*m.n33+this.n14*m.n43;
	values.n14=this.n11*m.n14+this.n12*m.n24+this.n13*m.n34+this.n14*m.n44;
							 
	values.n21=this.n21*m.n11+this.n22*m.n21+this.n23*m.n31+this.n24*m.n41;
	values.n22=this.n21*m.n12+this.n22*m.n22+this.n23*m.n32+this.n24*m.n42;
	values.n23=this.n21*m.n13+this.n22*m.n23+this.n23*m.n33+this.n24*m.n43;
	values.n24=this.n21*m.n14+this.n22*m.n24+this.n23*m.n34+this.n24*m.n44;
							 
	values.n31=this.n31*m.n11+this.n32*m.n21+this.n33*m.n31+this.n34*m.n41;
	values.n32=this.n31*m.n12+this.n32*m.n22+this.n33*m.n32+this.n34*m.n42;
	values.n33=this.n31*m.n13+this.n32*m.n23+this.n33*m.n33+this.n34*m.n43;
	values.n34=this.n31*m.n14+this.n32*m.n24+this.n33*m.n34+this.n34*m.n44;
							 
	values.n41=this.n41*m.n11+this.n42*m.n21+this.n43*m.n31+this.n44*m.n41;
	values.n42=this.n41*m.n12+this.n42*m.n22+this.n43*m.n32+this.n44*m.n42;
	values.n43=this.n41*m.n13+this.n42*m.n23+this.n43*m.n33+this.n44*m.n43;
	values.n44=this.n41*m.n14+this.n42*m.n24+this.n43*m.n34+this.n44*m.n44;
	
	this.initialize(values);
}

Matrix3D.prototype.initialize=function(values)
{
	for(var i in values) this[i]=values[i];
}

Matrix3D.prototype.createBox=function(scalex, scaley, scalez, rotationx, rotationy, rotationz, tx, ty, tz)
{
	this.identity();
	if (rotationx != 0) this.rotateX(rotationx);
	if (rotationy != 0) this.rotateY(rotationy);
	if (rotationz != 0) this.rotateZ(rotationz);
	if (scalex != 1 || scaley != 1 || scalez != 1) this.scale(scalex, scaley, scalez);
	if (tx != 0 || ty != 0 || tz != 0) this.translate(tx, ty, tz);
}

Matrix3D.prototype.identity=function()
{
	this.initialize({n11:1, n12:0, n13:0, n14:0, n21:0, n22:1, n23:0, n24:0, n31:0, n32:0, n33:1, n34:0, n41:0, n42:0, n43:0, n44:1});
}

Matrix3D.prototype.rotateX=function(angle)
{
	var sin=Math.sin(angle);
	var cos=Math.cos(angle);
	
	this.concat(new Matrix3D(
		1, 0, 0, 0, 
		0, cos, -sin, 0,	
		0, sin, cos, 0,	
		0, 0, 0, 1)
	);
}

Matrix3D.prototype.rotateY=function(angle)
{
	var sin=Math.sin(angle);
	var cos=Math.cos(angle);

	this.concat(new Matrix3D(
		cos, 0, -sin, 0, 
		0, 1, 0, 0, 
		sin, 0, cos, 0, 
		0, 0, 0, 1)
	);
}

Matrix3D.prototype.rotateZ=function(angle)
{
	var sin=Math.sin(angle);
	var cos=Math.cos(angle);
	
	this.concat(new Matrix3D(
		cos, sin, 0, 0, 
		-sin, cos, 0, 0, 
		0, 0, 1, 0, 
		0, 0, 0, 1)
	);
}

Matrix3D.prototype.scale=function(sx, sy, sz)
{
	this.concat(new Matrix3D(
		sx, 0, 0, 0, 
		0, sy, 0, 0, 
		0, 0, sz, 0, 
		0, 0, 0, 1)
	);
}

Matrix3D.prototype.translate=function(dx, dy, dz)
{
	this.n41+=dx;
	this.n42+=dy;
	this.n43+=dz;
}

Matrix3D.prototype.transformPoint=function(point)
{
	var oX=point.x;
	var oY=point.y;
	var oZ=point.z;
	
	point.x=this.n11*oX+this.n21*oY+this.n31*oZ+this.n41, 
	point.y=this.n12*oX+this.n22*oY+this.n32*oZ+this.n42, 
	point.z=this.n13*oX+this.n23*oY+this.n33*oZ+this.n43;
}

Matrix3D.prototype.transformArray=function(arr)
{
	var rVal=[];

	var numPoints=arr.length/3;
	
	for(var i=0;i<numPoints;i++)
	{
		var i3=i*3;
		var x=arr[i3];
		var y=arr[i3+1];
		var z=arr[i3+2];
		
		rVal[i3]=this.n11*x+this.n21*y+this.n31*z+this.n41;
		rVal[i3+1]=this.n12*x+this.n22*y+this.n32*z+this.n42;
		rVal[i3+2]=this.n13*x+this.n23*y+this.n33*z+this.n43;
	}
	
	return rVal;
}

Matrix3D.prototype.toString=function()
{
	return this.n11+","+this.n12+","+this.n13+","+this.n14+","+
		this.n21+","+this.n22+","+this.n23+","+this.n24+","+
		this.n31+","+this.n32+","+this.n33+","+this.n34+","+
		this.n41+","+this.n42+","+this.n43+","+this.n44;
}
