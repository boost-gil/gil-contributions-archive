/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef TIFF_IO_PRIVATE_HPP
#define TIFF_IO_PRIVATE_HPP

/// \file
/// \brief  Support for reading and writing TIFF files
//
/// \author Christian Henning
///         

/// see http://www.remotesensing.org/libtiff/

#include <fstream>
#include <algorithm>
#include <string>

extern "C" {
#include "tiff.h"
}

namespace boost { namespace gil {


void read_tiff_image_info( tiff_image_read_info& info )
{
}

} // namespace gil
} // namespace boost

#endif // TIFF_IO_PRIVATE_HPP
