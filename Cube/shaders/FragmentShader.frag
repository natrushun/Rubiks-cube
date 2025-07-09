#version 430

 in vec3 fs_out_col;
 in vec3 fs_out_pos;
 in vec3 fs_out_uv;

 
 out vec4 color;

uniform vec3 view;      




void main()
 {
    vec3 FragColor;
    vec3 lightColor= vec3(1.0, 1.0, 1.0);
    float ambientStrength = 0.95;
    vec3 ambient = ambientStrength * lightColor;

    if (fs_out_uv.x < 0.05 || fs_out_uv.x > 0.95 || fs_out_uv.y < 0.05 ||fs_out_uv.y > 0.95)
      FragColor = ambient *  vec3(0.0, 0.0, 0.0);
    else
      FragColor = ambient * fs_out_col;

    color = vec4(FragColor,1.0);
 } 