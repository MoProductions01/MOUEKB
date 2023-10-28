// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PushingCube.generated.h"

class UStaticMeshComponent;

UCLASS()
class MOUEKB_API APushingCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APushingCube();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CubeMesh;

private:
	UPROPERTY(EditAnywhere, Category="Pushing Cube")
	FVector PlatformVelocity = FVector(100, 0, 0);
	UPROPERTY(EditAnywhere, Category = "Pushing Cube")
	float MoveDistance = 0;	

	UPROPERTY(VisibleAnywhere)
	float DistanceMoved = -1;

	FVector StartLocation;
};
