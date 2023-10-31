// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Grabber.generated.h"

class UInputMappingContext;
class UInputAction;
class UPhysicsHandleComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOUEKB_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void GrabberGrab();
	
	UFUNCTION(BlueprintCallable)
	void GrabberRelease();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* GrabberMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* GrabAction;

private: 
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 400.f;		

	UPROPERTY(EditAnywhere)
	float GrabRadius = 100;		

	UPROPERTY(EditAnywhere)
	float HoldDistance = 300;		

	UPhysicsHandleComponent* GetPhysicsHandle() const;
	bool GetGrabbableInReach(FHitResult& OutHitResult) const;
};
