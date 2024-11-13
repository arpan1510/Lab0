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
    //Ambient Lighting
    vec3 ambient= ka*la;

	// Diffuse lighting
    vec3 nw = normalize(normal);
    vec3 lw = normalize(pl_position - FragPos);
    vec3 diffuse = kd*ld*max(dot(nw, lw), 0.0);

    // Specular Lighting
    vec3 vw = normalize(camPos-FragPos);
    vec3 rw = reflect(-lw, nw);
    vec3 specular = ks*ls*pow(max(dot(vw, rw), 0.0), alpha);

    vec3 pointLightResult= ambient+(diffuse+specular)/pow(length(pl_position - FragPos), 2);

    if (isTexEnabled) {
        fragcolor = texture(diffuse_tex, tex_coord)*cm*vec4(pointLightResult,0);
    } else {
        fragcolor = cm*vec4(pointLightResult,0);
    }
}




















