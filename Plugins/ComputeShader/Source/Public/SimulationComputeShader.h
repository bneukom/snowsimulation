/******************************************************************************
* The MIT License (MIT)
*
* Copyright (c) 2015 Fredrik Lindh
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
******************************************************************************/

#pragma once

#include "Private/ComputeShaderDeclaration.h"

/** Simulation Cell */
struct FComputeShaderSimulationCell
{
	float Aspect;
	float Inclination;
	float Altitude;
	float Area;
	float SWE;

	FComputeShaderSimulationCell(float Aspect, float Inclination, float Altitude, float Area, float SWE) : 
		Aspect(Aspect), Inclination(Inclination), Altitude(Altitude), Area(Area), SWE(SWE)
	{

	}

};

/** Encapsulates a GPU read/write structured buffer with its UAV and SRV. */
struct FRWStructuredBuffer
{
	FStructuredBufferRHIRef Buffer;
	FUnorderedAccessViewRHIRef UAV;
	FShaderResourceViewRHIRef SRV;
	uint32 NumBytes;

	FRWStructuredBuffer() : NumBytes(0) {}

	void Initialize(uint32 BytesPerElement, uint32 NumElements, FResourceArrayInterface* Data = nullptr, uint32 AdditionalUsage = 0, bool bUseUavCounter = false, bool bAppendBuffer = false)
	{
		check(GMaxRHIFeatureLevel == ERHIFeatureLevel::SM5);
		NumBytes = BytesPerElement * NumElements;
		FRHIResourceCreateInfo CreateInfo;
		CreateInfo.ResourceArray = Data;
		Buffer = RHICreateStructuredBuffer(BytesPerElement, NumBytes, BUF_UnorderedAccess | BUF_ShaderResource | AdditionalUsage, CreateInfo);
		UAV = RHICreateUnorderedAccessView(Buffer, bUseUavCounter, bAppendBuffer);
		SRV = RHICreateShaderResourceView(Buffer);
	}

	void Release()
	{
		NumBytes = 0;
		Buffer.SafeRelease();
		UAV.SafeRelease();
		SRV.SafeRelease();
	}
};

/**
* This class demonstrates how to use the compute shader we have declared.
* Most importantly which RHI functions are needed to call and how to get 
* some interesting output.                                                
*/
class COMPUTESHADER_API FSimulationComputeShader
{
public:
	FSimulationComputeShader(float SimulationSpeed, int32 SizeX, int32 SizeY, ERHIFeatureLevel::Type ShaderFeatureLevel);
	~FSimulationComputeShader();

	/**
	* Run this to execute the compute shader once!
	* @param TotalElapsedTimeSeconds - We use this for simulation state 
	*/
	void ExecuteComputeShader(float TotalElapsedTimeSeconds);

	/**
	* Only execute this from the render thread.
	*/
	void ExecuteComputeShaderInternal();

	FTexture2DRHIRef GetTexture() { return Texture; }

private:
	bool IsComputeShaderExecuting;
	bool IsUnloading;

	int32 NumCells;

	FComputeShaderConstantParameters ConstantParameters;
	FComputeShaderVariableParameters VariableParameters;
	ERHIFeatureLevel::Type FeatureLevel;

	/** Main texture */
	FTexture2DRHIRef Texture;

	/** We need a UAV if we want to be able to write to the resource*/
	FUnorderedAccessViewRHIRef TextureUAV;

	/** Cells for the simulation. */
	FRWStructuredBuffer* SimulationCellsBuffer;

	/** Temperature data for the simulation. */
	FRWStructuredBuffer* TemperatureDataBuffer;
};