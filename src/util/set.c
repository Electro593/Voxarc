// /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
// **                                                                         **
// **  Author: Aria Seiler                                                    **
// **                                                                         **
// **  This program is in the public domain. There is no implied warranty,    **
// **  so use it at your own risk.                                            **
// **                                                                         **
// \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal vptr
BinarySearchArray(vptr *Array, u32 Start, u32 End, vptr Target, type Type,
                  u32 *IndexOut)
{
   u32 Offset = 0;
   switch(Type.ID & TYPEID_MOD_MASK) {
      case TYPEID_MEMBER: {
         Offset = (Type.ID & TYPEID_EXTRA_MASK) >> TYPEID_EXTRA_EXP;
      }
   }
   
   u32 Index = Start;
   vptr Curr = NULL;
   
   while(Start != End) {
      Index = Start+(End-Start)/2;
      Curr = (vptr)((u64)(*Array) + Offset + Type.Size*Index);
      
      b08 Cmp = EQUAL;
      switch(Type.ID & TYPEID_TYPE_MASK) {
         case TYPEID_U32: {
                 if(*(u32*)Curr < *(u32*)Target) Cmp = LESS;
            else if(*(u32*)Curr > *(u32*)Target) Cmp = GREATER;
         } break;
         
         case TYPEID_VPTR: {
                 if(*(vptr*)Curr < *(vptr*)Target) Cmp = LESS;
            else if(*(vptr*)Curr > *(vptr*)Target) Cmp = GREATER;
         } break;
         
         default: {
            Assert(FALSE, "Type not supported in BinarySearchArray!");
         }
      }
      
      if(Cmp == EQUAL) break;
      if(Cmp == LESS) Start = Index+1;
      else End = Index;
   }
   
   // If we didn't find a match, give the insertion index
   // (In case the caller was looking for a place to insert)
   if(Index < Start) Index = Start;
   
   if(IndexOut) *IndexOut = Index;
   
   if(Start == End) Curr = NULL;
   return Curr;
}