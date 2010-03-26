#ifndef _GIL_IO_UNIT_TEST_PATHS_HPP_
#define _GIL_IO_UNIT_TEST_PATHS_HPP_

static const std::string base = "../../../";

static const std::string bmp_in  = base + "test_images/bmp/";
static const std::string bmp_out = base + "output/bmp/";

static const std::string jpeg_in  = base + "test_images/jpg/";
static const std::string jpeg_out = base + "output/jpeg/";

static const std::string png_in  = base + "test_images/png/PngSuite/";
static const std::string png_out = base + "output/png/";

static const std::string pnm_in  = base + "test_images/pnm/";
static const std::string pnm_out = base + "output/pnm/";

static const std::string tiff_in    = base + "test_images/tiff/";
static const std::string tiff_out   = base + "output/tiff/";
static const std::string tiff_in_GM = tiff_in + "GraphicsMagick/tiff-sample-images/";

static const std::string bmp_filename ( bmp_in  + "rgb.bmp"                );
static const std::string jpeg_filename( jpeg_in + "found online/test.jpg" );
static const std::string png_filename ( base    + "test_images/png/wikipedia/test.png"    );
static const std::string pnm_filename ( pnm_in  + "rgb.pnm"                );
static const std::string tiff_filename( tiff_in + "test.tif"               );

#endif // _GIL_IO_UNIT_TEST_PATHS_HPP_
