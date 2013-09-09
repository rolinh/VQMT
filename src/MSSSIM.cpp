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

#include "MSSSIM.hpp"

const double MSSSIM::WEIGHT[] = {0.0448, 0.2856, 0.3001, 0.2363, 0.1333};

MSSSIM::MSSSIM(int h, int w) : SSIM(h, w)
{
}

float MSSSIM::compute(const cv::Mat& original, const cv::Mat& processed)
{
	double mssim[NLEVS];
	double mcs[NLEVS];

	cv::Mat im1[NLEVS];
	cv::Mat im2[NLEVS];
	
	int w = original.cols;
	int h = original.rows;
	
	original.copyTo(im1[0]);
	processed.copyTo(im2[0]);
	
	for (int l=0; l<NLEVS; l++) {
		// [mssim_array(l) ssim_map_array{l} mcs_array(l) cs_map_array{l}] = ssim_index_new(im1, im2, K, window);
		cv::Scalar res = SSIM::computeSSIM(im1[l], im2[l]);
		mssim[l] = res.val[0];
		mcs[l] = res.val[1];

		if (l < NLEVS-1) {
			w /= 2;
			h /= 2;
			im1[l+1] = cv::Mat(h,w,CV_32F);
			im2[l+1] = cv::Mat(h,w,CV_32F);
			
			// filtered_im1 = filter2(downsample_filter, im1, 'valid');
			// im1 = filtered_im1(1:2:M-1, 1:2:N-1);
			cv::resize(im1[l], im1[l+1], cv::Size(w,h), 0, 0, cv::INTER_LINEAR);
			// filtered_im2 = filter2(downsample_filter, im2, 'valid');
			// im2 = filtered_im2(1:2:M-1, 1:2:N-1);
			cv::resize(im2[l], im2[l+1], cv::Size(w,h), 0, 0, cv::INTER_LINEAR);
		}
	}

	ssim = mssim[0];

	// overall_mssim = prod(mcs_array(1:level-1).^weight(1:level-1))*mssim_array(level);
	msssim = mssim[NLEVS-1];
	for (int l=0; l<NLEVS-1; l++)	msssim *= pow(mcs[l], WEIGHT[l]);

	return float(msssim);
}

float MSSSIM::getSSIM()
{
	return float(ssim);
}

float MSSSIM::getMSSSIM()
{
	return float(msssim);
}
