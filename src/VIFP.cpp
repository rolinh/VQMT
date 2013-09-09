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

#include "VIFP.hpp"

const float VIFP::SIGMA_NSQ = 2.0f;

VIFP::VIFP(int h, int w) : Metric(h, w)
{
}

float VIFP::compute(const cv::Mat& original, const cv::Mat& processed)
{
	double num = 0.0;
	double den = 0.0;
	
	cv::Mat ref[NLEVS];
	cv::Mat dist[NLEVS];
	cv::Mat tmp1, tmp2;
	
	int w = width;
	int h = height;
	
	// for scale=1:4
	for (int scale=0; scale<NLEVS; scale++) {
		// N=2^(4-scale+1)+1;
		int N = (2 << (NLEVS-scale-1)) + 1;
		
		if (scale == 0) {
			original.copyTo(ref[scale]);
			processed.copyTo(dist[scale]);
		}
		else {
			// ref=filter2(win,ref,'valid');
			applyGaussianBlur(ref[scale-1], tmp1, N, N/5.0);
			// dist=filter2(win,dist,'valid');
			applyGaussianBlur(dist[scale-1], tmp2, N, N/5.0);
			
			w = (w-(N-1)) / 2;
			h = (h-(N-1)) / 2;
			
			ref[scale] = cv::Mat(h,w,CV_32F);
			dist[scale] = cv::Mat(h,w,CV_32F);
			
			// ref=ref(1:2:end,1:2:end);
			cv::resize(tmp1, ref[scale], cv::Size(w,h), 0, 0, cv::INTER_NEAREST);
			// dist=dist(1:2:end,1:2:end);
			cv::resize(tmp2, dist[scale], cv::Size(w,h), 0, 0, cv::INTER_NEAREST);
		}
		
		computeVIFP(ref[scale], dist[scale], N, num, den);
	}
	
	return float(num/den);
}

void VIFP::computeVIFP(const cv::Mat& ref, const cv::Mat& dist, int N, double& num, double& den)
{
	int w = ref.cols - (N-1);
	int h = ref.rows - (N-1);
	
	cv::Mat tmp(h,w,CV_32F);
	cv::Mat mu1(h,w,CV_32F), mu2(h,w,CV_32F), mu1_sq(h,w,CV_32F), mu2_sq(h,w,CV_32F), mu1_mu2(h,w,CV_32F), sigma1_sq(h,w,CV_32F), sigma2_sq(h,w,CV_32F), sigma12(h,w,CV_32F), g(h,w,CV_32F), sv_sq(h,w,CV_32F);
	cv::Mat sigma1_sq_th, sigma2_sq_th, g_th;
	
	// mu1 = filter2(win, ref, 'valid');
	applyGaussianBlur(ref, mu1, N, N/5.0);
	// mu2 = filter2(win, dist, 'valid');
	applyGaussianBlur(dist, mu2, N, N/5.0);
	
	const float EPSILON = 1e-10f;

	// mu1_sq = mu1.*mu1;
	cv::multiply(mu1, mu1, mu1_sq);
	// mu2_sq = mu2.*mu2;
	cv::multiply(mu2, mu2, mu2_sq);
	// mu1_mu2 = mu1.*mu2;
	cv::multiply(mu1, mu2, mu1_mu2);		
	
	// sigma1_sq = filter2(win, ref.*ref, 'valid') - mu1_sq;
	cv::multiply(ref, ref, tmp);
	applyGaussianBlur(tmp, sigma1_sq, N, N/5.0);
	sigma1_sq -= mu1_sq;
	// sigma2_sq = filter2(win, dist.*dist, 'valid') - mu2_sq;
	cv::multiply(dist, dist, tmp);
	applyGaussianBlur(tmp, sigma2_sq, N, N/5.0);
	sigma2_sq -= mu2_sq;
	// sigma12 = filter2(win, ref.*dist, 'valid') - mu1_mu2;
	cv::multiply(ref, dist, tmp);
	applyGaussianBlur(tmp, sigma12, N, N/5.0);
	sigma12 -= mu1_mu2;
	
	// sigma1_sq(sigma1_sq<0)=0;
	cv::max(sigma1_sq, 0.0f, sigma1_sq);
	// sigma2_sq(sigma2_sq<0)=0;
	cv::max(sigma2_sq, 0.0f, sigma2_sq);
	
	// g=sigma12./(sigma1_sq+1e-10);
	tmp = sigma1_sq + EPSILON;
	cv::divide(sigma12, tmp, g);
	
	// sv_sq=sigma2_sq-g.*sigma12;
	cv::multiply(g, sigma12, tmp);
	sv_sq = sigma2_sq - tmp;
	
	cv::threshold(sigma1_sq, sigma1_sq_th, EPSILON, 1.0f, cv::THRESH_BINARY);

	// g(sigma1_sq<1e-10)=0;
	cv::multiply(g, sigma1_sq_th, g);
	
	// sv_sq(sigma1_sq<1e-10)=sigma2_sq(sigma1_sq<1e-10);
	cv::multiply(sv_sq, sigma1_sq_th, sv_sq);
	cv::multiply(sigma2_sq, 1.0f - sigma1_sq_th, tmp);
	sv_sq += tmp;
	
	// sigma1_sq(sigma1_sq<1e-10)=0;
	cv::threshold(sigma1_sq, sigma1_sq, EPSILON, 1.0f, cv::THRESH_TOZERO);
	
	cv::threshold(sigma2_sq, sigma2_sq_th, EPSILON, 1.0f, cv::THRESH_BINARY);

	// g(sigma2_sq<1e-10)=0;
	cv::multiply(g, sigma2_sq_th, g);
	
	// sv_sq(sigma2_sq<1e-10)=0;
	cv::multiply(sv_sq, sigma2_sq_th, sv_sq);
	
	cv::threshold(g, g_th, 0.0f, 1.0f, cv::THRESH_BINARY);
	
	// sv_sq(g<0)=sigma2_sq(g<0);
	cv::multiply(sv_sq, g_th, sv_sq);
	cv::multiply(sigma2_sq, 1.0f - g_th, tmp);
	cv::add(sv_sq, tmp, sv_sq);
	
	// g(g<0)=0;
	cv::max(g, 0.0f, g);
	
	// sv_sq(sv_sq<=1e-10)=1e-10;
	cv::max(sv_sq, EPSILON, sv_sq);
	
	// num=num+sum(sum(log10(1+g.^2.*sigma1_sq./(sv_sq+sigma_nsq))));
	sv_sq += SIGMA_NSQ;
	cv::multiply(g, g, g);
	cv::multiply(g, sigma1_sq, g);
	cv::divide(g, sv_sq, tmp);
	tmp += 1.0f;
	cv::log(tmp, tmp);
	num += cv::sum(tmp)[0] / log(10.0f);
	
	// den=den+sum(sum(log10(1+sigma1_sq./sigma_nsq)));
	tmp = 1.0f + sigma1_sq / SIGMA_NSQ;
	cv::log(tmp, tmp);
	den += cv::sum(tmp)[0] / log(10.0f);
}
