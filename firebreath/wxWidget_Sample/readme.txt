Sample code for integrating wxWidgets with a Firebreath plugin
----------------------------------------------------------------

Here are some facts:

- the code has only been tested for Windows
- only Windows' cmake file has been changed
- the code is only to intended as a prototype

- before running the prep script set the following environment variables:
	* MY_WINDOWS_INSTALL_PATH, so cmake can find comctl32.lib and some more libs
	* WXWINDOWS_INSTALL_PATH, so cmake can find wxWidgets header and libs

I use the following prep command:

prep2010.cmd C:\path_to_code "-DWITH_DYNAMIC_MSVC_RUNTIME=1" "-DWITH_SYSTEM_BOOST=1" "-DBOOST_ROOT=C:\boost" "-DBOOST_LIBRARYDIR=C:\boost\stage\lib\x86" "-DBoost_USE_STATIC_LIBS=on" "-DBoost_USE_STATIC_RUNTIME=off" "" "-DWXWINDOWS_INSTALL_PATH=C:/path_to_wxwidgets" "-DMY_WINDOWS_INSTALL_PATH=C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Lib"


-------
todo
-------

* add_boost_library(chrono) doesn't work for me