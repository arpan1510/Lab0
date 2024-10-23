#version 400            
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
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
    FragPos = vec3(M * vec4(pos_attrib, 1.0));
    mat3 normalMatrix = transpose(inverse(mat3(M)));
    normal = normalize(normalMatrix * normal_attrib);
    
    float wave = sin(pos_attrib.x * 10 + time * speed) * 0.01;
    vec3 modified_position = pos_attrib;
    modified_position.y += wave;
    gl_Position = P*V*M * vec4(modified_position, 1.0);
	
	tex_coord = tex_coord_attrib; //send tex_coord to fragment shader
    

}