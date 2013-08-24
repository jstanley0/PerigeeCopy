

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __shellext_h__
#define __shellext_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPSCopyShellExt_FWD_DEFINED__
#define __IPSCopyShellExt_FWD_DEFINED__
typedef interface IPSCopyShellExt IPSCopyShellExt;
#endif 	/* __IPSCopyShellExt_FWD_DEFINED__ */


#ifndef __IPSDeleteShellExt_FWD_DEFINED__
#define __IPSDeleteShellExt_FWD_DEFINED__
typedef interface IPSDeleteShellExt IPSDeleteShellExt;
#endif 	/* __IPSDeleteShellExt_FWD_DEFINED__ */


#ifndef __PSCopyShellExt_FWD_DEFINED__
#define __PSCopyShellExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class PSCopyShellExt PSCopyShellExt;
#else
typedef struct PSCopyShellExt PSCopyShellExt;
#endif /* __cplusplus */

#endif 	/* __PSCopyShellExt_FWD_DEFINED__ */


#ifndef __PSDeleteShellExt_FWD_DEFINED__
#define __PSDeleteShellExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class PSDeleteShellExt PSDeleteShellExt;
#else
typedef struct PSDeleteShellExt PSDeleteShellExt;
#endif /* __cplusplus */

#endif 	/* __PSDeleteShellExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IPSCopyShellExt_INTERFACE_DEFINED__
#define __IPSCopyShellExt_INTERFACE_DEFINED__

/* interface IPSCopyShellExt */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPSCopyShellExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F7D80D94-09AE-4660-BEC1-C8E74EC0361E")
    IPSCopyShellExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IPSCopyShellExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPSCopyShellExt * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPSCopyShellExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPSCopyShellExt * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPSCopyShellExt * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPSCopyShellExt * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPSCopyShellExt * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPSCopyShellExt * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IPSCopyShellExtVtbl;

    interface IPSCopyShellExt
    {
        CONST_VTBL struct IPSCopyShellExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPSCopyShellExt_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPSCopyShellExt_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPSCopyShellExt_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPSCopyShellExt_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPSCopyShellExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPSCopyShellExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPSCopyShellExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPSCopyShellExt_INTERFACE_DEFINED__ */


#ifndef __IPSDeleteShellExt_INTERFACE_DEFINED__
#define __IPSDeleteShellExt_INTERFACE_DEFINED__

/* interface IPSDeleteShellExt */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPSDeleteShellExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CC922D57-E93A-4895-8553-9C52C47BCB87")
    IPSDeleteShellExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IPSDeleteShellExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPSDeleteShellExt * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPSDeleteShellExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPSDeleteShellExt * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPSDeleteShellExt * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPSDeleteShellExt * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPSDeleteShellExt * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPSDeleteShellExt * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IPSDeleteShellExtVtbl;

    interface IPSDeleteShellExt
    {
        CONST_VTBL struct IPSDeleteShellExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPSDeleteShellExt_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPSDeleteShellExt_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPSDeleteShellExt_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPSDeleteShellExt_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPSDeleteShellExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPSDeleteShellExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPSDeleteShellExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPSDeleteShellExt_INTERFACE_DEFINED__ */



#ifndef __SHELLEXTLib_LIBRARY_DEFINED__
#define __SHELLEXTLib_LIBRARY_DEFINED__

/* library SHELLEXTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_SHELLEXTLib;

EXTERN_C const CLSID CLSID_PSCopyShellExt;

#ifdef __cplusplus

class DECLSPEC_UUID("80FCA4C0-56BD-406B-864C-0754BB449E76")
PSCopyShellExt;
#endif

EXTERN_C const CLSID CLSID_PSDeleteShellExt;

#ifdef __cplusplus

class DECLSPEC_UUID("3650CFF3-5EE5-44B7-A5C8-5B29A375B0E7")
PSDeleteShellExt;
#endif
#endif /* __SHELLEXTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


