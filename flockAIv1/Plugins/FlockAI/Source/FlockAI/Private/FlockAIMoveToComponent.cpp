// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockAIMoveToComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values for this component's properties
UFlockAIMoveToComponent::UFlockAIMoveToComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFlockAIMoveToComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	// ...
	roatationspeed = 5;
	FVector origin, bound;
	Owner->GetActorBounds(false, origin,bound);
	FVector origin1, bound1;
	AILeader->GetActorBounds(false, origin1, bound1);
	keepdistance = bound.Size() + bound1.Size();
}


// Called every frame
void UFlockAIMoveToComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (AILeader)
	{
		//UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), AILeader->GetActorLocation());
		
		//UKismetMathLibrary::RLerp(Owner->GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), AILeader->GetActorLocation()),0.005f,true);
		Owner->SetActorRotation(
			UKismetMathLibrary::RLerp(
				Owner->GetActorRotation(),
				UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(),AILeader->GetActorLocation()), 
				roatationspeed*0.005f, 
				true
			)
		);
		float dis = FVector::Dist(Owner->GetActorLocation(), AILeader->GetActorLocation());
		float speedmove = (dis - keepdistance)> maxspeed ? maxspeed : (dis - keepdistance);
		Owner->AddActorWorldOffset(
			Owner->GetActorForwardVector()*
			speedmove
		);
		
	}
	// ...
}

