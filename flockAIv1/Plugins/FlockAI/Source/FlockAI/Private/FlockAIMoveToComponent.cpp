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
	roatationspeed = 5;
	FVector origin;
	Owner->GetActorBounds(false, origin, ownerbound);
	FVector origin1;
	AILeader->GetActorBounds(false, origin1, aileaderbound);
	keepdistance = ownerbound.Size() + aileaderbound.Size();
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
		if (!isturning)
		{
			if (FVector::Dist(Owner->GetActorLocation(), AILeader->GetActorLocation()) > keepdistance)
			{
				Owner->SetActorLocation(UKismetMathLibrary::VLerp(Owner->GetActorLocation(), AILeader->GetActorLocation(), 0.001 * movespeed));
			}
		}

		///// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//TArray<TEnumAsByte<EObjectTypeQuery>>otq;
		////otq.Add(EObjectTypeQuery::ObjectTypeQuery2);
		//otq.Add(EObjectTypeQuery::ObjectTypeQuery1);
		//TArray<AActor*> actortoignore;
		//actortoignore.Add(Owner);
		//TArray<AActor*>outactors;
		//UKismetSystemLibrary::SphereOverlapActors(this, Owner->GetActorLocation(), ownerbound.Size() *3, otq,AActor::StaticClass(), actortoignore, outactors);
		//GEngine->AddOnScreenDebugMessage(-1, .7f, FColor::Red, FString::FromInt(outactors.Num()));
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///forward
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
				auto ft = hitresult.Actor->GetComponentByClass(UFlockAIMoveToComponent::StaticClass());
				if (ft)
				{
					Owner->AddActorWorldOffset(FMath::VRand() * turnspeed);
					isturning = true;
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString("ssssssssssss"));
				}
				else
				{
					isturning = false;
				}
			}
			else
			{
				isturning = false;
			}

		}

/////////////////////////////////////////////////////////////////////////////
		////////////////right
		{
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
				auto ft = hitresult.Actor->GetComponentByClass(UFlockAIMoveToComponent::StaticClass());
				if (ft)
				{
					Owner->AddActorWorldOffset(-Owner->GetActorRightVector() * turnspeed);
					isturning = true;
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString("ssssssssssss"));
				}
				else
				{
					isturning = false;
				}
			}
			else
			{
				isturning = false;
			}
		}
//////////////////////////////////////////////////////////////////////////////
		///////////left
		//{
		//	FVector starpoint = Owner->GetActorLocation();
		//	FVector director = -Owner->GetActorRightVector();
		//	FVector endpoint = starpoint + (ownerbound.Y * 1.5 * director);
		//	FVector HalfSize = FVector(ownerbound.X, 5, ownerbound.Z);
		//	TArray<AActor*> actorarray;
		//	FHitResult hitresult;
		//	bool b = UKismetSystemLibrary::BoxTraceSingle(this, starpoint, endpoint, HalfSize, Owner->GetActorForwardVector().Rotation(),
		//		ETraceTypeQuery::TraceTypeQuery2, true, actorarray,
		//		EDrawDebugTrace::Type::None, hitresult, true,
		//		FLinearColor::Red, FLinearColor::Green, 5.0f);
		//	if (b)
		//	{
		//		//distance = hitresult.Distance;
		//		//hitpoint = hitresult.ImpactPoint;
		//		//FName name = hitresult.Actor->GetFName();
		//		////GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, name.ToString());
		//		//////GEngine->AddOnScreenDebugMessage(-1, 35.0f, FColor::Yellow, FString::FromInt(distance).Append(" distance"));
		//		//bhitbackground = name.IsEqual("background");
		//	}
		//}
		//////////////////////////////////////////////////////////////////////////////
		///////////up
/////////////////////////////////////////////////////////////////////////////////////	
		////////////up
		{
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
				auto ft = hitresult.Actor->GetComponentByClass(UFlockAIMoveToComponent::StaticClass());
				if (ft)
				{
					Owner->AddActorWorldOffset(-Owner->GetActorUpVector() * turnspeed* FMath::FRand());
					isturning = true;
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString("ssssssssssss"));
				}
				else
				{
					isturning = false;
				}
			}
			else
			{
				isturning = false;
			}
		}
	}
	// ...
}

