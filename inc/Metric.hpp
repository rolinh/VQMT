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

#ifndef Metric_hpp
#define Metric_hpp

#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Metric {
public:
	Metric(int height, int width);
	virtual ~Metric();
	virtual float compute(const cv::Mat& original, const cv::Mat& processed) = 0;
protected:
	int height;
	int width;
	// Smoothing using a Gaussian kernel of size ksize with standard deviation sigma
	// Returns only those parts of the correlation that are computed without zero-padded edges
	// (similarly to 'filter2' in Matlab with option 'valid')
	void applyGaussianBlur(const cv::Mat& src, cv::Mat& dst, int ksize, double sigma);
};

#endif
