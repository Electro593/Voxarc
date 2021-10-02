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

//CREDIT: https://github.com/Chlumsky/msdfgen/blob/master/
//CREDIT: stb TrueType Library

internal ttf *
Font_ReadTTF(str FileText)
{
    ttf *TTF = (ttf*)FileText;
    mem Cursor = (mem)FileText;
    
    Mem_Cpy(&TTF->OffsetTable, Cursor, sizeof(ttf_offset_table));
    Cursor += sizeof(ttf_offset_table);
    
    TTF->DirTableEntries = (ttf_dir_table_entry*)Cursor;
    Cursor += TTF->OffsetTable.NumTables * sizeof(ttf_dir_table_entry);
    
    for(u16 Index = 0;
        Index < TTF->OffsetTable.NumTables;
        ++Index)
    {
        switch(TTF->DirTableEntries[Index].Tag)
        {
            case 'cmap':
                TTF->cmap = (ttf_cmap*)(FileText + TTF->DirTableEntries[Index].Offset);
                TTF->cmap->Tables = 
                break;
            case 'glyf':
                TTF->glyf = (ttf_glyf*)(FileText + TTF->DirTableEntries[Index].Offset);
                break;
            case 'head':
                TTF->head = (ttf_head*)(FileText + TTF->DirTableEntries[Index].Offset);
                break;
        }
    }
    
    return TTF;
}