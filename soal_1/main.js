"use strict";
const KEYUP = 38;
const KEYDOWN = 40;
const KEY_W = 87;
const KEY_S = 83;

const MOVE_CIRCLE_OFFSET = 7;
const MOVE_OFFSET = 70;
const CANVAS_WIDTH = 1200;
const CANVAS_HEIGHT = 720;

const CIRCLE_RADIUS = 20;
const CIRCLE_X = CANVAS_WIDTH/2;
const CIRCLE_Y = CANVAS_HEIGHT/2;

const rectwidth = 20;      // rectangle width
const rectheight = 150;   // rectangle height

const TRIANGLE_MIN_X = rectwidth + 200;
const TRIANGLE_MIN_Y = 100;
const TRIANGLE_MAX_X = CANVAS_WIDTH - 100;
const TRIANGLE_MAX_Y = CANVAS_HEIGHT - 100;
const TRIANGLE_SIZE = 50;
const HIT_BOX_TRIANGLE_DISTANCE = 45;

var canvas;
var gl;
var colorUniformLocation;
var matrixLocation;
var translationRectangle = [100, CANVAS_HEIGHT/2 - rectheight/2];
var translationCircle = [0,0];

var translationLocation;

var circleDirection = [1,0];
var triangleAngle = 0;
var triangleState = {};

var user_score = 0;
var inGame = false;

$(document).ready(function(){
    $("#start-button").click(function(event){
        $(this).attr("disabled", "disabled");
        inGame = true;
        $("#user-score").text(user_score);
        init();
    });

  $(document).keydown(function(event) {
    if (!inGame){return;}

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

//source : https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/random
function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min)) + min; //The maximum is exclusive and the minimum is inclusive
}

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

    initTriangleState();

    render();
}

function initTriangleState(){
    triangleState = {
        'x' : getRandomInt(TRIANGLE_MIN_X, TRIANGLE_MAX_X),
        'y' : getRandomInt(TRIANGLE_MIN_Y, TRIANGLE_MAX_Y),
        'r' : TRIANGLE_SIZE,
        'angle' : 0,
    };
}

function render(){
    gl.clear( gl.COLOR_BUFFER_BIT );
    setRectangle(gl, 0, 0, rectwidth, rectheight); 
    render_rectangle(); 

    setBall(gl, CIRCLE_RADIUS, CIRCLE_X, CIRCLE_Y );
    moveBall();
    render_ball();

    triangleHandle();

    if (!checkLose()){
        requestAnimationFrame(render);
    } else {
        gameStopHandle();
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
    var count = 52;

    gl.drawArrays( primitiveType, offset, count );

}

function render_triangle(){
    gl.uniform4f(colorUniformLocation, 0.0, 1.0, 1.0, 1);
    gl.uniform2fv(translationLocation, [0,0]);

    var primitiveType = gl.TRIANGLE_FAN;
    var offset = 0;
    var count = 4;

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
  for (let i = 0; i <= 50; i++){
    points.push(vec2(
        r*Math.cos(2*Math.PI*i/50.0) + x,
        r*Math.sin(2*Math.PI*i/50.0) + y
        ));

}
gl.bufferData(gl.ARRAY_BUFFER, flatten(points), gl.STATIC_DRAW);
}

function setTriangle(gl, x, y, r, angle){
  var x1 = r * Math.cos(2*Math.PI*angle/100.0) + x;
  var y1 = r * Math.sin(2*Math.PI*angle/100.0) + y;
  var x2 = r * Math.cos(2*Math.PI*angle/100.0 + (2.0/3)*Math.PI) + x;
  var y2 = r * Math.sin(2*Math.PI*angle/100.0 + (2.0/3)*Math.PI) + y;
  var x3 = r * Math.cos(2*Math.PI*angle/100.0 + (4.0/3)*Math.PI) + x;
  var y3 = r * Math.sin(2*Math.PI*angle/100.0 + (4.0/3)*Math.PI) + y;

  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([
    x, y,
    x1, y1,
    x2, y2,
    x3, y3,
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
    checkCollisionWithTriangle();
    if (translationCircle[0] + CIRCLE_X - CIRCLE_RADIUS < 0) circleDirection[0] = 1;
    if (translationCircle[0] + CIRCLE_X + CIRCLE_RADIUS > CANVAS_WIDTH) circleDirection[0] = 0;
    if (translationCircle[1] + CIRCLE_Y - CIRCLE_RADIUS < 0) circleDirection[1] = 1;
    if (translationCircle[1] + CIRCLE_Y + CIRCLE_RADIUS > CANVAS_HEIGHT) circleDirection[1] = 0;
}

function checkCollision(){
    let cirleLeftmostPoint = translationCircle[0] + CIRCLE_X - CIRCLE_RADIUS;
    if (cirleLeftmostPoint <= rectwidth + translationRectangle[0] && 
        cirleLeftmostPoint > rectwidth + translationRectangle[0] - 20){
        let circleTopPoint = translationCircle[1] + CIRCLE_Y - CIRCLE_RADIUS;
        let circleBottomPoint = translationCircle[1] + CIRCLE_Y + CIRCLE_RADIUS;

        if (circleTopPoint >= translationRectangle[1] - 10 && circleBottomPoint <= rectheight + translationRectangle[1] + 10){
            circleDirection[0] = 1;
        }
    }
}

function checkCollisionWithTriangle(){
    var circleCenterPoint = [translationCircle[0] + CIRCLE_X, translationCircle[1] + CIRCLE_Y];

    var circleDistanceToTriangle = euclidDistance(circleCenterPoint[0], circleCenterPoint[1]
        , triangleState.x, triangleState.y)

    if (circleDistanceToTriangle < HIT_BOX_TRIANGLE_DISTANCE){
        user_score += 1;
        updateScore();
        initTriangleState();
    }
}

function checkLose(){
    let circleCenterPoint = translationCircle[0] + CIRCLE_X;
    return circleCenterPoint <= rectwidth
}

function triangleHandle(){
    triangleState.angle  = (triangleState.angle + 1) % 100;
    setTriangle(gl, triangleState.x, triangleState.y ,triangleState.r, triangleState.angle);
    render_triangle();
}

function euclidDistance(x1,y1,x2,y2){
    return Math.sqrt(Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2))
}

function gameStopHandle(){
    $("#start-button").attr("disabled", false);
    user_score = 0;
    translationRectangle = [100, CANVAS_HEIGHT/2 - rectheight/2];
    translationCircle = [0,0];
}

function updateScore(){
    $("#user-score").text(user_score);
}