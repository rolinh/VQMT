//
// Copyright(c) Multimedia Signal Processing Group (MMSPG),
//              Ecole Polytechnique Fédérale de Lausanne (EPFL)
//              http://mmspg.epfl.ch
//              Nikolay Ponomarenko
//              http://ponomarenko.info
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
// or the authors be liable to any party for direct, indirect, special,
// incidental, or consequential damages arising out of the use of the
// software and its documentation.
// The Ecole Polytechnique Fédérale de Lausanne (EPFL) and the authors
// specifically disclaims any warranties.
// The software provided hereunder is on an "as is" basis and the Ecole
// Polytechnique Fédérale de Lausanne (EPFL) and the authors have no
// obligation to provide maintenance, support, updates, enhancements,
// or modifications.
//

//
// This is an OpenCV implementation of the original Matlab implementation
// from Nikolay Ponomarenko available from http://www.ponomarenko.info/psnrhvsm.htm.
// Please refer to the following papers:
// - K. Egiazarian, J. Astola, N. Ponomarenko, V. Lukin, F. Battisti, and
//   M. Carli, "New full-reference quality metrics based on HVS," in
//   Proceedings of the Second International Workshop on Video Processing
//   and Quality Metrics, 2006.
// - N. Ponomarenko, F. Silvestri, K. Egiazarian, M. Carli, J. Astola, and
//   V. Lukin, "On between-coefficient contrast masking of DCT basis
//   functions," in Proceedings of the Third International Workshop on Video
//   Processing and Quality Metrics for Consumer Electronics, January 2007.
//

/**************************************************************************

 Calculation of the PSNR-HVS-M and PSNR-HVS image quality measures.

 PSNR-HVS-M is Peak Signal-to-Noise Ratio taking into account Contrast
 Sensitivity Function (CSF) and between-coefficient contrast masking of
 DCT basis functions PSNR-HVS is Peak Signal-to-Noise Ratio taking into
 account only CSF.

**************************************************************************/

#ifndef PSNRHVS_hpp
#define PSNRHVS_hpp

#include "Metric.hpp"

class PSNRHVS : protected Metric {
public:
	PSNRHVS(int height, int width);
	// Compute the PSNR-HVS-M and PSNR-HVS indexes of the processed image
	// Return the PSNR-HVS-M index
	float compute(const cv::Mat& original, const cv::Mat& processed);
	// Return the PSNR-HVS index only
	// compute() needs to be called before getPSNRHVS()
	float getPSNRHVS();
	// Return the PSNR-HVS-M index only
	// compute() needs to be called before getPSNRHVSM()
	float getPSNRHVSM();
private:
	float psnrhvs;
	float psnrhvsm;
	static const float CSF[8][8];
	static const float MASK[8][8];
	float maskeff(const cv::Mat &z, const cv::Mat &zdct);
	float vari(const cv::Mat &z);
};

#endif
