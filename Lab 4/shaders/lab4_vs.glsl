#version 400
in vec3 pos_attrib1;
void main(void)
{
 gl_Position = vec4(pos_attrib1, 1.0);
}