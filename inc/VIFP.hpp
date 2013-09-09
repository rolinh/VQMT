//
// Copyright(c) Multimedia Signal Processing Group (MMSPG),
//              Ecole Polytechnique Fédérale de Lausanne (EPFL)
//              http://mmspg.epfl.ch
//              Laboratory for Image and Video Engineering (LIVE),
//              The University of Texas at Austin
//              http://live.ece.utexas.edu
// All rights reserved.
// Author: Philippe Hanhart (philippe.hanhart@epfl.ch)
//
// Permission is hereby granted, without written agreement and without license or royalty fees,
// to use, copy, modify, and distribute this code (the source files) and its documentation for
// research purpose only, provided that the copyright notice and the original authors' names in
// its entirety appear in all copies of this code, and the original source of this code is 
// acknowledged in any publication that reports research using this code.
// The research is to be cited in the bibliography as:
//
// H.R. Sheikh and A.C. Bovik, "Image information and visual quality," IEEE Transactions on
// Image Processing, vol. 15, no. 2, pp. 430-444, February 2006.
//
// The software provided may not be commercially distributed.
// In no event shall the Ecole Polytechnique Fédérale de Lausanne (EPFL) be liable to any party
// for direct, indirect, special, incidental, or consequential damages arising out of the use of
// the software and its documentation.
// The Ecole Polytechnique Fédérale de Lausanne (EPFL) specifically disclaims any warranties.
// The software provided hereunder is on an "as is" basis and the Ecole Polytechnique Fédérale de
// Lausanne (EPFL) has no obligation to provide maintenance, support, updates, enhancements, or
// modifications.
//
// In no event shall the University of Texas at Austin be liable to any party for direct, 
// indirect, special, incidental, or consequential damages arising out of the use of this 
// database and its documentation, even if the University of Texas at austin has been advised
// of the possibility of such damage.
// The University of Texas at Austin specifically disclaims any warranties, including, but not
// limited to, the implied warranties of merchantability and fitness for a particular purpose.
// the database provided hereunder is on an "as is" basis, and the University of Texas at Austin
// has no obligation to provide maintenance, support, updates, enhancements, or modifications.
//

//
// This is an OpenCV implementation of the original Matlab implementation from H.R. Sheikh
// available from http://live.ece.utexas.edu/research/quality/.
// Please refer to the following paper:
// - H.R. Sheikh and A.C. Bovik, "Image information and visual quality," IEEE Transactions on
//   Image Processing, vol. 15, no. 2, pp. 430-444, February 2006.
//

/**************************************************************************

 Calculation of the Visual Information Fidelity, pixel domain version, (VIFp)
 image quality measure.

**************************************************************************/

#ifndef VIFP_hpp
#define VIFP_hpp

#include "Metric.hpp"

class VIFP : protected Metric {
public:
	VIFP(int height, int width);
	// Compute the VIFp index of the processed image
	float compute(const cv::Mat& original, const cv::Mat& processed);
private:
	static const int NLEVS = 4;
	static const float SIGMA_NSQ;
	// Compute the coefficients of the VIFp index at a particular subband
	void computeVIFP(const cv::Mat& ref, const cv::Mat& dist, int N, double& num, double& den);
};

#endif
