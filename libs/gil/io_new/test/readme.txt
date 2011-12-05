Step-by-step tutorial to have the test suite build using MS VC 10 on Windows 7


1. Get boost, preferably latest version or trunk
	http://svn.boost.org/svn/boost/trunk

1.1 Get bjam.exe
	
2. Build boost
		debug:
		bjam -j4 --toolset=msvc --without-mpi variant=debug define=_CRT_NONSTDC_NO_DEPRECATE define=_CRT_SECURE_NO_DEPRECATE define=_SCL_SECURE_NO_DEPRECATE stage
		
3. Get boost build extension
		http://svn.boost.org/svn/boost/sandbox/tools/build_extensions/

3.1 Apply patch to boost_build extension
		http://mateusz.loskot.net/download/patches/boost-build_extensions-r66346-update.patch
		
4. Get image libs
	jpeg: http://www.ijg.org/
	tiff: http://www.remotesensing.org/libtiff/
	zlib: http://www.zlib.net/
	png: http://www.libpng.org/pub/png/libpng.html

	- rename tiffconf.vc.h into tiffconf.h
	
5. Set environment variables
	BOOST_ROOT=C:\boost
	BOOST_BUILD=C:\boost\tools\build\v2
	BOOST_BUILD_PATH=C:\boost_build\ext

6. Get io_new
	http://gil-contributions.googlecode.com/svn/trunk/

6.1 Copy io_new into boost source tree
	C:\gil_contribution\gil_2\boost\gil\extension\*.*  to C:\boost\boost\gil\extension
	C:\gil_contribution\gil_2\libs\gil\io_new\*.*	   to C:\boost\libs\gil\io_new
	
7. Create empty jamroot file in C:\boost\libs\gil
	- needs to be removed when building gil's unit tests

8. copy bjam.exe into gil test and gil::io_new test folders
	C:\boost\libs\gil\test
	C:\boost\libs\gil\io_new\test

9. copy C:\boost\tools\build\v2\user-config.jam to user root ( C:\Users\your_user_name )
	- needs to be removed when building any boost lib unit tests

	9.1 add line user-config.jam
	use-project boost : C:/boost/ ;

10. change main path in path.hpp
	
10. run bjam in in C:\boost\libs\gil\io_new\test
	debug:
	bjam -j4 --toolset=msvc threading=multi

	release:
	bjam -j4 --toolset=msvc variant=release threading=multi define=_SECURE_SCL=0 define=_HAS_ITERATOR_DEBUGGING=0
