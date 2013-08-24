

/* this ALWAYS GENERATED file contains the proxy stub code */


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

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "shellext.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _shellext_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } shellext_MIDL_TYPE_FORMAT_STRING;

typedef struct _shellext_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } shellext_MIDL_PROC_FORMAT_STRING;

typedef struct _shellext_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } shellext_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const shellext_MIDL_TYPE_FORMAT_STRING shellext__MIDL_TypeFormatString;
extern const shellext_MIDL_PROC_FORMAT_STRING shellext__MIDL_ProcFormatString;
extern const shellext_MIDL_EXPR_FORMAT_STRING shellext__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPSCopyShellExt_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPSCopyShellExt_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPSDeleteShellExt_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPSDeleteShellExt_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const shellext_MIDL_PROC_FORMAT_STRING shellext__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const shellext_MIDL_TYPE_FORMAT_STRING shellext__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IPSCopyShellExt, ver. 0.0,
   GUID={0xF7D80D94,0x09AE,0x4660,{0xBE,0xC1,0xC8,0xE7,0x4E,0xC0,0x36,0x1E}} */

#pragma code_seg(".orpc")
static const unsigned short IPSCopyShellExt_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IPSCopyShellExt_ProxyInfo =
    {
    &Object_StubDesc,
    shellext__MIDL_ProcFormatString.Format,
    &IPSCopyShellExt_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPSCopyShellExt_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    shellext__MIDL_ProcFormatString.Format,
    &IPSCopyShellExt_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _IPSCopyShellExtProxyVtbl = 
{
    0,
    &IID_IPSCopyShellExt,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */
};


static const PRPC_STUB_FUNCTION IPSCopyShellExt_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _IPSCopyShellExtStubVtbl =
{
    &IID_IPSCopyShellExt,
    &IPSCopyShellExt_ServerInfo,
    7,
    &IPSCopyShellExt_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPSDeleteShellExt, ver. 0.0,
   GUID={0xCC922D57,0xE93A,0x4895,{0x85,0x53,0x9C,0x52,0xC4,0x7B,0xCB,0x87}} */

#pragma code_seg(".orpc")
static const unsigned short IPSDeleteShellExt_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IPSDeleteShellExt_ProxyInfo =
    {
    &Object_StubDesc,
    shellext__MIDL_ProcFormatString.Format,
    &IPSDeleteShellExt_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPSDeleteShellExt_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    shellext__MIDL_ProcFormatString.Format,
    &IPSDeleteShellExt_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _IPSDeleteShellExtProxyVtbl = 
{
    0,
    &IID_IPSDeleteShellExt,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */
};


static const PRPC_STUB_FUNCTION IPSDeleteShellExt_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _IPSDeleteShellExtStubVtbl =
{
    &IID_IPSDeleteShellExt,
    &IPSDeleteShellExt_ServerInfo,
    7,
    &IPSDeleteShellExt_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    shellext__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x700022b, /* MIDL Version 7.0.555 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _shellext_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IPSDeleteShellExtProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPSCopyShellExtProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _shellext_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IPSDeleteShellExtStubVtbl,
    ( CInterfaceStubVtbl *) &_IPSCopyShellExtStubVtbl,
    0
};

PCInterfaceName const _shellext_InterfaceNamesList[] = 
{
    "IPSDeleteShellExt",
    "IPSCopyShellExt",
    0
};

const IID *  const _shellext_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _shellext_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _shellext, pIID, n)

int __stdcall _shellext_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _shellext, 2, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _shellext, 2, *pIndex )
    
}

const ExtendedProxyFileInfo shellext_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _shellext_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _shellext_StubVtblList,
    (const PCInterfaceName * ) & _shellext_InterfaceNamesList,
    (const IID ** ) & _shellext_BaseIIDList,
    & _shellext_IID_Lookup, 
    2,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

