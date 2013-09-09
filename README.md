# VQMT - Video Quality Measurement Tool

This software provides fast implementations of the following objective metrics:

* PSNR: Peak Signal-to-Noise Ratio,
* SSIM: Structural Similarity,
* MS-SSIM: Multi-Scale Structural Similarity,
* VIFp: Visual Information Fidelity, pixel domain version
* PSNR-HVS: Peak Signal-to-Noise Ratio taking into account Contrast Sensitivity
  Function (CSF),
* PSNR-HVS-M: Peak Signal-to-Noise Ratio taking into account Contrast
  Sensitivity Function (CSF) and between-coefficient contrast masking of DCT
  basis functions.

In this software, the above metrics are implemented in C++ with the help of
OpenCV and are based on the original Matlab implementations provided by their
developers.
The source code of this software can be compiled on any platform and
only requires the OpenCV library (core and imgproc modules).
This software allows performing video quality assessment without using Matlab
and shows better performance than Matlab in terms of run time.

# PREREQUISITE

The OpenCV library (http://opencv.willowgarage.com/wiki/) needs to be installed
to be able to compile this code. Only the core and imgproc modules are required.

# BUILD

CMake is required in order to build VQMT. Here are the required compilation
steps:

Create the build directory:

	mkdir build

Navigate into it:

	cd build

Run the cmake command to generate a Makefile that works for your operating
system:

	cmake ..

Now you can use the make command to build the software:

	make

The binary will be placed into a bin  sub-directory.

# USAGE

vqmt (or VQMT.exe on Windows) OriginalVideo ProcessedVideo Height Width 
NumberOfFrames ChromaFormat Output Metrics

OriginalVideo: the original video as raw YUV video file, progressively scanned, 
and 8 bits per sample
ProcessedVideo: the processed video as raw YUV video file, progressively 
scanned, and 8 bits per sample
Height: the height of the video
Width: the width of the video
NumberOfFrames: the number of frames to process
ChromaFormat: the chroma subsampling format. 0: YUV400, 1: YUV420, 2: YUV422, 3: 
YUV444
Output: the name of the output file(s)
Metrics: the list of metrics to use

Available metrics:
* PSNR: Peak Signal-to-Noise Ratio (PNSR)
* SSIM: Structural Similarity (SSIM)
* MSSSIM: Multi-Scale Structural Similarity (MS-SSIM)
* VIFP: Visual Information Fidelity, pixel domain version (VIFp)
* PSNRHVS: Peak Signal-to-Noise Ratio taking into account Contrast Sensitivity 
  Function (CSF) (PSNR-HVS)
* PSNRHVSM: Peak Signal-to-Noise Ratio taking into account Contrast Sensitivity 
  Function (CSF) and between-coefficient contrast masking of DCT basis 
  functions (PSNR-HVS-M)

Example:

VQMT.exe original.yuv processed.yuv 1088 1920 250 1 results PSNR SSIM MSSSIM 
VIFP

will create the following output files in CSV (comma-separated values) format:
* results_pnsr.csv
* results_ssim.csv
* results_msssim.csv
* results_vifp.csv

Notes:
* SSIM comes for free when MSSSIM is computed (but you still need to specify it 
  to get the output)
* PSNRHVS and PSNRHVSM are always computed at the same time (but you still need 
  to specify both to get the two outputs)
* When using MSSSIM, the height and width of the video have to be multiple of 16
* When using VIFP, the height and width of the video have to be multiple of 8

# COPYRIGHT

Permission is hereby granted, without written agreement and without license or 
royalty fees, to use, copy, modify, and distribute the software provided and its 
documentation for research purpose only, provided that this copyright notice and 
the original authors' names appear on all copies and supporting documentation.
The software provided may not be commercially distributed. In no event shall the 
Ecole Polytechnique Fédérale de Lausanne (EPFL) be liable to any party for 
direct, indirect, special, incidental, or consequential damages arising out of 
the use of the software and its documentation.
The Ecole Polytechnique Fédérale de Lausanne (EPFL) specifically disclaims any 
warranties.
The software provided hereunder is on an "as is" basis and the Ecole 
Polytechnique Fédérale de Lausanne (EPFL) has no obligation to provide 
maintenance, support, updates, enhancements, or modifications.

# REFERENCES

* Z. Wang, A.C. Bovik, H.R. Sheikh, and E.P. Simoncelli, "Image quality 
  assessment: from error visibility to structural similarity," IEEE 
  Transactions on Image Processing, vol. 13, no. 4, pp. 600–612, April 2004.
* Z. Wang, E.P. Simoncelli, and A.C. Bovik, "Multiscale structural similarity 
  for image quality assessment," in IEEE Asilomar Conference on Signals, 
  Systems and Computers, November 2003, vol. 2, pp. 1398–1402.
* H.R. Sheikh and A.C. Bovik, "Image information and visual quality," IEEE 
  Transactions on Image Processing, vol. 15, no. 2, pp. 430-444, February 2006.
* K. Egiazarian, J. Astola, N. Ponomarenko, V. Lukin, F. Battisti, and M. 
  Carli, "New full-reference quality metrics based on HVS," in Proceedings of 
  the Second International Workshop on Video Processing and Quality Metrics, 
  2006.
* N. Ponomarenko, F. Silvestri, K. Egiazarian, M. Carli, J. Astola, and V. 
  Lukin, "On between-coefficient contrast masking of DCT basis functions," in 
  Proceedings of the Third International Workshop on Video Processing and 
  Quality Metrics for Consumer Electronics, January 2007.
