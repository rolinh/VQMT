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

#include "VideoYUV.hpp"

VideoYUV::VideoYUV(const char *f, int h, int w, int nbf, int chroma_format)
{
	file = open(f, O_RDONLY | O_BINARY);
	if (!file) {
		fprintf(stderr, "readOneFrame: cannot open input file (%s)\n", f);
		exit(EXIT_FAILURE);
	}
	height = h;
	width  = w;
	nbframes = nbf;

	comp_height[0] = h;
	comp_width [0] = w;
	if (chroma_format == CHROMA_SUBSAMP_400) {
		comp_height[2] = comp_height[1] = 0;
		comp_width [2] = comp_width [1] = 0;
	}
	else if (chroma_format == CHROMA_SUBSAMP_420) {
		// Check size
		if (h % 2 == 1 || w % 2 == 1) {
			fprintf(stderr, "YUV420: 'height' and 'width' have to be even numbers.\n");
			exit(EXIT_FAILURE);
		}

		comp_height[2] = comp_height[1] = h >> 1;
		comp_width [2] = comp_width [1] = w >> 1;
	}
	else if (chroma_format == CHROMA_SUBSAMP_422) {
		// Check size
		if (w % 2 == 1) {
			fprintf(stderr, "YUV422: 'width' has to be an even number.\n");
			exit(EXIT_FAILURE);
		}

		comp_height[2] = comp_height[1] = h;
		comp_width [2] = comp_width [1] = w >> 1;
	}
	else {
		comp_height[2] = comp_height[1] = h;
		comp_width [2] = comp_width [1] = w;
	}
	comp_size[0] = comp_height[0]*comp_width[0];
	comp_size[1] = comp_height[1]*comp_width[1];
	comp_size[2] = comp_height[2]*comp_width[2];
	
	size = comp_size[0]+comp_size[1]+comp_size[2];
	
	data = new imgpel[size];
	luma = data;
	chroma[0] = data+comp_size[0];
	chroma[1] = data+comp_size[0]+comp_size[1];
}

VideoYUV::~VideoYUV()
{
	delete[] data;
	close(file);
}

bool VideoYUV::readOneFrame()
{
	imgpel *ptr_data = data;

	for (int j=0; j<3; j++) {
		int read_size = comp_width[j];
		if (read_size <= 0)
			continue;
		for (int i=0; i<comp_height[j]; i++) {
			if (read(file, ptr_data, static_cast<size_t>(read_size)) != read_size) {
				fprintf(stderr, "readOneFrame: cannot read %d bytes from input file, unexpected EOF.\n", read_size);
				return false;
			}
			ptr_data += read_size;
		}
	}
	return true;
}

void VideoYUV::getLuma(cv::Mat& local_luma, int type)
{
	cv::Mat tmp(height, width, CV_8UC1, this->luma);
	if (type == CV_8UC1) {
		tmp.copyTo(local_luma);
	}
	else {
		tmp.convertTo(local_luma, type);
	}
}
