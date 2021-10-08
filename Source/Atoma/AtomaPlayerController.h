// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AtomaPlayerController.generated.h"

UCLASS()
class AAtomaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAtomaPlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	/** True if the controlled character is controlled by keyboard. */
	bool bHasKeayboardInput = false;

	FVector Movedirection = FVector(0.0);

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Navigate player around the x axis. */
	void SetNewMoveDestinationForward(float Amount);

	/** Navigate player around the y axis. */
	void SetNewMoveDestinationRight(float Amount);

	/** Navigate player to the given world location. */
	void Move();

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
};


