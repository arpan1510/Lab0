#version 400
uniform sampler2D diffuse_tex;
uniform float time;

in vec2 tex_coord;
in vec3 normal;

out vec4 fragcolor; 

void main(void)
{   
	
	//fragcolor = texture(diffuse_tex, tex_coord); //read color from texture and assign to outgoing fragment color
	//fragcolor = vec4(tex_coord, 0.0, 1.0);
	fragcolor = vec4(abs(normal), 1.0);
}




















