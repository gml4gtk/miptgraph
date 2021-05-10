if %1==release goto RELEASE_BUILD

if exist ..\..\bin\libxml2.dll goto libxml2_OK

copy ..\..\libxml\bin\libxml2.dll ..\..\bin
echo libxml2 was copied...

:libxml2_OK

if exist ..\..\bin\iconv.dll goto iconv_OK

copy ..\..\libxml\bin\iconv.dll ..\..\bin
echo iconv.dll was copied...

:iconv_OK

if exist ..\..\bin\zlib1.dll goto zlib1_OK
 
copy ..\..\libxml\bin\zlib1.dll ..\..\bin
echo zlib1.dll was copied...

:zlib1_OK

if exist ..\..\bin\test_graph.xml goto testGraph_OK

copy test_graph.xml ..\..\bin
echo test_graph.xml was copied...

:testGraph_OK:

echo All libs are OK!

goto :EOF


:RELEASE_BUILD

if exist ..\..\release\bin\libxml2.dll goto libxml2_OK

copy ..\..\libxml\bin\libxml2.dll ..\..\release\bin
echo libxml2 was copied...

:libxml2_OK

if exist ..\..\release\bin\iconv.dll goto iconv_OK

copy ..\..\libxml\bin\iconv.dll ..\..\release\bin
echo iconv.dll was copied...

:iconv_OK

if exist ..\..\release\bin\zlib1.dll goto zlib1_OK
 
copy ..\..\libxml\bin\zlib1.dll ..\..\release\bin
echo zlib1.dll was copied...

:zlib1_OK

if exist ..\..\release\bin\test_graph.xml goto testGraph_OK

copy test_graph.xml ..\..\release\bin
echo test_graph.xml was copied...

:testGraph_OK:

echo All libs are OK!
