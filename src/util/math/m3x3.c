/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Aria Seiler                                       * 
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


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: R32 (m3x3r32)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal m3x3r32
M3x3r32_1(r32 N)
{
    m3x3r32 Result = {0};
    
    Result.M[0][0] = N;
    Result.M[1][1] = N;
    Result.M[2][2] = N;
    
    return Result;
}

internal m3x3r32
M3x3r32_3_3_3(v3r32 Col0,
              v3r32 Col1,
              v3r32 Col2)
{
    m3x3r32 Result;
    
    Result.V[0] = Col0;
    Result.V[1] = Col1;
    Result.V[2] = Col2;
    
    return Result;
}

internal v3r32
M3x3r32_Mul(m3x3r32 M,
            v3r32   V)
{
    v3r32 Result;
    
    Result.E[0] = (V.E[0] * M.M[0][0]) + (V.E[1] * M.M[1][0]) + (V.E[2] * M.M[2][0]);
    Result.E[1] = (V.E[0] * M.M[0][1]) + (V.E[1] * M.M[1][1]) + (V.E[2] * M.M[2][1]);
    Result.E[2] = (V.E[0] * M.M[0][2]) + (V.E[1] * M.M[1][2]) + (V.E[2] * M.M[2][2]);
    
    return Result;
};

// internal m3x3r32
// M3x3r32_Inv(m3x3r32 M)
// {
//     r32 C = 1 / (M.M[0][0]*(M.M[1][1]*M.M[2][2] - M.M[2][1]*M.M[1][2]) -
//                  M.M[1][0]*(M.M[0][1]*M.M[2][2] - M.M[2][1]*M.M[0][2]) +
//                  M.M[2][0]*(M.M[0][1]*M.M[1][2] - M.M[1][1]*M.M[0][2]));
//     m3x3r32 Result;
//     Result.M[0][0] = +(M.M[1][1]*M.M[2][2] - M.M[2][1]*M.M[1][2]) * C;
//     Result.M[1][0] = -(M.M[1][0]*M.M[2][2] - M.M[2][0]*M.M[1][2]) * C;
//     Result.M[2][0] = +(M.M[1][0]*M.M[2][1] - M.M[2][0]*M.M[1][1]) * C;
//     Result.M[0][1] = -(M.M[0][1]*M.M[2][2] - M.M[2][1]*M.M[0][2]) * C;
//     Result.M[1][1] = +(M.M[0][0]*M.M[2][2] - M.M[2][0]*M.M[0][2]) * C;
//     Result.M[2][1] = -(M.M[0][0]*M.M[2][1] - M.M[2][0]*M.M[0][1]) * C;
//     Result.M[0][2] = +(M.M[0][1]*M.M[1][2] - M.M[1][1]*M.M[0][2]) * C;
//     Result.M[1][2] = -(M.M[0][0]*M.M[1][2] - M.M[1][0]*M.M[0][2]) * C;
//     Result.M[2][2] = +(M.M[0][0]*M.M[1][1] - M.M[1][0]*M.M[0][1]) * C;
//     return Result;
// }