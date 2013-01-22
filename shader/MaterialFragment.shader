#version 120

// Material Fragment Shader

struct Material {
    vec4 ambientFactor;
    vec4 diffuseFactor;
    vec4 specularFactor;
    vec4 emissiveValue;
    float shininess;
};

// Interpolated values from the vertex shaders
varying vec3 vertexNormal_eyespace;

// Values that stay constant for the whole mesh.
uniform mat4 VM;
uniform Material material;

void main(){
    //ambiant
    vec4 ambient = material.ambientFactor*gl_LightSource[0].ambient;
    //diffuse
    vec3 lightDirection = normalize( vec3( VM * gl_LightSource[0].position ) );
    float cosNormaLight = max(0, dot(vertexNormal_eyespace, lightDirection ));
    vec4 diffuse = material.diffuseFactor*gl_LightSource[0].diffuse*cosNormaLight;

    gl_FragColor = clamp(material.emissiveValue + (ambient + diffuse), 0, 1);
}

