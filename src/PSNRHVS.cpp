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

#include <cfloat>
#include "PSNRHVS.hpp"

const float PSNRHVS::CSF[8][8]  =	{{1.608443f, 2.339554f, 2.573509f, 1.608443f, 1.072295f, 0.643377f, 0.504610f, 0.421887f},
									 {2.144591f, 2.144591f, 1.838221f, 1.354478f, 0.989811f, 0.443708f, 0.428918f, 0.467911f},
									 {1.838221f, 1.979622f, 1.608443f, 1.072295f, 0.643377f, 0.451493f, 0.372972f, 0.459555f},
									 {1.838221f, 1.513829f, 1.169777f, 0.887417f, 0.504610f, 0.295806f, 0.321689f, 0.415082f},
									 {1.429727f, 1.169777f, 0.695543f, 0.459555f, 0.378457f, 0.236102f, 0.249855f, 0.334222f},
									 {1.072295f, 0.735288f, 0.467911f, 0.402111f, 0.317717f, 0.247453f, 0.227744f, 0.279729f},
									 {0.525206f, 0.402111f, 0.329937f, 0.295806f, 0.249855f, 0.212687f, 0.214459f, 0.254803f},
									 {0.357432f, 0.279729f, 0.270896f, 0.262603f, 0.229778f, 0.257351f, 0.249855f, 0.259950f}};

const float PSNRHVS::MASK[8][8] =	{{0.390625f, 0.826446f, 1.000000f, 0.390625f, 0.173611f, 0.062500f, 0.038447f, 0.026874f},
									 {0.694444f, 0.694444f, 0.510204f, 0.277008f, 0.147929f, 0.029727f, 0.027778f, 0.033058f},
									 {0.510204f, 0.591716f, 0.390625f, 0.173611f, 0.062500f, 0.030779f, 0.021004f, 0.031888f},
									 {0.510204f, 0.346021f, 0.206612f, 0.118906f, 0.038447f, 0.013212f, 0.015625f, 0.026015f},
									 {0.308642f, 0.206612f, 0.073046f, 0.031888f, 0.021626f, 0.008417f, 0.009426f, 0.016866f},
									 {0.173611f, 0.081633f, 0.033058f, 0.024414f, 0.015242f, 0.009246f, 0.007831f, 0.011815f},
									 {0.041649f, 0.024414f, 0.016437f, 0.013212f, 0.009426f, 0.006830f, 0.006944f, 0.009803f},
									 {0.019290f, 0.011815f, 0.011080f, 0.010412f, 0.007972f, 0.010000f, 0.009426f, 0.010203f}};

PSNRHVS::PSNRHVS(int h, int w) : Metric(h, w)
{
}

float PSNRHVS::getPSNRHVS()
{
	return psnrhvs;
}

float PSNRHVS::getPSNRHVSM()
{
	return psnrhvsm;
}

float PSNRHVS::compute(const cv::Mat& original, const cv::Mat& processed)
{
	float s1 = 0.0f;
	float s2 = 0.0f;
	float num = static_cast<float>(width*height);
	float tmp;
	cv::Mat a(8,8,CV_32F), b(8,8,CV_32F), a_dct(8,8,CV_32F), b_dct(8,8,CV_32F);

	for (int y=0; y<height; y+=8) {
		for (int x=0; x<width; x+=8) {
			// a = img1(y:y+7,x:x+7);
			a = original(cv::Range(y,y+8),cv::Range(x,x+8));
			// b = img2(y:y+7,x:x+7);
			b = processed(cv::Range(y,y+8),cv::Range(x,x+8));
			// a_dct = dct2(a);
			cv::dct(a, a_dct);
			// b_dct = dct2(b);
			cv::dct(b, b_dct);

			// mask_a = maskeff(a,a_dct);
			float mask_a = maskeff(a,a_dct);
			// mask_b = maskeff(b,b_dct);
			float mask_b = maskeff(b,b_dct);

			// if mask_b > mask_a: mask_a = mask_b;
			mask_a = mask_b > mask_a ? mask_b : mask_a;

			for (int k=0; k<8; k++) {
				const float *ptr_a = a_dct.ptr<float>(k);
				const float *ptr_b = b_dct.ptr<float>(k);
				for (int l=0; l<8; l++) {
					// u = abs(a_dct(k,l)-b_dct(k,l));
					float u = std::abs(*ptr_a++ - *ptr_b++);
					// s2 = s2 + (u*CSF(k,l)).^2;
					tmp = u*CSF[k][l];
					s2 += tmp*tmp;
					// if (k~=1) | (l~=1)
					if (k != 0 || l !=0) {
						// if u < mask_a/mask(k,l)
						tmp = mask_a/MASK[k][l];
						if (u < tmp) {
							// u = 0;
							u = 0;
						}
						else {
							// u = u - mask_a/mask(k,l);
							u -= tmp;
						}
					}
					// s1 = s1 + (u*CSF(k,l)).^2;
					tmp = u*CSF[k][l];
					s1 += tmp*tmp;
				}
			}
		}
	}

	// s1 = s1/num;
	s1 /= num;
	// s2 = s2/num;
	s2 /= num;

	// if s1 == 0: p_hvs_m = 100000;
	// else: p_hvs_m = 10*log10(255*255/s1);
	psnrhvsm = s1 <= FLT_EPSILON ? 100000.0f : float(10*log10(255*255/s1));
	// if s2 == 0: p_hvs = 100000;
	// else: p_hvs = 10*log10(255*255/s2);
	psnrhvs = s2 <= FLT_EPSILON ? 100000.0f : float(10*log10(255*255/s2));

	return psnrhvsm;
}

float PSNRHVS::maskeff(const cv::Mat &z, const cv::Mat &zdct)
{
	float m = 0;

	float val;
	for (int k=0; k<8; k++) {
		const float* ptr = zdct.ptr<float>(k);
		for (int l=0; l<8; l++) {
			val = *ptr++;
			// if (k~=1) | (l~=1): m = m + (zdct(k,l).^2) * mask(k,l);
			if (k!=0 || l!=0) m += val*val*MASK[k][l];
		}
	}

	// pop=vari(z);
	float pop = vari(z);
	// if pop ~= 0: pop=(vari(z(1:4,1:4))+vari(z(1:4,5:8))+vari(z(5:8,5:8))+vari(z(5:8,1:4)))/pop;
	if (fabsf(pop) > FLT_EPSILON) {
		pop = (vari(z(cv::Range(0,4),cv::Range(0,4)))
			+vari(z(cv::Range(0,4),cv::Range(4,8)))
			+vari(z(cv::Range(4,8),cv::Range(4,8)))
			+vari(z(cv::Range(4,8),cv::Range(0,4)))) / pop;
	}

	// m = sqrt(m*pop)/32;
	return sqrtf(m*pop)/32.0f;
}

float PSNRHVS::vari(const cv::Mat &z)
{
	float mean = 0.0;
	float d = 0.0;

	int w = z.cols;
	int h = z.rows;
	float N = static_cast<float>(h*w);

	for (int i=0; i<h; i++) {
		const float *ptr = z.ptr<float>(i);
		for (int j=0; j<w; j++) {
			d += (*ptr)*(*ptr);
			mean += *ptr++;
		}
	}
	d /= N;
	mean /= N;
	d -= mean*mean;
	d *= N*N/(N-1);

	// d=var(AA(:))*length(AA(:));
	return d;
}
