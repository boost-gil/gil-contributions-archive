The GIL ( Generic Image Library ) extension provides a user with
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

In case a user wants to run the test suite there are some more steps.
First the test suite comes with sample image that are read in various ways. 
Second the output is written into a predefined folder. To adapt the folder
structure go ahead and change the paths.hpp header file.
Also the test suite requires all of the above libraries to run appropriately.
