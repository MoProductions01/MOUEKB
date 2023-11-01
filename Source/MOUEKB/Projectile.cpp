// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 1300.f;
	ProjectileMovementComponent->InitialSpeed = 1300.f;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	TrailParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if(LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// DAMAGE SETUP:
// 1) OnHit is on the Projectile and is setup in Projectile::BeginPlay()
// 3) HealthComponent handles health/damage.  Because it's an ActorComponent it can be added to any Actor
// 4) Add the HealthComponent to Tank and Turret via BP.
// 5) Actors inherit damage events, such as OnTakeAnyDamage.  We bind that delegate to the DamageTaken() on HealthComponent.
// 6) In HealthComponent::BeingPlay(), bind the OnTakeAnyDamage to the HealthComponent's OWNER, not itself.
// 7) From HealthComponent::BeginPlay(): GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
// 8) So we're binding the HealthComponent's Owner to a function on HealthDelegate itself. 
// 9) So Projectile::OnHit() is called when the projectile hits sometihng.  
//		ApplyDamage() is called from within OnHit().  This generates the damage event.
//		Because HealthComponent::DamageTaken is the delegate for OnTakeAnyDamage, DamageTaken is called after
//			ApplyDamage() is called.
// 10) Last input parameter for ApplyDamage() is TSubclassOf<class UDamageType> DamageTypeClass;
//		Because of this, we need the proper UClass type to send in (not going to customize damage).
// 		Need a UClass parameter to satisfy the UDamageType class.
//	11) If we want to get the UClass of any particular, such as UDamageType, we can call UDamageType::StaticClass()
//	12) Because we're not creating any blueprint based on UDamageType and setting up extra data, we don't
//		need a TSubclassOf variable, so in this case we can use StaicClass() and this will return a UClass
//		that represents this type.  We can store this in a Variable.

// DAMAGE/DEATH CYCLE
//  1) In Projectile::BeginPlay(), bind StaticMesh::OnComponentHit to Projectile::OnHit()
//  2) In HealthComponent::BeginPlay(), bind Actor::OnTakeAnyDamage() to the Tank/Tower's HealthComponent::DamageTaken()
//  3) In Projectile::OnHit(), call the UGameplayStatics::ApplyDamage().  This will call the Tank/Tower's HealthComponent::DamageTaken()
//  4) In HealthComponent::DamageTaken(), check if Health<= 0.  If so, call GameMode::ActorDied()
//  5) In GameMode::ActorDied(), call Tank/Tower::HandleDestruction()
//  6) In Tank/Tower::HandleDestruction(), deal with being destroyed.
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{		
	UE_LOG(LogTemp, Display, TEXT("--------------Projectile::OnHit()"));
	AActor* MyOwner = GetOwner();
	if(MyOwner == nullptr) 
	{
		UE_LOG(LogTemp, Error, TEXT("Null Owner in OnHit: %s"), *GetName()); 
		Destroy();
		return;
	}
	
		
	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	// Need the UClass from the TSubclassOf
	UClass* DamageTypeClass = UDamageType::StaticClass(); // returns a UClass Pointer

	if(OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		//UE_LOG(LogTemp, Display, TEXT("--------------Projectile::OnHit() about to call UGameplayStatics::ApplyDamage"));
		// this will generate the damage event set up in HealthComponent::BeingPlay (DamageTaken)
		// float ApplyDamage(AActor* DamagedActor, float BaseDamage, AController* EventInstigator, AActor* DamageCauser, TSubclassOf<class UDamageType> DamageTypeClass);
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		if(HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());		
		}
		if(LaunchSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
		}
		if(HitCameraShakeClass)
		{	
			UE_LOG(LogTemp, Display, TEXT("------Play Camera Shake"));
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No Camera Shake available"));
		}
	}	
	Destroy();

	//UE_LOG(LogTemp, Display, TEXT("--------------OnHit"));
	//UE_LOG(LogTemp, Display, TEXT("HitComp: %s"), *HitComp->GetName());
	//UE_LOG(LogTemp, Display, TEXT("OtherActor: %s"), *OtherActor->GetName());
	//UE_LOG(LogTemp, Display, TEXT("OtherComp: %s"), *OtherComp->GetName()); 
}