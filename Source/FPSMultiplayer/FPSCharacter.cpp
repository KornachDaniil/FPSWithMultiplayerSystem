// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AFPSCharacter::AFPSCharacter()
{
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetMesh(), "Camera");
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySound2D(GetWorld(), SpawnSound);
	
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}

		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		if (EnhancedInputComponent != nullptr)
		{
			EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
			EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);
			EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &AFPSCharacter::Jump);
			EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &AFPSCharacter::StopJumping);
			EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Started, this, &AFPSCharacter::Sprint);
			EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Completed, this, &AFPSCharacter::StopSprinting);
		}
	}
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

//private:
void AFPSCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	MulticastPlaySoundAtLocation(LandedSound);
}

void AFPSCharacter::MulticastPlaySoundAtLocation_Implementation(USoundBase* Sound)
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound, GetActorLocation());
}

void AFPSCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);

		if (MovementVector.Y == -1.0f)
		{
			GetCharacterMovement()->MaxWalkSpeed = MoveBackward;
		}
		else if (MovementVector.Y == 0.0f || MovementVector.Y == -0.0f)
		{
			GetCharacterMovement()->MaxWalkSpeed = MoveRight;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = MoveForward;
		}
	}
}

void AFPSCharacter::Look(const FInputActionValue& Value)
{
	FVector2d LookVector = Value.Get<FVector2d>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void AFPSCharacter::Sprint(const FInputActionValue& Value)
{
	bIsSprinting = true;
	MoveForward = 800.0f;
	MoveBackward = 500.0f;
	MoveRight = 600.0f;
}

void AFPSCharacter::StopSprinting(const FInputActionValue& Value)
{
	bIsSprinting = false;
	MoveForward = 600.0f;
	MoveBackward = 300.0f;
	MoveRight = 500.0f;
}
