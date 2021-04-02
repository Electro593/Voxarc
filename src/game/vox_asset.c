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

//CREDIT: https://github.com/Chlumsky/msdfgen/blob/master/
//CREDIT: stb TrueType Library

internal asset_ttf_data
Font_ParseTTF(heap *Heap,
              str FileText)
{
    asset_ttf_data Data = {0};
    mem FileCursor = FileText;
    
    ttf_header Header = *(ttf_header*)FileCursor;
    FileCursor += sizeof(ttf_header);
    ASSERT(Header.OffsetTable.SFNTVersion.Major == 1);
    
    for(u32 DirTableEntryIndex = 0;
        DirTableEntryIndex < Header.OffsetTable.NumTables;
        ++DirTableEntryIndex)
    {
        ttf_dir_table_entry *Entry = FileCursor;
        FileCursor += sizeof(ttf_dir_table_entry);
        
        if(Mem_Cmp(Entry->Tag, "head") == 0)
        {
            Data.HEADOffset = Entry->Offset;
        }
        else if(Mem_Cmp(Entry->Tag, "cmap") == 0)
        {
            Data.CMAPOffset = Entry->Offset;
        }
        else if(Mem_Cmp(Entry->Tag, "maxp") == 0)
        {
            Data.MAXPOffset = Entry->Offset;
        }
        else if(Mem_Cmp(Entry->Tag, "glyf") == 0)
        {
            Data.GLYFOffset = Entry->Offset;
        }
    }
    
    if(Data.MAXPOffset)
    {
        Data.GlyphCount = ((ttf_maxp*)(FileText + Data.MAXPOffset))->NumGlyphs;
    }
    
    ttf_cmap *CMAP = (ttf_cmap*)(FileText + Data.CMAPOffset);
    for(u32 TableIndex = 0;
        TableIndex < CMAP->EncodingTableCount;
        ++TableIndex)
    {
        ttf_cmap_subtable_header *Header = CMAP->Tables + TableIndex;
        
        switch(Header->PlatformID)
        {
            case TTFPlatformID_Windows:
            {
                switch(Header->EncodingID)
                {
                    case TTFEncodingID_Windows_UnicodeBMP:
                    case TTFEncodingID_Windows_UnicodeFull:
                    {
                        Data.CMAPSubtableOffset = Header->SubtableOffset;
                    } break;
                }
            } break;
            
            case TTFPlatformID_Unicode:
            {
                Data.CMAPSubtableOffset = Header->SubtableOffset;
            } break;
        }
    }
    ASSERT(Data.CMAPSubtableOffset != 0);
    
    Data.LocFormatIndex = ((ttf_head*)(FileText + Data.HEADOffset))->LocFormatIndex;
    
    return Data;
}