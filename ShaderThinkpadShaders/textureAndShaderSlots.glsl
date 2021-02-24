precision mediump float;
uniform vec2 v2Resolution;
uniform float fGlobalTime;
//uniform sampler1D texFFT;
uniform sampler2D texture0;
uniform vec2 vec2texture0size;
uniform sampler2D texture1;
uniform vec2 vec2texture1size;
uniform sampler2D shader1;
uniform vec2 vec2shader1size;
vec4 getColor09(int nr){
	vec4 vec4colors[10];
	vec4colors[0]=vec4(1.,1.,1.,1.);
	vec4colors[1]=vec4(0.,0.,0.,1.);
	vec4colors[2]=vec4(1.,.23,0.,1.);
	vec4colors[3]=vec4(1.,1.,0.,1.);
	vec4colors[4]=vec4(0.,1.,0.,1.);
	vec4colors[5]=vec4(1.,0.,0.,1.);
	vec4colors[6]=vec4(1.,.2,.59,1.);
	vec4colors[7]=vec4(.5,0.,.5,1.);
	vec4colors[8]=vec4(.39,.2,0.,1.);
	vec4colors[9]=vec4(0.,0.,1.,1.);
	for(int i=0;i<10;i++){
	if(nr==i)return vec4colors[i];
	}
	return vec4(0.);
}
vec4 inputSlots(vec2 m,vec2 uv){
	vec4 color0=texture2D(texture0,m);
	vec4 color1=texture2D(texture1,0.5*m+0.5);
	vec4 color2=texture2D(shader1,0.5*uv+0.5);
	//vec4 color2=texture2D(shader1,0.7*uv+0.5);
	//vec4 color2=texture2D(shader1,uv+0.5);
	return color0+color1+color2;
	//return color2;
}
void main(void){
	vec2 uv=vec2(gl_FragCoord.x/v2Resolution.x,gl_FragCoord.y/v2Resolution.y);
	uv-=0.5;
	uv/=vec2(v2Resolution.y/v2Resolution.x,1);
	vec2 m;
	m.x=atan(uv.x/uv.y)/3.14;
	m.y=1./length(uv)*.2;
	float d=m.y;
	//float f=texture1D(texFFT,d).r*100;
	float f=0.;
	m.x+=sin(fGlobalTime)*0.1;
	m.y+=fGlobalTime*0.25;
	d=clamp(d,1.,10.);
	float fltColorMagnitude=0.5;
	vec4 t=fltColorMagnitude*inputSlots(m.yx*3.14,uv)/d;
	t=clamp(t,0.0,1.0);
	gl_FragColor=f+t;
	//gl_FragColor=getColor09(1);
}
