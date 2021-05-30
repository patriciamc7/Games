
varying vec2 v_uv;

uniform sampler2D u_texture;
uniform sampler2D u_texture_1;
void main(){
    vec2 uv = v_uv;
  	//vec2 uv = fragCoord.xy / iResolution.xy;
       
	vec3 obump =  texture2D(u_texture,uv).rgb;
	float displace = dot(obump, vec3(0.3, 0.6, 0.1));
	displace = (displace - 0.5)*0.3;
	vec3 color = texture2D(u_texture_1,uv + (displace)).rgb;
	vec4 alpha = texture2D(u_texture_1,uv);
	if(alpha.a != 1.0)
		discard;
    
    gl_FragColor = vec4(color,0.5);
}