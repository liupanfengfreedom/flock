// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockAIMoveToComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine.h"
#include "FlockAIEnemyComponent.h"
#include "Async/AsyncWork.h"
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
	//Async(EAsyncExecution::ThreadPool, [=]() {threadwork(); }, nullptr);
	//GetWorld()->GetTimerManager().SetTimer(th,this,&UFlockAIMoveToComponent::timerwork,0.02,true,0.1);
	GetWorld()->GetTimerManager().SetTimer(thf,this,&UFlockAIMoveToComponent::timerworkf,0.2,true,0.5);
	GetWorld()->GetTimerManager().SetTimer(thr,this,&UFlockAIMoveToComponent::timerworkr,0.2,true,0.5);
	GetWorld()->GetTimerManager().SetTimer(thu,this,&UFlockAIMoveToComponent::timerworku,0.2,true,0.5);
}
void UFlockAIMoveToComponent::timerworkf()
{
	{

#define FORWARDDIS 1.5
		FVector starpoint = Owner->GetActorLocation();
		FVector director = Owner->GetActorForwardVector();
		FVector endpoint = starpoint + (ownerbound.X * FORWARDDIS * director* movespeed*20);
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
			auto ft1 = forwardactor->GetComponentByClass(UFlockAIEnemyComponent::StaticClass());
			if (ft)
			{
				movespeed = SLOWSPEED;
				roatationspeed = SLOWROTATION;

				forwardblockgoalrotation = Owner->GetTransform().TransformRotation(FRotator(0, FMath::RandRange(-5, -2), FMath::RandRange(-5, -2)).Quaternion()).Rotator();
				b_fb = true;
			}
			else if (ft1)
			{
				movespeed = FASTSPEED;
				roatationspeed = FASTROTATION;

				forwardblockgoalrotation = Owner->GetTransform().TransformRotation(FRotator(0, FMath::RandRange(-90, -70), FMath::RandRange(-90, -70)).Quaternion()).Rotator();
				b_fb = true;
			}

		}
	}
}
void UFlockAIMoveToComponent::timerworkr()
{
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
			rightactor = hitresult.Actor;
			auto ft = rightactor->GetComponentByClass(UFlockAIMoveToComponent::StaticClass());
			if (ft)
			{
				movespeed = SLOWSPEED;
				roatationspeed = SLOWROTATION;
				b_rb = true;
			}

		}
	}
}
void UFlockAIMoveToComponent::timerworku()
{
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
}
void UFlockAIMoveToComponent::timerwork()
{
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
				auto ft1 = forwardactor->GetComponentByClass(UFlockAIEnemyComponent::StaticClass());
				if (ft)
				{
					movespeed = SLOWSPEED;
					roatationspeed = SLOWROTATION;

					forwardblockgoalrotation = Owner->GetTransform().TransformRotation(FRotator(0, FMath::RandRange(-20, 2), FMath::RandRange(-20, 2)).Quaternion()).Rotator();
					b_fb = true;
				}
				else if (ft1)
				{
					movespeed = FASTSPEED;
					roatationspeed = FASTROTATION;

					forwardblockgoalrotation = Owner->GetTransform().TransformRotation(FRotator(0, FMath::RandRange(-90, -30), FMath::RandRange(-90, -30)).Quaternion()).Rotator();
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
}
//void UFlockAIMoveToComponent::threadwork()
//{
//	while (true)
//	{
//		FPlatformProcess::Sleep(0.05);
//		if (AILeader)
//		{
//			//UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), AILeader->GetActorLocation());
//
//			//UKismetMathLibrary::RLerp(Owner->GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), AILeader->GetActorLocation()),0.005f,true);
//			auto forwarddetected = [=]()
//			{
//
//#define FORWARDDIS 1.5
//				FVector starpoint = Owner->GetActorLocation();
//				FVector director = Owner->GetActorForwardVector();
//				FVector endpoint = starpoint + (ownerbound.X * FORWARDDIS * director);
//				FVector HalfSize = FVector(5, ownerbound.Y, ownerbound.Z);
//				TArray<AActor*> actorarray;
//				FHitResult hitresult;
//				bool b = UKismetSystemLibrary::BoxTraceSingle(this, starpoint, endpoint, HalfSize, director.Rotation(),
//					ETraceTypeQuery::TraceTypeQuery2, true, actorarray,
//					EDrawDebugTrace::Type::None, hitresult, true,
//					FLinearColor::Red, FLinearColor::Green, 5.0f);
//				if (b)
//				{
//					forwardactor = hitresult.Actor;
//					auto ft = forwardactor->GetComponentByClass(UFlockAIMoveToComponent::StaticClass());
//					auto ft1 = forwardactor->GetComponentByClass(UFlockAIEnemyComponent::StaticClass());
//					if (ft)
//					{
//						movespeed = SLOWSPEED;
//						roatationspeed = SLOWROTATION;
//
//						forwardblockgoalrotation = Owner->GetTransform().TransformRotation(FRotator(0, FMath::RandRange(-20, 2), FMath::RandRange(-20, 2)).Quaternion()).Rotator();
//						b_fb = true;
//					}
//					else if (ft1)
//					{
//						movespeed = FASTSPEED;
//						roatationspeed = FASTROTATION;
//
//						forwardblockgoalrotation = Owner->GetTransform().TransformRotation(FRotator(0, FMath::RandRange(-90, -30), FMath::RandRange(-90, -30)).Quaternion()).Rotator();
//						b_fb = true;
//					}
//
//				}
//			};
//			auto rightdetected = [=]() {
//
//#define RIGHTDIS 3
//
//				FVector starpoint = Owner->GetActorLocation();
//				FVector director = Owner->GetActorRightVector();
//				FVector endpoint = starpoint + (ownerbound.Y * RIGHTDIS * director);
//				FVector HalfSize = FVector(ownerbound.X, 5, ownerbound.Z);
//				TArray<AActor*> actorarray;
//				FHitResult hitresult;
//				bool b = UKismetSystemLibrary::BoxTraceSingle(this, starpoint, endpoint, HalfSize, Owner->GetActorForwardVector().Rotation(),
//					ETraceTypeQuery::TraceTypeQuery2, true, actorarray,
//					EDrawDebugTrace::Type::None, hitresult, true,
//					FLinearColor::Red, FLinearColor::Green, 5.0f);
//				if (b)
//				{
//					rightactor = hitresult.Actor;
//					auto ft = rightactor->GetComponentByClass(UFlockAIMoveToComponent::StaticClass());
//					if (ft)
//					{
//						movespeed = SLOWSPEED;
//						roatationspeed = SLOWROTATION;
//						b_rb = true;
//					}
//
//				}
//			};
//			auto updetected = [=]() {
//				{
//					/////////////////////////////////////////////////////////////////////////////////////	
//				////////////up
//#define UPDIS 3
//					FVector starpoint = Owner->GetActorLocation();
//					FVector director = Owner->GetActorUpVector();
//					FVector endpoint = starpoint + (ownerbound.Y * UPDIS * director);
//					FVector HalfSize = FVector(ownerbound.X, ownerbound.Y, 5);
//					TArray<AActor*> actorarray;
//					FHitResult hitresult;
//					bool b = UKismetSystemLibrary::BoxTraceSingle(this, starpoint, endpoint, HalfSize, Owner->GetActorForwardVector().Rotation(),
//						ETraceTypeQuery::TraceTypeQuery2, true, actorarray,
//						EDrawDebugTrace::Type::None, hitresult, true,
//						FLinearColor::Red, FLinearColor::Green, 5.0f);
//					if (b)
//					{
//						upactor = hitresult.Actor;
//						auto ft = upactor->GetComponentByClass(UFlockAIMoveToComponent::StaticClass());
//						if (ft)
//						{
//							movespeed = SLOWSPEED;
//							roatationspeed = SLOWROTATION;
//							b_ub = true;
//						}
//					}
//				}
//			};
//			//AsyncTask(
//			//	ENamedThreads::GameThread,
//			//	[=]()
//			//	{
//					forwarddetected();
//					rightdetected();
//					updetected();
//					AsyncTask(
//					ENamedThreads::GameThread,
//					[=]()
//					{
//					if (b_fb)
//					{
//						FRotator currentrotation = Owner->GetActorRotation();
//						if ((currentrotation - forwardblockgoalrotation).IsNearlyZero(1.5))
//						{
//							b_fb = false;
//						}
//						else
//						{
//							Owner->SetActorRotation(
//								UKismetMathLibrary::RLerp(
//									currentrotation,
//									forwardblockgoalrotation,
//									roatationspeed * 0.005f,
//									true
//								)
//							);
//						}
//						movespeed = SLOWSPEED;
//						roatationspeed = SLOWROTATION;
//					}
//
//
//					switch (state)
//					{
//					case AIMoveStatus::normal:
//
//						Owner->SetActorRotation(
//							UKismetMathLibrary::RLerp(
//								Owner->GetActorRotation(),
//								UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), AILeader->GetActorLocation()),
//								roatationspeed * 0.005f,
//								true
//							)
//						);
//						if (b_fb || b_rb || b_ub)
//						{
//							state = AIMoveStatus::avoid;
//						}
//						break;
//					case AIMoveStatus::avoid:
//						if (b_rb)
//						{
//							Owner->AddActorWorldOffset(-Owner->GetActorRightVector() * turnspeed * FMath::FRand());
//							movespeed = FMath::RandRange(SLOWSPEED, SLOWSPEED + 1);
//							if (rightmovecounter++ > 5)
//							{
//								b_rb = false;
//								rightmovecounter = 0;
//							}
//
//						}
//						if (b_ub)
//						{
//							Owner->AddActorWorldOffset(-Owner->GetActorUpVector() * turnspeed * FMath::FRand());
//							movespeed = FMath::RandRange(SLOWSPEED, SLOWSPEED + 1);
//							if (rightmovecounter++ > 5)
//							{
//								b_ub = false;
//
//								upmovecounter = 0;
//							}
//						}
//						if (b_fb || b_rb || b_ub)
//						{
//						}
//						else
//						{
//							movespeed = NORMALSPEED;
//							roatationspeed = NORMALROTATION;
//							state = AIMoveStatus::normal;
//						}
//
//						break;
//					default:
//						break;
//					}
//					if (FVector::Dist(Owner->GetActorLocation(), AILeader->GetActorLocation()) > keepdistance)
//					{
//						Owner->AddActorWorldOffset(Owner->GetActorForwardVector() * movespeed);
//					}
//					else
//					{
//
//					}
//
//				}
//
//			);
//
//		}
//
//	}
//
//}
// Called every frame
void UFlockAIMoveToComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (AILeader)
	{
		//UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), AILeader->GetActorLocation());
		
		//UKismetMathLibrary::RLerp(Owner->GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), AILeader->GetActorLocation()),0.005f,true);
//		auto forwarddetected = [=]()
//		{
//
//#define FORWARDDIS 1.5
//				FVector starpoint = Owner->GetActorLocation();
//				FVector director = Owner->GetActorForwardVector();
//				FVector endpoint = starpoint + (ownerbound.X * FORWARDDIS * director);
//				FVector HalfSize = FVector(5, ownerbound.Y, ownerbound.Z);
//				TArray<AActor*> actorarray;
//				FHitResult hitresult;
//				bool b = UKismetSystemLibrary::BoxTraceSingle(this, starpoint, endpoint, HalfSize, director.Rotation(),
//					ETraceTypeQuery::TraceTypeQuery2, true, actorarray,
//					EDrawDebugTrace::Type::None, hitresult, true,
//					FLinearColor::Red, FLinearColor::Green, 5.0f);
//				if (b)
//				{
//					forwardactor = hitresult.Actor;
//					auto ft = forwardactor->GetComponentByClass(UFlockAIMoveToComponent::StaticClass());
//					auto ft1 = forwardactor->GetComponentByClass(UFlockAIEnemyComponent::StaticClass());
//					if (ft)
//					{
//						movespeed = SLOWSPEED;
//						roatationspeed = SLOWROTATION;
//
//						forwardblockgoalrotation = Owner->GetTransform().TransformRotation(FRotator(0, FMath::RandRange(-20, 2), FMath::RandRange(-20, 2)).Quaternion()).Rotator();
//						b_fb = true;
//					}
//					else if (ft1)
//					{
//						movespeed = FASTSPEED;
//						roatationspeed = FASTROTATION;
//
//						forwardblockgoalrotation = Owner->GetTransform().TransformRotation(FRotator(0, FMath::RandRange(-90, -30), FMath::RandRange(-90, -30)).Quaternion()).Rotator();
//						b_fb = true;
//					}
//
//				}
//		};
//		auto rightdetected = [=]() {
//
//#define RIGHTDIS 3
//
//				FVector starpoint = Owner->GetActorLocation();
//				FVector director = Owner->GetActorRightVector();
//				FVector endpoint = starpoint + (ownerbound.Y * RIGHTDIS * director);
//				FVector HalfSize = FVector(ownerbound.X, 5, ownerbound.Z);
//				TArray<AActor*> actorarray;
//				FHitResult hitresult;
//				bool b = UKismetSystemLibrary::BoxTraceSingle(this, starpoint, endpoint, HalfSize, Owner->GetActorForwardVector().Rotation(),
//					ETraceTypeQuery::TraceTypeQuery2, true, actorarray,
//					EDrawDebugTrace::Type::None, hitresult, true,
//					FLinearColor::Red, FLinearColor::Green, 5.0f);
//				if (b)
//				{
//					rightactor = hitresult.Actor;
//					auto ft = rightactor->GetComponentByClass(UFlockAIMoveToComponent::StaticClass());
//					if (ft)
//					{
//						movespeed = SLOWSPEED;
//						roatationspeed = SLOWROTATION;
//						b_rb = true;
//					}
//
//				}
//		};
//		auto updetected = [=]() {
//			{
//				/////////////////////////////////////////////////////////////////////////////////////	
//			////////////up
//#define UPDIS 3
//				FVector starpoint = Owner->GetActorLocation();
//				FVector director = Owner->GetActorUpVector();
//				FVector endpoint = starpoint + (ownerbound.Y * UPDIS * director);
//				FVector HalfSize = FVector(ownerbound.X, ownerbound.Y, 5);
//				TArray<AActor*> actorarray;
//				FHitResult hitresult;
//				bool b = UKismetSystemLibrary::BoxTraceSingle(this, starpoint, endpoint, HalfSize, Owner->GetActorForwardVector().Rotation(),
//					ETraceTypeQuery::TraceTypeQuery2, true, actorarray,
//					EDrawDebugTrace::Type::None, hitresult, true,
//					FLinearColor::Red, FLinearColor::Green, 5.0f);
//				if (b)
//				{
//					upactor = hitresult.Actor;
//					auto ft = upactor->GetComponentByClass(UFlockAIMoveToComponent::StaticClass());
//					if (ft)
//					{
//						movespeed = SLOWSPEED;
//						roatationspeed = SLOWROTATION;
//						b_ub = true;
//					}
//				}
//			}
//		};
		//forwarddetected();
		//rightdetected();
		//updetected();
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
		case AIMoveStatus::idle:
			Owner->SetActorRotation(
				UKismetMathLibrary::RLerp(
					Owner->GetActorRotation(),
					idlerotation,
					roatationspeed * 0.005f,
					true
				)
			);
			if (idlerotationcounter++ > 50)
			{
				idlerotationcounter = 0;
				movespeed = NORMALSPEED;
				roatationspeed = NORMALROTATION;
				state = AIMoveStatus::normal;
			}
			break;
		default:
			break;
		}
		if (state == AIMoveStatus::idle||FVector::Dist(Owner->GetActorLocation(), AILeader->GetActorLocation()) > keepdistance)
		{
			Owner->AddActorWorldOffset(Owner->GetActorForwardVector() * movespeed);
		}
		else
		{
			movespeed = SLOWSPEED;
			roatationspeed = SLOWROTATION;
			state = AIMoveStatus::idle;
			idlerotation = Owner->GetTransform().TransformRotation(FRotator(0, FMath::RandRange(-10, -2), FMath::RandRange(5, 10)).Quaternion()).Rotator();
		}

	}
	// ...
}

