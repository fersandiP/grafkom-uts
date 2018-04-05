"use strict";
const KEYLEFT = 37;
const KEYUP = 38;
const KEYRIGHT = 39;
const KEYDOWN = 40;
const KEY_A = 65;
const KEY_W = 87;
const KEY_D = 68;
const KEY_S = 83;
const MOVE_OFFSET = 50;
const MOVE_CIRCLE_OFFSET = 7;
const CANVAS_WIDTH = 1200;
const CANVAS_HEIGHT = 720;

const CIRCLE_RADIUS = 20;
const CIRCLE_X = CANVAS_WIDTH/2;
const CIRCLE_Y = CANVAS_HEIGHT/2;
const rectwidth = 20;      // rectangle width
const rectheight = 150;   // rectangle height

var canvas;
var gl;
var colorUniformLocation;
var translationRectangle = [100, CANVAS_HEIGHT/2 - rectheight/2]; //top-left of rectangle
var translationCircle = [0,0];

var translationLocation;

var circleDirection = [1,0];


$(document).ready(function(){
  init();

  $(document).keydown(function(event) {
    // Move Up
    if (event.which == KEYUP || event.which == KEY_W){
      if (translationRectangle[1] > 0){
        translationRectangle[1] = Math.max(0, translationRectangle[1] - MOVE_OFFSET);
      }
    }

    // Move down
    if (event.which == KEYDOWN || event.which == KEY_S){
      if (translationRectangle[1] < CANVAS_HEIGHT - rectheight){
        translationRectangle[1] = Math.min(CANVAS_HEIGHT - rectheight, translationRectangle[1] + MOVE_OFFSET);
      }
    }
  });

});

function init()
{
  canvas = document.getElementById( "gl-canvas" );

  gl = WebGLUtils.setupWebGL( canvas );
  if ( !gl ) { alert( "WebGL isn't available" ); }

    //
    //  Configure WebGL
    //
    gl.viewport( 0, 0, canvas.width, canvas.height );
    gl.clearColor( 0.0, 0.0, 0.0, 1.0 );

    //  Load shaders and initialize attribute buffers
    var program = initShaders( gl, "vertex-shader", "fragment-shader" );
    gl.useProgram( program );

    // Load the data into the GPU
    var bufferId = gl.createBuffer();
    gl.bindBuffer( gl.ARRAY_BUFFER, bufferId );

    // Associate out shader variables with our data buffer

    var vPosition = gl.getAttribLocation( program, "vPosition" );
    gl.vertexAttribPointer( vPosition, 2, gl.FLOAT, false, 0, 0 );
    gl.enableVertexAttribArray( vPosition );

	// set the translation
	var resolutionUniformLocation = gl.getUniformLocation(program, "u_resolution");
	translationLocation = gl.getUniformLocation(program, "u_translation");
	
	// set the resolution
  gl.uniform2f(resolutionUniformLocation, gl.canvas.width, gl.canvas.height);
  colorUniformLocation = gl.getUniformLocation(program, "u_color");

	render(); //default render
}

function render(){
  gl.clear( gl.COLOR_BUFFER_BIT );
  setRectangle(gl, 0, 0, rectwidth, rectheight); 
  render_rectangle();

  setBall(gl, CIRCLE_RADIUS, CIRCLE_X, CIRCLE_Y );
  moveBall();
  render_ball();

  setRectangle(gl, 100,120,150);
  render_rectangle();

  if (!checkLose()){
    requestAnimationFrame(render);
  }
}

function render_rectangle(){
  gl.uniform4f(colorUniformLocation, 1.0, 0, 0, 1);
  gl.uniform2fv(translationLocation, translationRectangle);
    
  var primitiveType = gl.TRIANGLES;
  var offset = 0;
  var count = 6;
    
  gl.drawArrays( primitiveType, offset, count );

}

function render_ball() 
{
    gl.uniform4f(colorUniformLocation, 0.0, 1.0, 0, 1);
    gl.uniform2fv(translationLocation, translationCircle);

	var primitiveType = gl.TRIANGLE_FAN;
	var offset = 0;
	var count = 102;
		
	gl.drawArrays( primitiveType, offset, count );
	
}

function render_triangle(){
    gl.uniform4f(colorUniformLocation, 0.0, 1.0, 0, 1);

    var primitiveType = gl.TRIANGLES;
    var offset = 0;
    var count = 3;

    gl.drawArrays(primitiveType, offset, count);
}

// Fill the buffer with the values that define a rectangle.
function setRectangle(gl, x, y, width, height) {
  var x1 = x;
  var x2 = x + width;
  var y1 = y;
  var y2 = y + height;
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([
     x1, y1,
     x2, y1,
     x1, y2,
     x1, y2,
     x2, y1,
     x2, y2,
  ]), gl.STATIC_DRAW);
}

