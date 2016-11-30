#version 120
#extension GL_ARB_gpu_shader5 : enable

uniform vec4 lightPosition;
uniform float heightFactor;
uniform int width; // width of the texture in pixels
uniform int height; // height of the texture in pixels
uniform sampler2D texture; // OpenGL texture id

varying vec3 Normal; // to store the computed normal vector of the vertex
varying vec3 ToLight; // the vector from the vertex to the light source
varying vec4 PositionVCS; // the position of the vertex in Viewing Coord. Sys.
varying vec2 texture_coordinate; // texture coordinate of the vertex
// these varying variable are interpolated by the rasterized and passed to fragment shader


void main()
{
	   texture_coordinate = vec2(gl_MultiTexCoord0);
		 PositionVCS = gl_ModelViewMatrix * gl_Vertex ;


     vec4 texel, newVertex ,texel1,texel2,texel3,texel4,texel5,texel6 ;
		 vec3 nor1 , nor2 , nor3 , nor4 , nor5 , nor6 , ver1 ,ver2 , ver3 , ver4 , ver5 , ver6  ;
     //Read the texture offset. Offset in the z direction only

     texel = texture2D(texture,texture_coordinate);
     newVertex = gl_Vertex; // get My Pixel
     newVertex.y = texel.x * 0.2126 + texel.y* 0.7152 + texel.z * 0.0722; // calculate the height
     newVertex.y = heightFactor * newVertex.y;

		 gl_Position = gl_ModelViewProjectionMatrix * newVertex ;

		 //calculating the color of the adjacent vertecies
     texel1 = texture2DLod(texture,vec2(texture_coordinate.x,texture_coordinate.y-(1.0/height)) , 0.0);
		 ver1.y = texel1.x * 0.2126 + texel1.y*0.7152 + texel1.z * 0.0722;
     ver1.y = heightFactor * ver1.y;
     texel2 = texture2DLod(texture,vec2(texture_coordinate.x+(1.0/width),texture_coordinate.y) , 0.0);
		 ver2.y = texel2.x * 0.2126 + texel2.y*0.7152 + texel2.z * 0.0722;
     ver2.y = heightFactor * ver2.y;
     texel3 = texture2DLod(texture,vec2(texture_coordinate.x-(1.0/width),texture_coordinate.y) , 0.0);
		 ver3.y = texel3.x * 0.2126 + texel3.y*0.7152 + texel3.z * 0.0722;
     ver3.y = heightFactor * ver3.y;
     texel4 = texture2DLod(texture,vec2(texture_coordinate.x,texture_coordinate.y+(1.0/height)) , 0.0);
		 ver4.y = texel4.x * 0.2126 + texel4.y*0.7152 + texel4.z*0.0722;
     ver4.y = heightFactor * ver4.y;
     texel5 = texture2DLod(texture,vec2(texture_coordinate.x+(1.0/width),texture_coordinate.y-(1.0/height)), 0.0);
		 ver5.y = texel5.x * 0.2126 + texel5.y*0.7152 + texel5.z*0.0722;
     ver5.y = heightFactor * ver5.y;
     texel6 = texture2DLod(texture,vec2(texture_coordinate.x-(1.0/width),texture_coordinate.y+(1.0/height)) , 0.0);
		 ver6.y = texel6.x*0.2126 + texel6.y*0.7152 + texel6.z*0.0722;
     ver6.y = heightFactor * ver6.y;

		 /*ver1 = vec3(gl_Position.x,ver1.y,gl_Position.z-1) ;
		 ver2 = vec3(gl_Position.x+1,ver2.y,gl_Position.z) ;
		 ver3 = vec3(gl_Position.x-1,ver3.y,gl_Position.z) ;
		 ver4 = vec3(gl_Position.x,ver4.y,gl_Position.z+1) ;
		 ver5 = vec3(gl_Position.x+1,ver5.y,gl_Position.z-1) ;
		 ver6 = vec3(gl_Position.x-1,ver6.y,gl_Position.z+1) ;

 		 nor1 = cross(ver5-vec3(newVertex),ver1-vec3(newVertex)) ;
   	 nor2 = cross(ver2-vec3(newVertex),ver5-vec3(newVertex)) ;
   	 nor3 = cross(ver4-vec3(newVertex),ver2-vec3(newVertex)) ;
   	 nor4 = cross(ver6-vec3(newVertex),ver4-vec3(newVertex)) ;
   	 nor5 = cross(ver3-vec3(newVertex),ver6-vec3(newVertex)) ;
   	 nor6 = cross(ver1-vec3(newVertex),ver3-vec3(newVertex)) ;
		 */

		ver1 = vec3(0.0,ver1.y-newVertex.y,-1.0) ;
		ver2 = vec3(1.0,ver2.y-newVertex.y,0) ;
		ver3 = vec3(-1.0,ver3.y-newVertex.y,0) ;
		ver4 = vec3(0.0,ver4.y-newVertex.y,1.0) ;
		ver5 = vec3(+1.0,ver5.y-newVertex.y,-1.0) ;
		ver6 = vec3(-1.0,ver6.y-newVertex.y,1.0) ;

		 nor1 = cross(ver5,ver1) ;
  	 nor2 = cross(ver2,ver5) ;
  	 nor3 = cross(ver4,ver2) ;
  	 nor4 = cross(ver6,ver4) ;
  	 nor5 = cross(ver3,ver6) ;
  	 nor6 = cross(ver1,ver3) ;

		// vec3 positione = vec3(gl_Vertex.x,newVertex.y,gl_Vertex.z) ;

		 Normal = nor1+nor2+nor3+nor4+nor5+nor6 ;
		 Normal = normalize((transpose(inverse(gl_ModelViewMatrix))*vec4(Normal,1.0)).xyz) ;


		 //ToLight = normalize(lightPosition.xyz-PositionVCS.xyz)  ;
		 ToLight = vec3(gl_ModelViewMatrix*lightPosition);

}
