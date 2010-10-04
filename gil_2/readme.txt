The IO GIL ( Generic Image Library ) extension provides a user with
the abilities to read and write various image formats. Please read
the documentation first before using it. A short introduction
will be provided below.

In order to use this boost::gil extension the user has first
to resolve some dependencies that come with certain image formats.

BMP  - no dependencies
JPEG - libjpeg
PNG  - libpng
PNM  - no dependencies
TIFF - libtiff

Though, a user has to make sure when building his/her application
all header files and binaries are set up correctly to be able
to compile and link correctly.

In case a user wants to run the test suite the path.cpp header files
needs to change to reflect the user folder structure.
