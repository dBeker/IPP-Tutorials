int morphologicalOpenning(Ipp8u* src, int srcStep, int width, int height, Ipp8u** dst, int* dstStep, int kernelSize, int iteration)
{
	int specSize = 0, bufferSize = 0, pMaskStep, bufferStep;
	IppiMorphAdvState* pState;
	Ipp8u* pBuffer;
	Ipp8u* pMask;
	Ipp8u* buffer;
	
	IppiSize roiSize = { width, height };
	IppiSize maskSize = { kernelSize, kernelSize };
	IppiPoint anchor = { 0,0 };
	IppiBorderType borderType = IppiBorderType::ippBorderRepl;

	// Initialize Output
	*dst = ippiMalloc_8u_C1(width, height, dstStep);

	// Initialize mask
	pMask = ippiMalloc_8u_C1(maskSize.width, maskSize.height, &pMaskStep);

	ippiMorphAdvGetSize_8u_C1R(roiSize, pMask, maskSize, &bufferSize);
	pBuffer = (Ipp8u*)ippsMalloc_8u(bufferSize);
	ippiMorphAdvInitAlloc_8u_C1R(&pState, roiSize, pMask, maskSize, anchor);

	*dst = ippiMalloc_8u_C1(roiSize.width, roiSize.height, &dstStep);
	buffer = ippiMalloc_8u_C1(roiSize.width, roiSize.height, &bufferStep);

	ippiCopy_8u_C1R(src, srcStep, dst, dstStep, roiSize);

	for (int i = 0; i < iteration; i++)
	{
		ippiCopy_8u_C1R(dst, dstStep, buffer, bufferStep, roiSize);
		ippiMorphOpenBorder_8u_C1R(buffer, bufferStep, dst, dstStep, roiSize, borderType, pState);
	}

	if (pMask)
	{
		ippiFree(pMask);
	}	
	if (pBuffer)
	{
		ippiFree(pBuffer);
	}
	if (buffer)
	{
		ippiFree(buffer);
	}	
}