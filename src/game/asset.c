/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal s32
Glyph_IsUnicodeControl(u32 Codepoint)
{
    return (Codepoint < 32) || (Codepoint > 127 && Codepoint < 160);
}

// TODO: Bin packer

#if 0
internal b08
MSDF_Init(msdf *MSDF)
{
    /*
        msdfgl_context_t context;
        context = msdfgl_create_context(NULL);  // NULL defaults to "330 core"
        
        // Enable auto-generating undefined glyphs as they are encountered for the first time.
        msdfgl_set_missing_glyph_callback(context, msdfgl_generate_glyph, NULL);
    */
    
    b08 Result = TRUE;
    
    GL_GetIntegerv(GL_MAX_TEXTURE_SIZE, &MSDF->MaxTextureSize);
    
    MSDF->GenProgramID =    File_ReadShaders("shaders/gen_msdf.vert",    "shaders/gen_msdf.frag");
    MSDF->RenderProgramID = File_ReadShaders("shaders/render_msdf.vert", "shaders/render_msdf.frag");
    
    // Uniforms
    
    
    
    GL_GenVertexArrays(1, &MSDF->VAOID);
    GL_GenBuffers(1, &MSDF->VBOID);
    GL_BindBuffer(GL_ARRAY_BUFFER, &MSDF->VBOID);
    GL_BufferData(GL_ARRAY_BUFFER, 12 * sizeof(r32), NULL, GL_STREAM_READ);
    GL_BindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    
    GL_GenBuffers(1, &MSDF->EBOID);
    GL_GenTextures(1, &MSDF->IndexTexID);
    GL_GenTextures(1, &MSDF->AtlasTexID);
    GL_GenFramebuffers(1, &MSDF->FramebufferID);
    
    return Result;
}

internal void
MSDF_LoadFont(msdf *MSDF)
{
    UNUSED(MSDF);
    /*
        msdfgl_font_t font;
        font = msdfgl_load_font(context, "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
                                4.0, 2.0, NULL); // range, scale, atlas (NULL creates a new one)
    */
    
}

internal void
MSDF_GenGlyph(u32 Codepoint)
{
}

internal void
MSDF_Print(msdf *MSDF)
{
    UNUSED(MSDF);
    /*
        //            x    y           size  color       4x4 projection-matrix  flags
        msdfgl_printf(0.0, 20.0, font, 18.0, 0xffffffff, (GLfloat *)projection, MSDFGL_KERNING,
                      "Hello, MSDFGL v%s!", MSDFGL_VERSION);
    */
    
}

internal void
MSDF_Free(msdf *MSDF)
{
    /*
        msdfgl_destroy_font(font);
    */
    
    // Delete font
    
    Heap_Free(MSDF->Atlases);
    
    GL_DeleteFramebuffers(1, &MSDF->FramebufferID);
    
    GL_DeleteTextures(1, &MSDF->AtlasTexID);
    GL_DeleteTextures(1, &MSDF->IndexTexID);
    
    GL_DeleteBuffers(1, &MSDF->EBOID);
    GL_DeleteBuffers(1, &MSDF->VBOID);
    GL_DeleteVertexArrays(1, &MSDF->VAOID);
    
    GL_DeleteProgram(&MSDF->GenProgramID);
    GL_DeleteProgram(&MSDF->RenderProgramID);
}
#endif

//CREDIT: https://github.com/Chlumsky/msdfgen/blob/master/
//CREDIT: stb TrueType Library

// internal ttf *
// Font_ReadTTF(str FileText)
// {
//     ttf *TTF = (ttf*)FileText;
//     mem Cursor = (mem)FileText;
    
//     Mem_Cpy(&TTF->OffsetTable, Cursor, sizeof(ttf_offset_table));
//     Cursor += sizeof(ttf_offset_table);
    
//     TTF->DirTableEntries = (ttf_dir_table_entry*)Cursor;
//     Cursor += TTF->OffsetTable.NumTables * sizeof(ttf_dir_table_entry);
    
//     for(u16 Index = 0;
//         Index < TTF->OffsetTable.NumTables;
//         ++Index)
//     {
//         switch(TTF->DirTableEntries[Index].Tag)
//         {
//             case 'cmap':
//                 TTF->cmap = (ttf_cmap*)(FileText + TTF->DirTableEntries[Index].Offset);
//                 TTF->cmap->Tables = 
//                 break;
//             case 'glyf':
//                 TTF->glyf = (ttf_glyf*)(FileText + TTF->DirTableEntries[Index].Offset);
//                 break;
//             case 'head':
//                 TTF->head = (ttf_head*)(FileText + TTF->DirTableEntries[Index].Offset);
//                 break;
//         }
//     }
    
//     return TTF;
// }