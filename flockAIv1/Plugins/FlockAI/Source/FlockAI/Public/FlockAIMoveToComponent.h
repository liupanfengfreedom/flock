// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlockAIMoveToComponent.generated.h"

UENUM()
enum AIMoveStatus
{
	normal,
    avoid,
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLOCKAI_API UFlockAIMoveToComponent : public UActorComponent
{
	GENERATED_BODY()
	AActor* Owner;
#define FASTROTATION 4.1f
#define NORMALROTATION 2.1f
#define SLOWROTATION 1.2f
	int roatationspeed = NORMALROTATION;
	float keepdistance;
#define FASTSPEED 2.3f
#define NORMALSPEED 2.1f
#define SLOWSPEED 1.2f
	float movespeed= NORMALSPEED;
	FVector ownerbound;
	FVector aileaderbound;
	float turnspeed=1;
	AIMoveStatus state = AIMoveStatus::normal;
	FRotator forwardblockgoalrotation;
	bool b_fb = false;
	bool b_rb = false;
	bool b_ub = false;
	FVector forwardactorbound;
	TWeakObjectPtr<class AActor> forwardactor;
	
	FVector rightactorbound;
	TWeakObjectPtr<class AActor>  rightactor;
	int rightmovecounter=0;
	FVector upactorbound;
	TWeakObjectPtr<class AActor>  upactor;
	int upmovecounter=0;

public:	
	// Sets default values for this component's properties
	UFlockAIMoveToComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlockAI|GeneralConfig")
	AActor* AILeader;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	int getstate()
	{
		return (int)(state);
	}
	UFUNCTION(BlueprintCallable)
		float getmovespeed()
	{
		return movespeed;
	}
};
