

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sun Jul 07 20:03:49 2013
 */
/* Compiler settings for .\shellext.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IPSCopyShellExt,0xF7D80D94,0x09AE,0x4660,0xBE,0xC1,0xC8,0xE7,0x4E,0xC0,0x36,0x1E);


MIDL_DEFINE_GUID(IID, IID_IPSDeleteShellExt,0xCC922D57,0xE93A,0x4895,0x85,0x53,0x9C,0x52,0xC4,0x7B,0xCB,0x87);


MIDL_DEFINE_GUID(IID, LIBID_SHELLEXTLib,0x26B45228,0x004A,0x4774,0xB0,0x33,0xFB,0x9C,0x23,0x5C,0x60,0xE3);


MIDL_DEFINE_GUID(CLSID, CLSID_PSCopyShellExt,0x80FCA4C0,0x56BD,0x406B,0x86,0x4C,0x07,0x54,0xBB,0x44,0x9E,0x76);


MIDL_DEFINE_GUID(CLSID, CLSID_PSDeleteShellExt,0x3650CFF3,0x5EE5,0x44B7,0xA5,0xC8,0x5B,0x29,0xA3,0x75,0xB0,0xE7);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



