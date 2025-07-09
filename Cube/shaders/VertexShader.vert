#version 430

layout( location = 0 ) in vec3 vs_in_pos;
layout( location = 1 ) in vec3 vs_in_uv;
layout( location = 2 ) in vec3 vs_in_color;


uniform mat4 viewProj;
uniform mat4 world;

out vec3 fs_out_col;
out vec3 fs_out_norm;
out vec3 fs_out_pos;
out vec3 fs_out_uv;

void main()
{ 
	gl_Position = viewProj * world * vec4( vs_in_pos, 1.0 );

	fs_out_col = vs_in_color;
	fs_out_pos=vs_in_pos;
	fs_out_uv=vs_in_uv;

	
}