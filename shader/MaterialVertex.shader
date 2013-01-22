#version 120

//Material Vertex Shader

// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition_modelspace;
attribute vec3 vertexNormal_modelspace;

// Output data ; will be interpolated for each fragment.
varying vec3 vertexNormal_eyespace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 VM;

void main(){
    gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);

    vertexNormal_eyespace = normalize( vec3( VM * vec4(vertexNormal_modelspace, 0)) );
}

