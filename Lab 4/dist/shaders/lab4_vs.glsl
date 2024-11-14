#version 400            
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float time;
uniform float speed;

in vec3 pos_attrib; //this variable holds the position of mesh vertices
in vec3 pos_attrib1;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;  

out vec2 tex_coord;
out vec3 normal;
out vec3 FragPos;

void main(void)
{
    gl_Position = vec4(pos_attrib, 1.0);
}