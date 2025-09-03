#version 410 core

uniform float fGlobalTime; // in seconds
uniform vec2 v2Resolution; // viewport resolution (in pixels)

uniform sampler1D texFFT; // towards 0.0 is bass / lower freq, towards 1.0 is higher / treble freq
uniform sampler1D texFFTSmoothed; // this one has longer falloff and less harsh transients
uniform sampler1D texFFTIntegrated; // this is continually increasing

layout(location = 0) out vec4 out_color; // out_color must be written in order to see anything

mat2 rot(float a){
  return mat2(cos(a),sin(a),-sin(a),cos(a));
}
#define rollSize 2
float rollTime=1.2*mod(fGlobalTime,1000);
float theBall,thePint,theFoam;
float ball(vec3 p,float r){
  p.x+=rollSize*sin(rollTime);
  theBall=length(p)-r;
  return theBall;
}
vec3 ball_n(vec3 p){
  return normalize(vec3(ball(p+vec3(1,0,0),1),ball(p+vec3(0,1,0),1),ball(p+vec3(0,0,1),1)));
}
float cylinder(vec3 p,float r,float h){
    return max(length(p.xy)-r,abs(p.z)-h);
}
#define PI 3.1415
void pint_run(inout vec3 p){
  float runSpeed=1.2;
  float fRunTime=runSpeed*(mod(fGlobalTime,1000))-1;
  if(cos(fRunTime)>0){
   p.x+=1.9*rollSize;
    p.z+=15*sin(fRunTime)-7.5;
  }else{
    p.x-=1.9*rollSize;
    p.z-=15*sin(fRunTime-0.5)-7.5;
  }
  p.yz*=-rot(PI/2.); 
}
void testmovement(inout vec3 p){
  p.yz*=rot(mod(fGlobalTime,1000));
}
void move(inout vec3 p){
  pint_run(p);
  //testmovement(p);
}
float ear(vec3 p,float rInner,float rOuter){
    return length(vec2(length(p.xz)-rOuter,p.y))-rInner;
}
#define TAU (2*PI)
float pint(vec3 p){
  float a=PI/8;float b=PI/4;  
  move(p);
  thePint=(p.z>-0.5)?cylinder(p,1.2*sin(a*p.z+b),2):(p.z>-1.85)?cylinder(p,0.65,2):cylinder(p,0.7,2);  thePint=max(p.z-1.85,abs(thePint)-0.02);
  //theGlass=(p.z>-1.5)?cylinder(p,1.2*sin(1.3*(a*p.z+b)),2):(p.z>-1.85)?cylinder(p,0.3,2):cylinder(p,sin(-3*p.z+PI/2.),2);  theGlass=max(p.z-1.85,abs(thePint)-0.02);
  //float theGlass=(p.z>-1.5)?cylinder(p,1.2*sin(1.3*(a*p.z+b)),2):(p.z>-1.85)?cylinder(p,0.3,2):cylinder(p,1.,2);  theGlass=max(p.z-1.85,abs(theGlass)-0.02);
  float theGlass=max(p.z-1.85,abs((p.z>-1.5)?cylinder(p,1.2*sin(1.3*(a*p.z+b)),2):(p.z>-1.85)?cylinder(p,0.3,2):cylinder(p,1.,2))-0.02);
  //  theGlass=max(p.z-1.85,abs(theGlass)-0.02);
  
  //float theMug=(p.z>-1.85)?cylinder(p,1.2,2):cylinder(p,1.25,2);
  
  //theMug=max(p.z-1.85,abs((p.z>-1.85)?cylinder(p,1.2,2):cylinder(p,1.25,2))-0.02);
  //thePint=min(theMug,ear(vec3((p.x-1.5>-0.33)?p.x-1.5:10,p.y,p.z),.3,0.85));
  //thePint=min(max(p.z-1.85,abs((p.z>-1.85)?cylinder(p,1.2,2):cylinder(p,1.25,2))-0.02),ear(vec3((p.x-1.5>-0.33)?p.x-1.5:10,p.y,p.z),.3,0.85));
  ///float dasBeer=min(max(p.z-1.85,abs((p.z>-1.85)?cylinder(p,1.2,2):cylinder(p,1.25,2))-0.02),(p.x-1.5>-0.33)?length(vec2(length(vec2(p.z,p.x-1.5))-.85,p.y))-.3:10);
  ///thePint=dasBeer;
  ///thePint=theGlass;
 
  
  //thePint=ear(p,.5,2);
  return thePint;
}
float foam(vec3 p){
  float a=PI/8;float b=PI/4;  
  move(p);
  p+=sin(p.y*10)*cos(p.x*10)*.05;
  float r=(p.z>-0.5)?min(1.2*sin(a*p.z+b),1.15):(p.z>-1.85)?0.65:0.7;
  theFoam=(2>(p.z)&&p.z>1.3&&length(p.xy)<r)?0.:10.;  
  //theFoam=10;
  return theFoam;
  
}
vec3 beer(vec3 p){
  float a=PI/8;float b=PI/4;  
  move(p); 
  return (p.z>-1.85&&p.z<1.3)?vec3(1.,.8,0.):(p.z>0)?vec3(1.):vec3(.5);
}
vec4 hexagon(vec2 uv,float d){
  vec4 orange=vec4(1.,0.23,0.,1.); 
  vec4 black=vec4(0.02,0.02,0.02,1.); 
  float _0xF8h=0.03;  
  float _0xD8h=0.2;
  uv.x-=_0xD8h;
  uv.y-=_0xD8h/2.;
  //if(abs(uv.y-tan(PI/4.)*uv.x)<_0xF8h)return vec4(1.,0.23,0.,1.);//*
  if(abs(uv.y-tan(PI/4.)*uv.x-_0xD8h)<_0xF8h&&uv.x>=-_0xD8h&&uv.x<=-_0xD8h/2.)return black;
  if(abs(uv.y-tan(PI/4.)*uv.x+_0xD8h)<_0xF8h&&uv.x<=_0xD8h&&(uv.x)>=_0xD8h/2.)return black;
  //if(abs(uv.y-tan(PI)*uv.x)<_0xF8h)return vec4(1.,0.23,0.,1.);//*
  if(abs(uv.y-tan(PI)*uv.x-_0xD8h/2.)<_0xF8h&&uv.x>=-_0xD8h/2.&&uv.x<=_0xD8h/2.)return black;
  if(abs(uv.y-tan(PI)*uv.x+_0xD8h/2.)<_0xF8h&&uv.x>=-_0xD8h/2.&&uv.x<=_0xD8h/2.)return orange;
  //if(abs(uv.y-tan(PI-PI/4.)*uv.x)<_0xF8h)return vec4(1.,0.23,0.,1.);//*
  if(abs(uv.y-tan(PI-PI/4.)*uv.x-_0xD8h)<_0xF8h&&uv.x<=_0xD8h&&(uv.x)>=_0xD8h/2.)return black;
  if(abs(uv.y-tan(PI-PI/4.)*uv.x+_0xD8h)<_0xF8h&&uv.x>=-_0xD8h&&uv.x<=-_0xD8h/2.)return black;
  return orange;
}
vec4 hexen(vec2 uv,float d){
  float _0xD8h=0.2;
  uv*=rot(PI/2.);
  uv=mod(uv,vec2(2*_0xD8h,_0xD8h));
  return hexagon(uv,d);
}
vec3 grass(vec2 uv){
  return (abs(uv.x)<-(tan(PI/12.))*(uv.y)+0.5)?(true==true)?(sin(10*(uv.y+mod(fGlobalTime,1000)))<0)?vec3(0.,.4,0.):vec3(.0,.3,.0):vec3(.3,.3,.3):vec3(0);
}
vec3 road(vec2 uv){
  return (abs(uv.x)<-(tan(PI/12.))*(uv.y)+0.5)?(abs(uv.x)<-0.1*sin(PI/12.)*(uv.y-0.6))?(sin(10*(uv.y+mod(fGlobalTime,1000)))<0)?vec3(1.):vec3(.3,.3,.3):vec3(.3,.3,.3):vec3(0);
}
vec3 fiveline(vec2 uv){
  vec3 line=vec3(0.);
  if(abs(uv.x)<-(tan(PI/10.))*(uv.y)+0.6){
     line=vec3(.3,.3,.3);
      if(sin(3*(uv.y+mod(fGlobalTime,1000)))<0.8){
        if(abs(uv.x)<-0.1*sin(PI/12.)*(uv.y-0.6)){
          line=vec3(1.);
        }
      if((((uv.x+0.1)<-tan(PI/20.)*(uv.y)+0.33)&&
      ((uv.x+0.1)>-tan(PI/30.)*(uv.y)+0.3)  
      ||
      (((uv.x-0.1)>tan(PI/20.)*(uv.y)-0.33))
      &&
      ((uv.x-0.1)<tan(PI/30.)*(uv.y)-0.3))
      ){
        line=vec3(1.);
      }
      if((((uv.x+0.12)<-tan(PI/13.)*(uv.y)+0.53)&&
      ((uv.x+0.12)>-tan(PI/16.)*(uv.y)+0.5)  
      ||
      (((uv.x-0.12)>tan(PI/13.)*(uv.y)-0.53))
      &&
      ((uv.x-0.12)<tan(PI/16.)*(uv.y)-0.5))
      ){
        line=vec3(1.);
      }
    }    
  }else{
    line=vec3(0);
  }
  return line;
}
float tayl(vec3 p){
  return min(foam(p),min(pint(p),ball(p,1))); 
  //return min(foam(p),min(pint(p),10)); 

}
void main(void)
{
	vec2 uv = vec2(gl_FragCoord.x / v2Resolution.x, gl_FragCoord.y / v2Resolution.y);
	uv -= 0.5;
	uv /= vec2(v2Resolution.y / v2Resolution.x, 1);
  float tail=0;
  vec3 o=vec3(0,0,0),
  cam=vec3(0,10,5),
  dist=o-cam,
  fwd=normalize(dist),
  right=normalize(cross(vec3(0,1,0),fwd)),
  up=normalize(cross(fwd,right)),
  dp=vec3(fwd+uv.x*right+uv.y*up),
  p=cam+dp*0.5,
  color=fiveline(uv);
  //color=road(uv);
  //color=grass(uv);
  int i=0;
  bool gotit=false;
  for(;i<32;i++)
  {
    tail=tayl(p);
    if(tail<0.001){
      gotit=true;
      break;
    }
    p+=tail*dp;
  }
  if(true==gotit){
      if(thePint==tail||theFoam==tail){
        if(thePint==tail){
          color=beer(p);
        }
      else 
        color=vec3(1.,1.,1.0);
        color*=vec3(1-i/32.)*0.85;
    }else
    {
      color=vec3(0.,0.,0.);
      vec3 p2=p;
      p2.xz*=rot(PI);//artificial rear shadow
      vec3 n=ball_n(p2);
      p.x+=rollSize*sin(rollTime);
      p.yz*=-rot(-rollTime);
      p.xy*=rot(-rollTime);
      vec4 col=hexen(vec2(acos(p.z),atan(p.y/p.x)),2.23);
      color=vec3(1.*(col.x),0.23*(col.y),0.*(col.z));
      color*=(1-i/32.)*(.5+exp(abs(2*sin(rollTime+PI/1.5))-1)/2.)*dot(cam,n);
    }
 	}
  out_color = vec4(color,1);
//out_color=hexen(uv,2.23);
  }
