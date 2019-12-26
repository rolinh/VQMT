//
// Copyright(c) Multimedia Signal Processing Group (MMSPG),
//              Ecole Polytechnique Fédérale de Lausanne (EPFL)
//              http://mmspg.epfl.ch
// All rights reserved.
// Author: Philippe Hanhart (philippe.hanhart@epfl.ch)
//
// Permission is hereby granted, without written agreement and without
// license or royalty fees, to use, copy, modify, and distribute the
// software provided and its documentation for research purpose only,
// provided that this copyright notice and the original authors' names
// appear on all copies and supporting documentation.
// The software provided may not be commercially distributed.
// In no event shall the Ecole Polytechnique Fédérale de Lausanne (EPFL)
// be liable to any party for direct, indirect, special, incidental, or
// consequential damages arising out of the use of the software and its
// documentation.
// The Ecole Polytechnique Fédérale de Lausanne (EPFL) specifically
// disclaims any warranties.
// The software provided hereunder is on an "as is" basis and the Ecole
// Polytechnique Fédérale de Lausanne (EPFL) has no obligation to provide
// maintenance, support, updates, enhancements, or modifications.
//

#ifndef VideoYUV_hpp
#define VideoYUV_hpp

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <opencv2/core/core.hpp>

// _WIN32 is also defined in WIN64 environment (why on earth? => backward
// compatibility the argue). Added this note to remember that this actually
// includes 64-bit versions of Windows
#ifdef _WIN32
#include <io.h>
#else /* Linux, *BSD, ... */
#include <unistd.h>
#endif /* _WIN32 */

#ifdef __linux__
#include <sys/io.h>
#endif /* __linux__ */

#ifndef _WIN32
// compatibility with UNIX systems as, unlike un Windows, there is no
// difference between text and binary files: they are just sequence of bytes...
#define O_BINARY 0
#endif /* _WIN32 */

typedef unsigned char imgpel;

// Chroma subsampling format definitions
enum ChromaSubsampling {
	CHROMA_SUBSAMP_400 = 0,
	CHROMA_SUBSAMP_420 = 1,
	CHROMA_SUBSAMP_422 = 2,
	CHROMA_SUBSAMP_444 = 3
};

class VideoYUV {
public:
	VideoYUV(const char *file, int height, int width, int nbframes, int chroma_format);
	~VideoYUV();
	// Read one frame
	bool readOneFrame();
	// Get the luma component
	// readOneFrame() needs to be called before getLuma()
	void getLuma(cv::Mat& luma, int type = CV_8UC1);
private:
	FILE* file;		// file stream
	int nbframes;		// number of frames
	int height;		// height
	int width;		// width
	int comp_height[3];	// height in specific component
	int comp_width[3];	// width in specific component

	int size;		// number of samples
	int comp_size[3];	// number of samples in specific component

	imgpel *data;		// data array
	imgpel *luma;		// pointer to luma
	imgpel *chroma[2];	// pointers to chroma
};

#endif
