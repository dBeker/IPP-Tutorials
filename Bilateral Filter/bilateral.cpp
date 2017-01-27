#include "ippi.h"

int bilateral(Ipp8u* src, int srcStride, int width, int height, int radius, double sigmaColor, double sigmaSpace, Ipp8u** dst, int* dstStep)
{
	*dst = ippiMalloc_8u_C1(width, height, dstStep);
	
	IppiFilterBilateralType filter = IppiFilterBilateralType::ippiFilterBilateralGauss;
	int d = radius * 2 + 1; 
	IppiSize kernel = { d, d };
	IppiSize roi = { width - 1, height - 1 };
	IppiSize imageSize = { width, height };
	
	int bufsize;
	IppStatus status = ippiFilterBilateralGetBufSize_8u_C1R(filter, roi, kernel, &bufsize);
	if(status != ippStsNoErr) finalize(); 
	
	status = IppiFilterBilateralSpec* pSpec = (IppiFilterBilateralSpec*)ippsMalloc_8u(bufsize);
	if(status != ippStsNoErr) finalize(); 

	status = ippiFilterBilateralInit_8u_C1R(filter, kernel, sigmaColor, sigmaSpace, d, pSpec);
	if(status != ippStsNoErr) finalize();

						
	status = ippiFilterBilateral_8u_C1R(
		src,
		srcStride,
		*dst,
		*dstStep,
		kernel,
		pSpec);
	if(status != ippStsNoErr) finalize(); 

finalize:
	if (pSpec)
	{
		ippFree(pSpec);
	}
	
	return status;
}