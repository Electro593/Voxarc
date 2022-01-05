/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_UI_H_
#define GAME_UI_H_

#define GAME__UI__EXPORTS \

#define GAME__UI__FUNCS \
    FUNC(void, UI_CreateStringObject, heap *Heap, mesh *Mesh, mesh_object *OutObject, asset_pack *AssetPack, str String, v3r32 Pos, r32 LetterHeight) \

#endif