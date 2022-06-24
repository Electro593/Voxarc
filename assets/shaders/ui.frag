#version 460

in vec4 Color;
in vec4 Position;

// uniform uvec2 ViewSize;

out vec4 FragColor;

void curveCorner(vec2 Corner, float Radius, vec2 OuterDir) {
   vec2 ViewSize = vec2(1424, 720);
   
   vec2 Pos1 = gl_FragCoord.xy;
   vec2 Pos2 = (Corner+1)/2*ViewSize;
   Pos2.x -= OuterDir.x*Radius;
   Pos2.y -= OuterDir.y*Radius;
   
   vec2 Diff = Pos1 - Pos2;
   float Len = length(Diff);
   
   uint IsOutside = 1;
   if(OuterDir.x > 0)
      IsOutside &= uint(Pos1.x > Pos2.x);
   else
      IsOutside &= uint(Pos1.x < Pos2.x);
   if(OuterDir.y > 0)
      IsOutside &= uint(Pos1.y > Pos2.y);
   else
      IsOutside &= uint(Pos1.y < Pos2.y);
   if(Len > Radius && IsOutside==1) discard;
}

void main() {
   float Radius = 80;
   curveCorner(Position.xy, Radius, vec2(-1,1));
   curveCorner(Position.xy+vec2(Position.z,0), Radius, vec2(1,1));
   curveCorner(Position.xy+Position.zw, Radius, vec2(1,-1));
   curveCorner(Position.xy+vec2(0,Position.w), Radius, vec2(-1,-1));
   
   FragColor = Color;
}