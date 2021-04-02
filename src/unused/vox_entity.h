/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Andrew Seiler                                       * 
 *                                                                         * 
 *  This program is free software: you can redistribute it and/or modify   * 
 *  it under the terms of the GNU General Public License as published by   * 
 *  the Free Software Foundation, either version 3 of the License, or      * 
 *  (at your option) any later version.                                    * 
 *                                                                         * 
 *  This program is distributed in the hope that it will be useful,        * 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         * 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           * 
 *  GNU General Public License for more details.                           * 
 *                                                                         * 
 *  You should have received a copy of the GNU General Public License      * 
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.  * 
 *                                                                         * 
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if !defined(GAME_VOX_ENTITY_H_)

typedef struct player
{
    r32 Yaw;
    r32 Pitch;
    r32 Speed;
    v3r32 Pos;
} player;

internal void
UpdatePlayer(player *Player,
             m4x4r32 *ViewMatrix,
             v2s32 DeltaMouse,
             r32 Sensitivity,
             game_input *Input,
             game_config *Config)
{
    //TODO: DeltaTime?
    
    Player->Yaw += (r32)DeltaMouse.X * Sensitivity;
    Player->Pitch += (r32)DeltaMouse.Y * Sensitivity;
    
    if(Player->Pitch > PI / 2.0f)
        Player->Pitch = PI / 2.0f;
    if(Player->Pitch < -PI / 2.0f)
        Player->Pitch = -PI / 2.0f;
    
    v3r32 Direction;
    Direction.E[0] = Cos(Player->Pitch) * Sin(Player->Yaw);
    Direction.E[1] = Sin(Player->Pitch);
    Direction.E[2] = Cos(Player->Pitch) * Cos(Player->Yaw);
    
    v3r32 Front;
    Front.E[0] = Sin(Player->Yaw);
    Front.E[1] = 0;
    Front.E[2] = Cos(Player->Yaw);
    
    v3r32 Right;
    Right.E[0] = Sin(Player->Yaw - ((r32)PI / 2.0f));
    Right.E[1] = 0;
    Right.E[2] = Cos(Player->Yaw - ((r32)PI / 2.0f));
    
    v3r32 Up = V3r32_Cross(Right, Front);
    
    v3r32 ToMove = {0};
    
    //TODO: Config for this
    if(Input->Keys[Config->MoveForward])
    {
        ToMove = V3r32_Add(ToMove, Front);
    }
    if(Input->Keys[Config->MoveBack])
    {
        ToMove = V3r32_Sub(ToMove, Front);
    }
    if(Input->Keys[Config->MoveRight])
    {
        ToMove = V3r32_Add(ToMove, Right);
    }
    if(Input->Keys[Config->MoveLeft])
    {
        ToMove = V3r32_Sub(ToMove, Right);
    }
    if(Input->Keys[Config->MoveUp])
    {
        ToMove = V3r32_Add(ToMove, Up);
    }
    if(Input->Keys[Config->MoveDown])
    {
        ToMove = V3r32_Sub(ToMove, Up);
    }
    
    Player->Pos = V3r32_Add(Player->Pos, V3r32_MulS(V3r32_Normalize(ToMove), Player->Speed));
    
    *ViewMatrix = M4x4r32_LookAt(Player->Pos, V3r32_Add(Player->Pos, Direction), V3r32_Cross(Right, Direction));
}

#define GAME_VOX_ENTITY_H_
#endif