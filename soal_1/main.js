"use strict";
const KEYLEFT = 37;
const KEYUP = 38;
const KEYRIGHT = 39;
const KEYDOWN = 40;
const KEY_A = 65;
const KEY_W = 87;
const KEY_D = 68;
const KEY_S = 83;
const MOVE_OFFSET = 20;
const CANVAS_WIDTH = 1200;
const CANVAS_HEIGHT = 720;

var canvas;
var gl;
var colorUniformLocation;
var translation = [30, 10]; //top-left of rectangle
const rectwidth = 20;      // rectangle width
const rectheight = 100;   // rectangle height
var translationLocation;


$(document).ready(function(){
  init();

  $("#sliderX").change(function(e){
    translation[0] = e.target.value;
    $("#Xvalue").innerHTML = translation[0];
    render();
  });

  $("#sliderY").change(function(e){
    translation[1] = e.target.value;
    $("#Yvalue").innerHTML = translation[1];
    render();
  });

  $(document).keydown(function(event) {
    // Move Up
    if (event.which == KEYUP || event.which == KEY_W){
      if (translation[1] > 0){
        translation[1] = Math.max(0, translation[1] - MOVE_OFFSET);
      }
    }

    // Move down
    if (event.which == KEYDOWN || event.which == KEY_S){
      if (translation[1] < CANVAS_HEIGHT - rectheight){
        translation[1] = Math.min(CANVAS_HEIGHT, translation[1] + MOVE_OFFSET);
      }
    }

    render();
  });

  setInterval(render, 100);
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

	setRectangle(gl, translation[0], translation[1], rectwidth, rectheight);	
	
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

function render() 
{
	gl.uniform4f(colorUniformLocation, 1.0, 0, 0, 1);
	gl.uniform2fv(translationLocation, translation);
		
	var primitiveType = gl.TRIANGLES;
	var offset = 0;
	var count = 6;
		

	gl.clear( gl.COLOR_BUFFER_BIT );
	gl.drawArrays( primitiveType, offset, count );
	
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
 