function setBall(gl, r, x, y){
  var points = []
  var center = vec2(x, y); 

  points.push(center);
  for (let i = 0; i <= 100; i++){
    points.push(vec2(
        r*Math.cos(2*Math.PI*i/100.0) + x,
        r*Math.sin(2*Math.PI*i/100.0) + y
    ));

    }
  gl.bufferData(gl.ARRAY_BUFFER, flatten(points), gl.STATIC_DRAW);
}

function setTriangle(gl,x,y,z){
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([
        x, y,
        y, z,
        z, x,
        ]), gl.STATIC_DRAW);
}

function moveBall(){
    if (circleDirection[0]){
        translationCircle[0] += MOVE_CIRCLE_OFFSET;
    } else {
        translationCircle[0] -= MOVE_CIRCLE_OFFSET;
    }

    if (circleDirection[1]){
        translationCircle[1] += MOVE_CIRCLE_OFFSET;
    } else {
        translationCircle[1] -= MOVE_CIRCLE_OFFSET;
    }

    checkCollision();
    if (translationCircle[0] + CIRCLE_X - CIRCLE_RADIUS < 0) circleDirection[0] = 1;
    if (translationCircle[0] + CIRCLE_X + CIRCLE_RADIUS > CANVAS_WIDTH) circleDirection[0] = 0;
    if (translationCircle[1] + CIRCLE_Y - CIRCLE_RADIUS < 0) circleDirection[1] = 1;
    if (translationCircle[1] + CIRCLE_Y + CIRCLE_RADIUS > CANVAS_HEIGHT) circleDirection[1] = 0;
}

function checkCollision(){
    let cirleLeftmostPoint = translationCircle[0] + CIRCLE_X - CIRCLE_RADIUS;
    if (cirleLeftmostPoint <= rectwidth + translationRectangle[0] && cirleLeftmostPoint > rectwidth + translationRectangle[0] - 20){
        let circleTopPoint = translationCircle[1] + CIRCLE_Y - CIRCLE_RADIUS;
        let circleBottomPoint = translationCircle[1] + CIRCLE_Y + CIRCLE_RADIUS;

        if (circleTopPoint >= translationRectangle[1] && circleBottomPoint <= rectheight + translationRectangle[1]){
            circleDirection[0] = 1;
        }
    }
}

function checkLose(){
    let circleCenterPoint = translationCircle[0] + CIRCLE_X;
    return circleCenterPoint <= rectwidth
}
 
var m3 = {            //setup 3x3 transformation matrix object
   identity: function() {
    return [
      1, 0, 0,
      0, 1, 0,
      0, 0, 1,
    ];
   },
   
   projection: function(width, height) {
    // Note: This matrix flips the Y axis so that 0 is at the top.
    return [
      2 / width, 0, 0,
      0, -2 / height, 0,
      -1, 1, 1
    ];
   },

  translation: function(tx, ty) {
    return [
      1, 0, 0,
      0, 1, 0,
      tx, ty, 1,
    ];
  },

  rotation: function(angleInRadians) {
    var c = Math.cos(angleInRadians);
    var s = Math.sin(angleInRadians);
    return [
      c,-s, 0,
      s, c, 0,
      0, 0, 1,
    ];
  },

  scaling: function(sx, sy) {
    return [
      sx, 0, 0,
      0, sy, 0,
      0, 0, 1,
    ];
  },

  multiply: function(a, b) {
    var a00 = a[0 * 3 + 0];
    var a01 = a[0 * 3 + 1];
    var a02 = a[0 * 3 + 2];
    var a10 = a[1 * 3 + 0];
    var a11 = a[1 * 3 + 1];
    var a12 = a[1 * 3 + 2];
    var a20 = a[2 * 3 + 0];
    var a21 = a[2 * 3 + 1];
    var a22 = a[2 * 3 + 2];
    var b00 = b[0 * 3 + 0];
    var b01 = b[0 * 3 + 1];
    var b02 = b[0 * 3 + 2];
    var b10 = b[1 * 3 + 0];
    var b11 = b[1 * 3 + 1];
    var b12 = b[1 * 3 + 2];
    var b20 = b[2 * 3 + 0];
    var b21 = b[2 * 3 + 1];
    var b22 = b[2 * 3 + 2];
    return [
      b00 * a00 + b01 * a10 + b02 * a20,
      b00 * a01 + b01 * a11 + b02 * a21,
      b00 * a02 + b01 * a12 + b02 * a22,
      b10 * a00 + b11 * a10 + b12 * a20,
      b10 * a01 + b11 * a11 + b12 * a21,
      b10 * a02 + b11 * a12 + b12 * a22,
      b20 * a00 + b21 * a10 + b22 * a20,
      b20 * a01 + b21 * a11 + b22 * a21,
      b20 * a02 + b21 * a12 + b22 * a22,
    ];
  },
};