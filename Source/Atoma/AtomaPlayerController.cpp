// Copyright Epic Games, Inc. All Rights Reserved.

#include "AtomaPlayerController.h"

#include "AtomaCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"

AAtomaPlayerController::AAtomaPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AAtomaPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor) {
		MoveToMouseCursor();
	}

	if (bHasKeayboardInput) {
		Move();
	}
}

void AAtomaPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AAtomaPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AAtomaPlayerController::OnSetDestinationReleased);
	InputComponent->BindAxis("MoveForward", this, &AAtomaPlayerController::SetNewMoveDestinationForward);
	InputComponent->BindAxis("MoveRight", this, &AAtomaPlayerController::SetNewMoveDestinationRight);

	// support touch devices
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AAtomaPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AAtomaPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AAtomaPlayerController::OnResetVR);
}

void AAtomaPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}


void AAtomaPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled()) {
		if (AAtomaCharacter* MyPawn = Cast<AAtomaCharacter>(GetPawn())) {
			if (MyPawn->GetCursorToWorld()) {
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else {
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit) {
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AAtomaPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit) {
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void AAtomaPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn) {
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f)) {
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AAtomaPlayerController::SetNewMoveDestinationForward(float Amount)
{
	Movedirection += FVector(Amount, 0.0, 0.0);
	bHasKeayboardInput = true;
}

void AAtomaPlayerController::SetNewMoveDestinationRight(float Amount)
{
	Movedirection += FVector(0.0, Amount, 0.0);
	bHasKeayboardInput = true;
}

void AAtomaPlayerController::Move()
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn) {
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetActorLocation() + (Movedirection.GetSafeNormal() * 100));
	}
	Movedirection = FVector(0.0);
	bHasKeayboardInput = false;
}


void AAtomaPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AAtomaPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}
