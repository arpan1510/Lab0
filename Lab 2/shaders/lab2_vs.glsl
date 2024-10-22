#version 400            
uniform mat4 PVM;
uniform float time;
uniform float speed;

in vec3 pos_attrib; //this variable holds the position of mesh vertices
in vec2 tex_coord_attrib;
in vec3 normal_attrib;  

out vec2 tex_coord;
out vec3 normal;
out vec3 FragPos;

void main(void)
{
    float wave = sin(pos_attrib.x * 10 + time * speed) * 0.01;
    vec3 modified_position = pos_attrib;
    modified_position.y += wave;
    gl_Position = PVM * vec4(modified_position, 1.0);
	
	tex_coord = tex_coord_attrib; //send tex_coord to fragment shader
    normal = normal_attrib;
    FragPos = pos_attrib;
}