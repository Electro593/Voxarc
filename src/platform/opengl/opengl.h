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

#ifndef PLATFORM_OPENGL_OPENGL_H_
#define PLATFORM_OPENGL_OPENGL_H_

#define PLATFORM_OPENGL__OPENGL_DEBUG__EXPORTS \
    FUNC(void _API_ENTRY, GL_DebugCallback, u32 Source, u32 Type, u32 ID, u32 Severity, s32 Length, chr *Message, vptr UserParam) \

#define PLATFORM_OPENGL__OPENGL_DEBUG__FUNCS \

#define PLATFORM_OPENGL__OPENGL__EXPORTS \

#define PLATFORM_OPENGL__OPENGL__FUNCS \

#define PLATFORM__GL__EXPORTS \
    PLATFORM_OPENGL__OPENGL_DEBUG__EXPORTS \
    PLATFORM_OPENGL__OPENGL__EXPORTS \

#define PLATFORM__GL__FUNCS \
    PLATFORM_OPENGL__OPENGL_DEBUG__FUNCS \
    PLATFORM_OPENGL__OPENGL__FUNCS \

#endif