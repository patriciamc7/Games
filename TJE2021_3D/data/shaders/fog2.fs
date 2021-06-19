
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;
varying float v_fogDepth;

uniform int u_alpha;
uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;
uniform float u_texture_tiling;

uniform vec3 u_light_ambient;
uniform vec3 u_light_position;
uniform vec3 u_light_direction;
uniform vec3 u_light_color;
uniform int u_light_type;
uniform float u_light_maxdist;
uniform float u_light_cutoffCos;
uniform float u_light_exponent;
uniform float u_light_intensity;

//fog
uniform vec4 u_fogColor;
uniform float u_fog_density;
uniform vec2 u_fog_settings; 
vec3 L;
float NdotL;
vec3 N ;
float spotFactor = 0.0;
uniform vec3 u_camera_position; 
vec3 getDirectionalLight(vec3 light) //DIRECTIONAL LIGHT
{
	L = normalize(-u_light_direction);
	N = normalize( v_normal);
	NdotL = max( dot(N,L), 0.0 ); 
	light += NdotL * u_light_color * u_light_intensity;

	return light;
}
vec3 getSpotLight(vec3 light) //SPOT LIGHT
{
	L = normalize( u_light_position - v_world_position); 
	N = normalize( v_normal);
	NdotL = max( dot(N,L), 0.0); 
	float spotCosine = dot(-u_light_direction,L); 

	if (spotCosine >= u_light_cutoffCos){	
		spotFactor = pow(spotCosine, u_light_exponent);
	}
	light += NdotL * u_light_color * u_light_intensity * spotFactor;	
	return light;
}
void main()
{
	const float log2 = 1.442695;
	float z = gl_FragCoord.z / gl_FragCoord.w;

	//pasamos al shader tanto el color de la niebla como la distancia max y min de la niebla
	float dist = length( u_camera_position - v_world_position );
	float fog_factor = clamp( 1.0 - (u_fog_settings.y - dist) / (u_fog_settings.y - u_fog_settings.x),0.0,1.0);

	vec2 uv = v_uv;
	vec4 color = u_color * texture2D( u_texture, uv * u_texture_tiling );
	//PHONG
	vec3 light = u_light_ambient;

	if (u_light_type==1){ //SPOT
		light+=getSpotLight(light);
	}
	if (u_light_type==2){ //DIRECTIONAL
		light+=getDirectionalLight(light);
	}
	color.xyz *= light;
	if (u_alpha == 1)
		color.a = 0.0f;

	gl_FragColor = mix(color,u_fogColor, fog_factor);
}
