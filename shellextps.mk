
shellextps.dll: dlldata.obj shellext_p.obj shellext_i.obj
	link /dll /out:shellextps.dll /def:shellextps.def /entry:DllMain dlldata.obj shellext_p.obj shellext_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del shellextps.dll
	@del shellextps.lib
	@del shellextps.exp
	@del dlldata.obj
	@del shellext_p.obj
	@del shellext_i.obj
