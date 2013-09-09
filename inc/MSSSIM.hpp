//
// Copyright(c) Multimedia Signal Processing Group (MMSPG),
//              Ecole Polytechnique Fédérale de Lausanne (EPFL)
//              http://mmspg.epfl.ch
//              Zhou Wang
//              https://ece.uwaterloo.ca/~z70wang/
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

//
// This is an OpenCV implementation of the original Matlab implementation
// from Nikolay Ponomarenko available from http://live.ece.utexas.edu/research/quality/.
// Please refer to the following papers:
// - Z. Wang, E.P. Simoncelli, and A.C. Bovik, "Multiscale structural
//   similarity for image quality assessment," in IEEE Asilomar Conference
//   on Signals, Systems and Computers, November 2003, vol. 2, pp. 1398–1402.
//

/**************************************************************************

 Calculation of the MS-SSIM and SSIM image quality measures.

 Multi-Scale Structural Similarity (MS-SSIM) is a multi-scale extension of
 Structural Similarity (SSIM), where the SSIM index is computed at 5 levels
 and combined to form the MS-SSIM index.

**************************************************************************/

#ifndef MSSSIM_hpp
#define MSSSIM_hpp

#include "SSIM.hpp"

class MSSSIM : protected SSIM {
public:
	MSSSIM(int height, int width);
	// Compute the SSIM and MS-SSIM indexes of the processed image
	// Return the MS-SSIM index
	float compute(const cv::Mat& original, const cv::Mat& processed);
	// Return the SSIM index only
	// compute() needs to be called before getSSIM()
	float getSSIM();
	// Return the MS-SSIM index only
	// compute() needs to be called before getMSSSIM()
	float getMSSSIM();
private:
	double ssim;
	double msssim;
	static const int NLEVS = 5;
	static const double WEIGHT[];
};

#endif
