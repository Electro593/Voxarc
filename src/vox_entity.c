#include "vox_entity.h"

#include "math/vox_v2s32.h"

local_func void
UpdatePlayer(player *Player, m4r *ViewMatrix, v2s DeltaMouse, r32 Sensitivity, game_input *Input)
{
    //TODO(andrew): DeltaTime?
    
    Player->Yaw += (r32)DeltaMouse.X * Sensitivity;
    Player->Pitch += (r32)DeltaMouse.Y * Sensitivity;
    
    if(Player->Pitch > PI / 2.0f)
        Player->Pitch = PI / 2.0f;
    if(Player->Pitch < -PI / 2.0f)
        Player->Pitch = -PI / 2.0f;
    
    v3r Direction = {Cos(Player->Pitch) * Sin(Player->Yaw),
                     Sin(Player->Pitch),
                     Cos(Player->Pitch) * Cos(Player->Yaw)};
    v3r Front = {Sin(Player->Yaw),
                 0,
                 Cos(Player->Yaw)};
    v3r Right = {Sin(Player->Yaw - ((r32)PI / 2.0f)),
                 0,
                 Cos(Player->Yaw - ((r32)PI / 2.0f))};
    v3r Up = Cross(Right, Front);
    
    v3r ToMove = {};
    
    //TODO(andrew): Config for this
    if(Input->W || Input->Up)
        ToMove += Front;
    if(Input->S || Input->Down)
        ToMove -= Front;
    if(Input->D || Input->Right)
        ToMove += Right;
    if(Input->A || Input->Left)
        ToMove -= Right;
    if(Input->Space)
        ToMove += Up;
    if(Input->LShift)
        ToMove -= Up;
    
    Player->Pos += Normalize(ToMove) * Player->Speed;
    
    *ViewMatrix = LookAt(Player->Pos, Player->Pos + Direction, Cross(Right, Direction));
}