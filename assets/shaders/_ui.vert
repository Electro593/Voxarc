#version 460

layout(location = 0) in vec4 PositionIn;
layout(location = 3) in vec4 ColorIn;

out vec4 Color;
out vec4 Position;

void main() {
   uint index = uint(gl_VertexID % 4);
   
   if(index == 0)
      gl_Position = vec4(PositionIn.x, PositionIn.y+PositionIn.w, 0, 1);
   else if(index == 1)
      gl_Position = vec4(PositionIn.x, PositionIn.y, 0, 1);
   else if(index == 2)
      gl_Position = vec4(PositionIn.x+PositionIn.z, PositionIn.y, 0, 1);
   else if(index == 3)
      gl_Position = vec4(PositionIn.x+PositionIn.z, PositionIn.y+PositionIn.w, 0, 1);
   
   Position = PositionIn;
   Color = ColorIn;
}