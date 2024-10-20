// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class UInputMappingContext;

UCLASS()
class FPSMULTIPLAYER_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sound")
	USoundBase* SpawnSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sound")
	USoundBase* DamageSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sound")
	USoundBase* LandedSound;
	
protected:
	// Sets default values for this character's properties
	AFPSCharacter();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category="Setup")
	UCameraComponent* FollowCamera;
	
private:
	virtual void Landed(const FHitResult& Hit) override;
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlaySoundAtLocation(USoundBase* Sound);
	
	
	// Функции для управления персонажем
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void StopSprinting(const FInputActionValue& Value);

	// Переменные для Sprint
	bool bIsSprinting = false;
	float MoveForward = 600.0f;
	float MoveBackward = 300.0f;
	float MoveRight = 500.0f;
	
	// Переменные InputAction
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UInputAction* MoveInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UInputAction* LookInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UInputAction* JumpInputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UInputAction* SprintInputAction;
};
