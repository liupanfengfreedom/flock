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
	idle,
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLOCKAI_API UFlockAIMoveToComponent : public UActorComponent
{
	GENERATED_BODY()
	AActor* Owner;
#define mlti 1
#define FASTROTATION (4.1f*mlti)
#define NORMALROTATION (2.1f*mlti)
#define SLOWROTATION (1.2f*mlti)
	int roatationspeed = NORMALROTATION;
	float keepdistance;
#define FASTSPEED (3.3f*mlti)
#define NORMALSPEED (2.1f*mlti)
#define SLOWSPEED (1.2f*mlti)
	float movespeed= NORMALSPEED;
	FVector ownerbound;
	FVector aileaderbound;
	float turnspeed=1;
	AIMoveStatus state = AIMoveStatus::normal;
	FRotator forwardblockgoalrotation;
	FRotator idlerotation;
	int idlerotationcounter=0;
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
	//void threadwork();
	FTimerHandle th;
	void timerwork();
	FTimerHandle thf;
	void timerworkf();
	FTimerHandle thr;
	void timerworkr();
	FTimerHandle thu;
	void timerworku();
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
	//UFUNCTION(BlueprintCallable)
	//int getstate()
	//{
	//	return (int)(state);
	//}
	//UFUNCTION(BlueprintCallable)
	//	float getmovespeed()
	//{
	//	return movespeed;
	//}
};
