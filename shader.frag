#version 120

varying vec3 Normal;
varying vec3 ToLight;
varying vec4 PositionVCS;
uniform sampler2D texture;
varying vec2 texture_coordinate;
uniform int flag ;
void main()
{

    // get the texture color
    vec4 tColor = texture2D(texture, texture_coordinate);

    // apply Phong shading by using the following parameters
    vec4 ka = vec4(0.25,0.25,0.25,1.0); // reflectance coeff. for ambient
    vec4 Ia = vec4(0.3,0.3,0.3,1.0); // light color for ambient
    vec4 Id = vec4(1.0, 1.0, 1.0, 1.0); // light color for diffuse
    vec4 kd = vec4(1.0, 1.0, 1.0, 1.0); // reflectance coeff. for diffuse
    vec4 Is = vec4(1.0, 1.0, 1.0, 1.0); // light color for specular
    vec4 ks = vec4(1.0, 1.0, 1.0, 1.0); // reflectance coeff. for specular
    int specExp = 100; // specular exponent

    // ......
    // ......
    // compute ambient component

    vec4 ambient = ka*Ia ;  // ambient is not computed correectly here. this is just a placeholder

    // compute diffuse component


    // vec4(1.0,1.0,1.0,1.0);//dot(kd*Id*cosD); // diffuse is not computed correectly here. this is just a placeholder
    // compute specular component
    //vec4(1.0,1.0,1.0,1.0); // specular is not computed correectly here. this is just a placeholder

    vec3 L = normalize(ToLight-vec3(PositionVCS)); // computing the Light direction vector, Viewing direction vector ,half vector
    vec3 V = normalize(-1*vec3(PositionVCS))  ;
    vec3 H = normalize(L+vec3(V)) ;


    vec4 diffuse = kd*Id* max(0,dot(Normal,L));

    vec4 specular = ks*Is*pow(max(0,dot(Normal,H)),specExp);
    // compute the color using the following equation
    //vec3 color = clamp( tColor.xyz /* vec3(ambient + diffuse + specular)*/, 0.0, 1.0);
    if(flag==0)
      gl_FragColor = tColor * (ambient + diffuse + specular);
    else
      gl_FragColor =(ambient + diffuse + specular);
}
