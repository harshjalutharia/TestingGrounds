// Fill out your copyright notice in the Description page of Project Settings.

#include "Mannequin.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Weapons/Gun.h"


// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FPCamera->SetupAttachment(RootComponent);
	FPCamera->bUsePawnControlRotation = true;
	FPCamera->RelativeLocation = FVector(-39.56f, 1.75f, 64.f);

	FPArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPArms"));
	FPArms->SetupAttachment(FPCamera);
	FPArms->SetOnlyOwnerSee(true);
	FPArms->bCastDynamicShadow = false;
	FPArms->CastShadow = false;
	FPArms->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	FPArms->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	if (GunBlueprint == NULL) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Gun blueprint missing."));
		return;
	}
	FPGun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	if (FPGun != NULL)
	{
		if (IsPlayerControlled())
		{
			FPGun->AttachToComponent(FPArms, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
			FPGun->AnimInstance1P = FPArms->GetAnimInstance();
			FPGun->AnimInstance3P = nullptr;
		}
		else
		{
			FPGun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
			FPGun->AnimInstance3P = GetMesh()->GetAnimInstance();
			FPGun->AnimInstance1P = nullptr;
		}
	}
	if (InputComponent != NULL)
	{
		InputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);
	}
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMannequin::UnPossessed()
{
	Super::UnPossessed();
	if(FPGun != nullptr)
		FPGun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

void AMannequin::PullTrigger()
{
	if(FPGun != nullptr)
		FPGun->OnFire();
}