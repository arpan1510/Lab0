#version 400            
uniform mat4 PVM;
uniform float time;
uniform float speed;

in vec3 pos_attrib; //this variable holds the position of mesh vertices
in vec2 tex_coord_attrib;
in vec3 normal_attrib;  

out vec2 tex_coord; 

void main(void)
{
	// Modify the y-coordinate of the position using the sine wave
    float wave = sin(pos_attrib.x * 10 + time * speed) * 0.01;

    // Apply the wave to the vertex position (in the y-axis in this case)
    vec3 modified_position = pos_attrib;
    modified_position.y += wave;

    // Transform the modified vertex position with the PVM matrix
    gl_Position = PVM * vec4(modified_position, 1.0);
	//gl_Position = PVM*vec4(pos_attrib, 1.0); //transform vertices and send result into pipeline
	
	tex_coord = tex_coord_attrib; //send tex_coord to fragment shader
}