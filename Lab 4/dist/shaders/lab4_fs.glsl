#version 400
uniform sampler2D diffuse_tex;
uniform float time;
uniform float cm;
uniform float alpha;
uniform vec3 pl_position;
uniform vec3 ld;
uniform vec3 la;
uniform vec3 ls;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform vec3 camPos;
uniform bool isTexEnabled;

in vec2 tex_coord; 
in vec3 normal;
in vec3 FragPos;

out vec4 fragcolor; //the output color for this fragment    

void main(void)
{ 
    
    fragcolor = vec4(1,1,1,0);
    
}




















