// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuroEffectActor.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class AURA_API AAuroEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuroEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent>Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent>Sphere;
};



