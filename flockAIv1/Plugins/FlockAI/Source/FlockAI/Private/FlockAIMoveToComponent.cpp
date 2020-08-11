// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockAIMoveToComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine.h"
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
	FVector origin;
	Owner->GetActorBounds(false, origin, ownerbound);
	FVector origin1;
	if (AILeader)
	{
		AILeader->GetActorBounds(false, origin1, aileaderbound);
		keepdistance = ownerbound.Size() + aileaderbound.Size();
	}

}


// Called every frame
void UFlockAIMoveToComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (AILeader)
	{
		//UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), AILeader->GetActorLocation());
		
		//UKismetMathLibrary::RLerp(Owner->GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), AILeader->GetActorLocation()),0.005f,true);
		auto forwarddetected = [=]()
		{

#define FORWARDDIS 1.5
				FVector starpoint = Owner->GetActorLocation();
				FVector director = Owner->GetActorForwardVector();
				FVector endpoint = starpoint + (ownerbound.X * FORWARDDIS * director);
				FVector HalfSize = FVector(5, ownerbound.Y, ownerbound.Z);
				TArray<AActor*> actorarray;
				FHitResult hitresult;
				bool b = UKismetSystemLibrary::BoxTraceSingle(this, starpoint, endpoint, HalfSize, director.Rotation(),
					ETraceTypeQuery::TraceTypeQuery2, true, actorarray,
					EDrawDebugTrace::Type::None, hitresult, true,
					FLinearColor::Red, FLinearColor::Green, 5.0f);
				if (b)
				{
					forwardactor = hitresult.Actor;
					auto ft = forwardactor->GetComponentByClass(UFlockAIMoveToComponent::StaticClass());
					if (ft)
					{
						movespeed = SLOWSPEED;
						roatationspeed = SLOWROTATION;

						forwardblockgoalrotation = Owner->GetTransform().TransformRotation(FRotator(0, FMath::RandRange(-20, 2), FMath::RandRange(-20, 2)).Quaternion()).Rotator();
						b_fb = true;
					}

				}
		};
		auto rightdetected = [=]() {

#define RIGHTDIS 3

				FVector starpoint = Owner->GetActorLocation();
				FVector director = Owner->GetActorRightVector();
				FVector endpoint = starpoint + (ownerbound.Y * RIGHTDIS * director);
				FVector HalfSize = FVector(ownerbound.X, 5, ownerbound.Z);
				TArray<AActor*> actorarray;
				FHitResult hitresult;
				bool b = UKismetSystemLibrary::BoxTraceSingle(this, starpoint, endpoint, HalfSize, Owner->GetActorForwardVector().Rotation(),
					ETraceTypeQuery::TraceTypeQuery2, true, actorarray,
					EDrawDebugTrace::Type::None, hitresult, true,
					FLinearColor::Red, FLinearColor::Green, 5.0f);
				if (b)
				{
					rightactor = hitresult.Actor;
					auto ft = rightactor->GetComponentByClass(UFlockAIMoveToComponent::StaticClass());
					if (ft)
					{
						movespeed = SLOWSPEED;
						roatationspeed = SLOWROTATION;
						b_rb = true;
					}

				}
		};
		auto updetected = [=]() {
			{
				/////////////////////////////////////////////////////////////////////////////////////	
			////////////up
#define UPDIS 3
				FVector starpoint = Owner->GetActorLocation();
				FVector director = Owner->GetActorUpVector();
				FVector endpoint = starpoint + (ownerbound.Y * UPDIS * director);
				FVector HalfSize = FVector(ownerbound.X, ownerbound.Y, 5);
				TArray<AActor*> actorarray;
				FHitResult hitresult;
				bool b = UKismetSystemLibrary::BoxTraceSingle(this, starpoint, endpoint, HalfSize, Owner->GetActorForwardVector().Rotation(),
					ETraceTypeQuery::TraceTypeQuery2, true, actorarray,
					EDrawDebugTrace::Type::None, hitresult, true,
					FLinearColor::Red, FLinearColor::Green, 5.0f);
				if (b)
				{
					upactor = hitresult.Actor;
					auto ft = upactor->GetComponentByClass(UFlockAIMoveToComponent::StaticClass());
					if (ft)
					{
						movespeed = SLOWSPEED;
						roatationspeed = SLOWROTATION;
						b_ub = true;
					}
				}
			}
		};
		forwarddetected();
		rightdetected();
		updetected();
		if (b_fb)
		{
			FRotator currentrotation = Owner->GetActorRotation();
			if ((currentrotation - forwardblockgoalrotation).IsNearlyZero(1.5))
			{
				b_fb = false;
			}
			else
			{
				Owner->SetActorRotation(
					UKismetMathLibrary::RLerp(
						currentrotation,
						forwardblockgoalrotation,
						roatationspeed * 0.005f,
						true
					)
				);
			}
			movespeed = SLOWSPEED;
			roatationspeed = SLOWROTATION;
		}


		switch (state)
		{
		case AIMoveStatus::normal:

			Owner->SetActorRotation(
				UKismetMathLibrary::RLerp(
					Owner->GetActorRotation(),
					UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), AILeader->GetActorLocation()),
					roatationspeed * 0.005f,
					true
				)
			);
			if (b_fb || b_rb || b_ub)
			{
				state = AIMoveStatus::avoid;
			}
			break;
		case AIMoveStatus::avoid:
			if (b_rb)
			{
				Owner->AddActorWorldOffset(-Owner->GetActorRightVector() * turnspeed * FMath::FRand());
				movespeed = FMath::RandRange(SLOWSPEED, SLOWSPEED + 1);
				if (rightmovecounter++ > 5)
				{
					b_rb = false;
					rightmovecounter = 0;
				}
	
			}
			if (b_ub)
			{
				Owner->AddActorWorldOffset(-Owner->GetActorUpVector() * turnspeed * FMath::FRand());
				movespeed = FMath::RandRange(SLOWSPEED, SLOWSPEED + 1);
				if (rightmovecounter++ > 5)
				{
					b_ub = false;

					upmovecounter = 0;
				}
			}
			if (b_fb || b_rb || b_ub)
			{		
			}
			else
			{
				movespeed = NORMALSPEED;
				roatationspeed = NORMALROTATION;
				state = AIMoveStatus::normal;
			}

			break;
		default:
			break;
		}
		if (FVector::Dist(Owner->GetActorLocation(), AILeader->GetActorLocation()) > keepdistance)
		{
			Owner->AddActorWorldOffset(Owner->GetActorForwardVector() * movespeed);
		}
		else
		{

		}

	}
	// ...
}

