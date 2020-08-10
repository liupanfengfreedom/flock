// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlockAIMoveToComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLOCKAI_API UFlockAIMoveToComponent : public UActorComponent
{
	GENERATED_BODY()
	AActor* Owner;
	int roatationspeed = 1;
	float keepdistance;
	float movespeed=1;
	FVector ownerbound;
	FVector aileaderbound;
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

		
};
