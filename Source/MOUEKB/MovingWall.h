// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovingWall.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOUEKB_API UMovingWall : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMovingWall();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere)
	FVector MoveOffset;		

	UPROPERTY(EditAnywhere)
	float MoveTime = 4;

	//UPROPERTY(EditAnywhere)
	bool ShouldMove = false;

	FVector OriginalLocation;
		
};
