// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SimulationDataProviderBase.generated.h"

/**
* Precipitation data.
*/
USTRUCT(Blueprintable)
struct FPrecipitation
{
	GENERATED_USTRUCT_BODY()


};

/**
* Temperature data.
*/
USTRUCT(Blueprintable)
struct FTemperature
{
	GENERATED_USTRUCT_BODY()

	/** Minimum Temperature in degree Celsius. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature")
	float Minimum;

	/** Maximum Temperature in degree Celsius. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature")
	float Maximum;

	/** Mean Temperature in degree Celsius. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature")
	float Mean;


	FTemperature(float minimum, float maximum, float mean) : Minimum(minimum), Maximum(maximum), Mean(mean) {}

	FTemperature() : Minimum(0), Maximum(0), Mean(0) {}

};

/**
 * Base class for all data providers for the simulation.
 */
UCLASS(BlueprintType)
class SNOWSIMULATION_API USimulationDataProviderBase : public UObject
{
	GENERATED_BODY()

public:

	/** 
	* Returns the temperature data at base elevation at the given timestep (in hours).
	*/
	FTemperature GetTemperatureAt(int timestep);

	/**
	* Returns the temperature data at base elevation at the given timestep (in hours).
	*/
	FPrecipitation GetPercipitationAt(int timestep);

};