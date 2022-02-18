/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal string File_Read(c08 *FileName, u64 Length, u64 Offset);

typedef struct assetpack_header {
    u32 TagRegistryCount;
    u32 AssetCount;
    u32 AtlasCount;
} assetpack_header;

typedef struct assetpack_tag_registry {
    
} assetpack_tag_registry;

typedef struct assetpack {
    
} assetpack;

/* Asset Pack

Each asset is described by several tags. In the file format,
each tag has a "registry", a sorted list of values, and each value
points to all of the assets that are described by it.

[File Format]
- assetpack_header
- assetpack_tag_registry[TagTypeCount]
  - assetpack_tag[TagCount[Type]]
    - assetpack_asset[AssetCount[Tag]]
- assetpack_asset[AssetCount]
- assetpack_atlas[AtlasCount]
*/