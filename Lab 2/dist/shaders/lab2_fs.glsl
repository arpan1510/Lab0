#version 400
uniform sampler2D diffuse_tex;
uniform float time;
uniform float cm;
uniform vec3 pl_position;
uniform vec3 pl_color;

in vec2 tex_coord; 
in vec3 normal;
in vec3 FragPos;

out vec4 fragcolor; //the output color for this fragment    

void main(void)
{   

	// Diffuse lighting
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(pl_position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * pl_color;

	fragcolor = texture(diffuse_tex, tex_coord)*cm*vec4(diffuse/length(pl_position - FragPos),0); //read color from texture and assign to outgoing fragment color
}




















