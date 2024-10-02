#version 400
out vec4 fragcolor; //the output color for this fragment
uniform float time;
               
void main(void)
{   
	//Exercise: try to change the color to red
	
		fragcolor = vec4(1, 0+time*0.25, 0+time*0.25, 1.0);
	
}




















