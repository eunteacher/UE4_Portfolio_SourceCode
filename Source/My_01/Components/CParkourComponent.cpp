#include "Components/CParkourComponent.h"
#include "Global.h"

#include "Characters/IParkour.h"

#include "Animation/AnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CMontageComponent.h"

UCParkourComponent::UCParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
}

void UCParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	if (Jumping == false)
	{
		ForwardTrace();
		HeightTrace();
	}
	
	if (Hanging == true)
	{
		FVector rightStartLocation = RightTrace();
		FVector leftStartLocation = LeftTrace();
		JumpUpTrace();

		MoveLeftRigt();

		if (CanMoveLeft == true)
		{
			CanJumpLeft = false;
			CanTurnLeft = false;
		}
		else
		{

			JumpLeftTrace();

			if (CanJumpLeft == true)
			{
				CanTurnLeft = false;
			}
			else
			{
				TurnLeftTrace(leftStartLocation);
			}

			return;
		}

		if (CanMoveRight == true)
		{
			CanJumpRight = false;
			CanTurnRight = false;
		}
		else
		{
			JumpRightTrace();

			if (CanJumpRight == true)
			{
				CanTurnRight = false;
			}
			else
			{
				TurnRightTrace(rightStartLocation);
			}
		}
	}

}

void UCParkourComponent::ForwardTrace()
{
	FVector start;
	start = OwnerCharacter->GetActorLocation();
	FVector end;
	end = start + FVector(OwnerCharacter->GetActorForwardVector().X * 200.0f, OwnerCharacter->GetActorForwardVector().Y * 200.0f, OwnerCharacter->GetActorForwardVector().Z);

	TArray<AActor*> ActorToIgnore;
	FHitResult hit;

	bool result = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		start,
		end,
		20.0f,
		TraceType,
		true,
		ActorToIgnore,
		DrawDebugType,
		hit,
		true
	);

	if (result == true)
	{
		WallLocation = hit.Location;
		WallNormal = hit.Normal;

		JumpOverTrace(WallLocation);
	}
}

void UCParkourComponent::HeightTrace()
{
	FVector start;
	FVector up = FVector(OwnerCharacter->GetActorLocation().X, OwnerCharacter->GetActorLocation().Y, OwnerCharacter->GetActorLocation().Z + HeightTraceLength);
	FVector forward = OwnerCharacter->GetActorForwardVector() * 70.0f;
	start = up + forward;
	FVector end;
	end = FVector(start.X, start.Y, start.Z - HeightTraceLength);

	TArray<AActor*> ActorToIgnore;
	FHitResult hit;

	bool result = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		start,
		end,
		20.0f,
		TraceType,
		true,
		ActorToIgnore,
		DrawDebugType,
		hit,
		true
	);

	if (result == true)
	{
		WallHeight = hit.Location;
		HangingWall = WallHeight;

		if (HipToLedge() == true)
		{
			if (bClimbLedge == false)
			{
				//CLog::Log("Height Grab");
				GrabLedge();
			}
		}
		else
		{
			//CLog::Log("HipToLedge() == false");
			CheckTrue(Hanging);

			float height = WallHeight.Z - WallLocation.Z;
			//CLog::Log(height);
			if (height > HeightLimit)
			{
				//CLog::Log("bGoUpWall false");
				bGoUpWall = false;
			}
			else
			{
				//CLog::Log("bGoUpWall true");
				FVector2D v1 = FVector2D(OwnerCharacter->GetActorLocation().X, OwnerCharacter->GetActorLocation().Y);
				FVector2D v2 = FVector2D(WallLocation.X, WallLocation.Y);
				float distacne = UKismetMathLibrary::Distance2D(v1, v2);

				//CLog::Log(distacne);
				if (distacne < DistacneOffset)
				{
					//CLog::Log("bGoUpWall True");
					bGoUpWall = true;
				}
				else
				{
					bGoUpWall = false;
				}
			} // else
			
		}
		
	}
}

FVector UCParkourComponent::RightTrace()
{
	FVector start = (OwnerCharacter->GetActorLocation()) + (OwnerCharacter->GetActorRightVector() * 50.0f)
		+ (OwnerCharacter->GetActorForwardVector() * 40.0f) + (OwnerCharacter->GetActorUpVector() * 40.0f);
	FVector end = start + OwnerCharacter->GetActorUpVector();

	TArray<AActor*> ActorsToIgnore;
	FHitResult hit;

	bool result = UKismetSystemLibrary::CapsuleTraceSingle
	(
		GetWorld(),
		start,
		end,
		20.0f,
		60.0f,
		TraceType,
		true,
		ActorsToIgnore,
		DrawDebugType,
		hit,
		true
	);


	if (result == true)
	{
		CanMoveRight = true;
	}
	else
	{
		CanMoveRight = false;
	}

	return start;
}

FVector UCParkourComponent::LeftTrace()
{
	FVector start = (OwnerCharacter->GetActorLocation()) - (OwnerCharacter->GetActorRightVector() * 50.0f)
		+ (OwnerCharacter->GetActorForwardVector() * 40.0f) + (OwnerCharacter->GetActorUpVector() * 40.0f);
	FVector end = start + OwnerCharacter->GetActorUpVector();

	TArray<AActor*> ActorsToIgnore;
	FHitResult hit;

	bool result = UKismetSystemLibrary::CapsuleTraceSingle
	(
		GetWorld(),
		start,
		end,
		20.0f,
		60.0f,
		TraceType,
		true,
		ActorsToIgnore,
		DrawDebugType,
		hit,
		true
	);


	if (result == true)
	{
		CanMoveLeft = true;
	}
	else
	{
		CanMoveLeft = false;
	}

	return start;
}

void UCParkourComponent::JumpLeftTrace()
{
	FVector start = (OwnerCharacter->GetActorLocation()) - (OwnerCharacter->GetActorRightVector() * 150.0f)
		+ (OwnerCharacter->GetActorForwardVector() * 40.0f) + (OwnerCharacter->GetActorUpVector() * 40.0f);
	FVector end = start + OwnerCharacter->GetActorUpVector();
	
	TArray<AActor*> ActorsToIgnore;
	FHitResult hit;

	bool result = UKismetSystemLibrary::CapsuleTraceSingle
	(
		GetWorld(),
		start,
		end,
		25.0f,
		60.0f,
		TraceType,
		true,
		ActorsToIgnore,
		DrawDebugType,
		hit,
		true
	);

	if (result == true)
	{
		if (CanMoveLeft == true)
		{
			CanJumpLeft = false;
		}
		else
		{
			CanJumpLeft = true;
			CheckTrue(CanJumpUp);

			JumpForwardTrace(start);
			JumpHeightTrace(start);
		}
	}
	else
	{
		CanJumpLeft = false;
	}
}

void UCParkourComponent::JumpRightTrace()
{
	FVector start = (OwnerCharacter->GetActorLocation()) + (OwnerCharacter->GetActorRightVector() * 150.0f)
		+ (OwnerCharacter->GetActorForwardVector() * 40.0f) + (OwnerCharacter->GetActorUpVector() * 40.0f);
	FVector end = start + OwnerCharacter->GetActorUpVector();

	TArray<AActor*> ActorsToIgnore;
	FHitResult hit;

	bool result = UKismetSystemLibrary::CapsuleTraceSingle
	(
		GetWorld(),
		start,
		end,
		25.0f,
		60.0f,
		TraceType,
		true,
		ActorsToIgnore,
		DrawDebugType,
		hit,
		true
	);

	if (result == true)
	{
		if (CanMoveRight == true)
		{
			CanJumpRight = false;
		}
		else
		{
			CanJumpRight = true;
			CheckTrue(CanJumpUp);
			
			JumpForwardTrace(start);
			JumpHeightTrace(start);
		}
	}
	else
	{
		//CLog::Print("jumpMoveLeft false");
		CanJumpRight = false;
	}
}

void UCParkourComponent::JumpForwardTrace(FVector InStart)
{
	FVector start;
	start = InStart;
	FVector end;
	end = start + FVector(OwnerCharacter->GetActorForwardVector().X * 150.0f, OwnerCharacter->GetActorForwardVector().Y * 150.0f, OwnerCharacter->GetActorForwardVector().Z);

	TArray<AActor*> ActorToIgnore;
	FHitResult hit;

	bool result = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		start,
		end,
		20.0f,
		TraceType,
		true,
		ActorToIgnore,
		DrawDebugType,
		hit,
		true
	);

	if (result == true)
	{
		WallLocation = hit.Location;
		WallNormal = hit.Normal;
	}
}

void UCParkourComponent::JumpHeightTrace(FVector InStart)
{
	FVector start;
	FVector up = FVector(InStart.X, InStart.Y, InStart.Z + HeightTraceLength);
	FVector forward = OwnerCharacter->GetActorForwardVector() * 35.0f;
	start = up + forward;
	FVector end;
	end = FVector(start.X, start.Y, start.Z - HeightTraceLength);

	TArray<AActor*> ActorToIgnore;
	FHitResult hit;

	bool result = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		start,
		end,
		20.0f,
		TraceType,
		true,
		ActorToIgnore,
		DrawDebugType,
		hit,
		true
	);

	if (result == true)
	{
		WallHeight = hit.Location;
		JumpingWall = WallHeight;
	}
}

void UCParkourComponent::TurnLeftTrace(FVector InStart)
{
	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();

	FVector start = FVector(InStart.X, InStart.Y, InStart.Z + 60.0f);
	FVector end = start + (OwnerCharacter->GetActorForwardVector() * 70.0f);

	TArray<AActor*> ActorsToIgnore;
	FHitResult hit;

	bool result = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		start,
		end,
		20.0f,
		TraceType,
		true,
		ActorsToIgnore,
		DrawDebugType,
		hit,
		true
	);

	if (result == true)
	{
		CanTurnLeft = false;
	}
	else
	{
		CanTurnLeft = true;
	}

}

void UCParkourComponent::TurnRightTrace(FVector InStart)
{
	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();

	FVector start = FVector(InStart.X, InStart.Y, InStart.Z + 60.0f);
	FVector end = start + (OwnerCharacter->GetActorForwardVector() * 70.0f);

	TArray<AActor*> ActorsToIgnore;
	FHitResult hit;

	bool result = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		start,
		end,
		20.0f,
		TraceType,
		true,
		ActorsToIgnore,
		DrawDebugType,
		hit,
		true
	);

	if (result == true)
	{
		CanTurnRight = false;
	}
	else
	{
		CanTurnRight = true;
	}
}

void UCParkourComponent::JumpUpTrace()
{
	FVector start = FVector(OwnerCharacter->GetActorLocation().X, OwnerCharacter->GetActorLocation().Y, OwnerCharacter->GetActorLocation().Z + 300.0f)
		+ (OwnerCharacter->GetActorForwardVector() * 70.0f);
	FVector end = start + OwnerCharacter->GetActorForwardVector();

	TArray<AActor*> ActorsToIgnore;
	FHitResult hit;

	bool result = UKismetSystemLibrary::CapsuleTraceSingle
	(
		GetWorld(),
		start,
		end,
		20.0f,
		75.0f,
		TraceType,
		true,
		ActorsToIgnore,
		DrawDebugType,
		hit,
		true
	);

	if (result == true)
	{
		CanJumpUp = true;
		JumpUpForwardTrace(start, hit.Location);
	}
	else
	{
		CanJumpUp = false;
	}

}

void UCParkourComponent::JumpOverTrace(FVector InStart)
{
	FVector start = FVector(InStart.X, InStart.Y, InStart.Z + 150.0f);
	FVector end;
	end = FVector(start.X, start.Y, start.Z - 200.0f);

	TArray<AActor*> ActorToIgnore;
	FHitResult hit;

	bool result = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		start,
		end,
		20.0f,
		TraceType,
		true,
		ActorToIgnore,
		DrawDebugType,
		hit,
		true
	);

	if (result == true)
	{
		WallJumOverHeight = hit.Location;
		WallJumOverNormal = hit.Normal;

		if (HipToLedge() == false)
		{
			CheckTrue(Hanging);

			//CLog::Log("BehindTrace");
			BehindTrace(WallJumOverHeight);
		}

	}
}

void UCParkourComponent::BehindTrace(FVector InStart)
{
	FVector start = InStart + OwnerCharacter->GetActorForwardVector() * BehindOffset;
	start.Z += 250.0f;
	FVector end = start;
	end.Z -= 400.0f;

	TArray<AActor*> ActorToIgnore;
	FHitResult hit;

	bool result = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		start,
		end,
		20.0f,
		TraceType,
		true,
		ActorToIgnore,
		DrawDebugType,
		hit,
		true
	);

	if (result == true)
	{
		WallBehindLocation = hit.Location;

		float height = WallJumOverHeight.Z - WallBehindLocation.Z;

		//CLog::Log(height);

		if (height > 0.0f)
		{
			FVector2D v1 = FVector2D(OwnerCharacter->GetActorLocation().X, OwnerCharacter->GetActorLocation().Y);
			FVector2D v2 = FVector2D(WallLocation.X, WallLocation.Y);
			float distacne = UKismetMathLibrary::Distance2D(v1, v2);

			//CLog::Log(distacne);

			if (distacne > JumpOverDistacneOffset)
			{
				bJumpOver = true;
			}
			else
			{
				bJumpOver = false;
			}

		}
		else
		{
			//CLog::Log("bJumpOver false");
			bJumpOver = false;
		}

	}
}

void UCParkourComponent::JumpUpForwardTrace(FVector InStart, FVector InHitLoaction)
{
	FVector start = FVector(OwnerCharacter->GetActorLocation().X, OwnerCharacter->GetActorLocation().Y, InStart.Z - 60.0f);
	FVector end = start + FVector(OwnerCharacter->GetActorForwardVector().X * 150.0f, OwnerCharacter->GetActorForwardVector().Y * 150.0f, OwnerCharacter->GetActorForwardVector().Z);

	TArray<AActor*> ActorsToIgnore;
	FHitResult hit;

	bool result = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		start,
		end,
		20.0f,
		TraceType,
		true,
		ActorsToIgnore,
		DrawDebugType,
		hit,
		true
	);

	if (result == true)
	{
		CheckTrue(Jumping);
		WallHeight = InHitLoaction;
		WallLocation = hit.Location;
		WallNormal = hit.Normal;
	}
}

bool UCParkourComponent::HipToLedge()
{
	FVector socketLoaction = OwnerCharacter->GetMesh()->GetSocketLocation(SocketName);
	float height = socketLoaction.Z - WallHeight.Z;

	//CLog::Log(height);

	return  UKismetMathLibrary::InRange_FloatFloat(height, MinPelvisSubtractWallHeight, MaxPelvisSubtractWallHeight);
}

void UCParkourComponent::GrabLedge()
{
	//CLog::Log("GrabLedge");
	IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
	CheckNull(parkour);
	parkour->CanGrab(true);

	State->SetParkourMode();

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	Hanging = true;

	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.UUID = GetUniqueID();
	info.ExecutionFunction = "MoveToGrabLedge";
	info.Linkage = 0;

	UKismetSystemLibrary::MoveComponentTo
	(
		OwnerCharacter->GetCapsuleComponent(),
		MoveToLocation(),
		LookAtWall(),
		false,
		false,
		0.13f,
		false,
		EMoveComponentAction::Move,
		info
	);

}

void UCParkourComponent::JumpLeftGrabLedge()
{
	float right = RightAxis();

	if (right < 0.0f && Jumping == false)
	{
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

		IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
		CheckNull(parkour);
		parkour->JumpLeft();


		Jumping = true;
		Hanging = false;

		UKismetSystemLibrary::K2_SetTimer(this, "GrabLedge", 0.8f, false);

	}
}

void UCParkourComponent::JumpRightGrabLedge()
{
	float right = RightAxis();

	if (right > 0.0f && Jumping == false)
	{
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

		IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
		CheckNull(parkour);
		parkour->JumpRight();

		Jumping = true;
		Hanging = false;

		UKismetSystemLibrary::K2_SetTimer(this, "GrabLedge", 0.9f, false);
	}
}

void UCParkourComponent::MoveInLedge()
{
	float right = RightAxis();

	if (CanMoveRight == true)
	{
		if (right > 0)
		{
			FVector newLocation;
			FVector currentLocation = OwnerCharacter->GetActorLocation();
			FVector targetLocation = (UKismetMathLibrary::GetRightVector(OwnerCharacter->GetActorRotation()) * 20.0f) + currentLocation;

			newLocation = UKismetMathLibrary::VInterpTo(currentLocation, targetLocation, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 5.0f);

			OwnerCharacter->SetActorLocation(newLocation);

			MovingRight = true;
			MovingLeft = false;

		}

	}

	if (CanMoveLeft == true)
	{
		if (right < 0)
		{
			FVector newLocation;
			FVector currentLocation = OwnerCharacter->GetActorLocation();
			FVector targetLocation = (UKismetMathLibrary::GetRightVector(OwnerCharacter->GetActorRotation()) * -20.0f) + currentLocation;

			newLocation = UKismetMathLibrary::VInterpTo(currentLocation, targetLocation, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 5.0f);

			OwnerCharacter->SetActorLocation(newLocation);

			MovingLeft = true;
			MovingRight = false;

		}
	}

	if (UKismetMathLibrary::EqualEqual_FloatFloat(right, 0.0f))
	{
		MovingLeft = false;
		MovingRight = false;
	}
}

FVector UCParkourComponent::MoveToLocation()
{
	return FVector((WallNormal.X * 22.0f) + WallLocation.X, (WallNormal.Y * 22.0f) + WallLocation.Y, WallHeight.Z + HeightOffset);
}

FRotator UCParkourComponent::LookAtWall()
{
	FRotator result = WallNormal.Rotation();
	result.Yaw -= 180.0f;

	return result;
}

void UCParkourComponent::MoveToGrabLedge()
{
	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
}

void UCParkourComponent::OffParkour()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
	CheckNull(parkour);
	parkour->CanGrab(false);

	State->SetIdleMode();

	Hanging = false;
}

void UCParkourComponent::ClimbLedge()
{
	CheckTrue(bClimbLedge == true);
	CheckTrue(TurnBack == true);

	IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
	CheckNull(parkour);
	parkour->ClimbLedge(false);

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	bClimbLedge = true;
	Hanging = false;
	State->SetIdleMode();
}

void UCParkourComponent::EndClimbLedge()
{
	bClimbLedge = false;
	bGoUpWall = false;
	bJumpOver = false;

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

float UCParkourComponent::RightAxis()
{
	UInputComponent* input = CHelpers::GetComponent<UInputComponent>(OwnerCharacter);
	return input->GetAxisValue(FName(TEXT("MoveRight")));
}

void UCParkourComponent::MoveLeftRigt()
{
	CheckTrue(TurnBack);

	float right = RightAxis();

	if (Hanging == true)
	{
		IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
		CheckNull(parkour);
		parkour->MoveLeftRight(right);
		MoveInLedge();
	}
	else
	{
		MovingLeft = false;
		MovingRight = false;
	}
}

void UCParkourComponent::EndJumpLeftRight()
{
	//OwnerCharacter->GetController()->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	OwnerCharacter->GetController()->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();

	Jumping = false;

	//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void UCParkourComponent::TurnParkour()
{
	CheckFalse(Hanging);

	if (CanJumpLeft == false && CanTurnLeft == true)
	{
		OwnerCharacter->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		
		IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
		CheckNull(parkour);
		parkour->TurnLeft();

		UKismetSystemLibrary::K2_SetTimer(this, "GrabLedge", 0.6f, false);
		UKismetSystemLibrary::K2_SetTimer(this, "CallTimer", 0.6f, false);

		OwnerCharacter->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}

	if (CanJumpRight == false && CanTurnRight == true)
	{
		OwnerCharacter->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		
		IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
		CheckNull(parkour);
		parkour->TurnRight();
		
		UKismetSystemLibrary::K2_SetTimer(this, "GrabLedge", 0.6f, false);
		UKismetSystemLibrary::K2_SetTimer(this, "CallTimer", 0.6f, false);

		OwnerCharacter->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
}

void UCParkourComponent::JumpUpLedge()
{
	CheckFalse(UKismetMathLibrary::EqualEqual_FloatFloat(RightAxis(), 0.0f));
	CheckFalse(CanJumpUp);
	CheckTrue(Jumping);

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
	CheckNull(parkour);
	parkour->JumpUp();

	Jumping = true;

	OwnerCharacter->GetController()->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

}

void UCParkourComponent::EndJumpUpLedge()
{
	GrabLedge();
	Jumping = false;
	OwnerCharacter->GetController()->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void UCParkourComponent::CallTimer()
{
	// Call Set_Timer Func 
}

void UCParkourComponent::TurnedBack()
{
	CheckFalse(State->IsParkour());
	CheckFalse(Hanging == true);

	if (TurnBack == true)
	{
		TurnBack = false;

		IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
		CheckNull(parkour);
		parkour->TurnedBack(false);

		OffParkour();

		return;
	}

	if (TurnBack == false)
	{
		TurnBack = true;

		IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
		CheckNull(parkour);
		parkour->TurnedBack(true);

	}

}

void UCParkourComponent::TurnedForward()
{
	CheckFalse(State->IsParkour());
	CheckFalse(Hanging == true);
	CheckFalse(TurnBack == true);

	TurnBack = false;

	IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
	CheckNull(parkour);
	parkour->TurnedBack(false);

}

void UCParkourComponent::TurnedJump()
{
	CheckFalse(State->IsParkour());
	CheckFalse(Hanging == true);
	CheckFalse(TurnBack == true);
	CheckTrue(Jumping == true);

	FVector direction = OwnerCharacter->GetActorForwardVector() * -500.0f;
	direction.Z += 700.0f;
	OwnerCharacter->LaunchCharacter(direction, false, false);

	OffParkour();

	IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
	CheckNull(parkour);
	parkour->TurnedBack(false);

	TurnBack = false;

	FRotator rotator = OwnerCharacter->GetActorRotation();
	rotator.Yaw -= 180.0f;
	OwnerCharacter->SetActorRotation(FQuat(rotator));
}

void UCParkourComponent::GoUpWall()
{
	CheckFalse(State->IsIdle());

	State->SetParkourMode();
	
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FRotator rotator = UKismetMathLibrary::MakeRotFromX(WallNormal);
	FVector location = UKismetMathLibrary::GetForwardVector(rotator);
	location *= 50.0f;
	FVector startLocation = OwnerCharacter->GetActorLocation() + location;

	OwnerCharacter->SetActorLocation(startLocation);

	IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
	CheckNull(parkour);
	parkour->GoUpWall();

	UKismetSystemLibrary::K2_SetTimer(this, "Restore", 0.5f, false);
}

void UCParkourComponent::JumpOver()
{
	CheckFalse(State->IsIdle());

	State->SetParkourMode();
	
	IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
	CheckNull(parkour);
	parkour->JumpOver();

	//OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	FVector nextLocation = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * 50.0f);
	nextLocation.Z += JumpOverHeight;

	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.UUID = GetUniqueID();
	info.ExecutionFunction = "MoveToBehindLocation";
	info.Linkage = 0;

	UKismetSystemLibrary::MoveComponentTo
	(
		OwnerCharacter->GetCapsuleComponent(),
		nextLocation,
		OwnerCharacter->GetActorRotation(),
		false,
		false,
		0.2f,
		false,
		EMoveComponentAction::Move,
		info
	);

}

void UCParkourComponent::MoveToBehindLocation()
{
	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.UUID = GetUniqueID();
	info.ExecutionFunction = "LastMoveTo";
	info.Linkage = 0;

	FVector nextLocation = WallBehindLocation;

	nextLocation.Z += 140.0f;

	UKismetSystemLibrary::MoveComponentTo
	(
		OwnerCharacter->GetCapsuleComponent(),
		nextLocation,
		OwnerCharacter->GetActorRotation(),
		false,
		false,
		0.5f,
		false,
		EMoveComponentAction::Move,
		info
	);
}


void UCParkourComponent::LastMoveTo()
{
	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.UUID = GetUniqueID();
	info.ExecutionFunction = "Restore";
	info.Linkage = 0;

	FVector nextLocation = WallBehindLocation + (OwnerCharacter->GetActorForwardVector() * 30.0f);
	nextLocation.Z += OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.3f);

	UKismetSystemLibrary::MoveComponentTo
	(
		OwnerCharacter->GetCapsuleComponent(),
		nextLocation,
		OwnerCharacter->GetActorRotation(),
		false,
		false,
		0.1f,
		false,
		EMoveComponentAction::Move,
		info
	);
}

void UCParkourComponent::Restore()
{
	CLog::Log("Restore");
	OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	bGoUpWall = false;
	bJumpOver = false;
	
	State->SetIdleMode();

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}


void UCParkourComponent::WallRun()
{
	CheckFalse(bCanWallRun);

	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.UUID = GetUniqueID();
	info.ExecutionFunction = "StopMoveToWallRun";
	info.Linkage = 0;

	if (bRight == true)
	{
		IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
		CheckNull(parkour);
		parkour->WallRunRight();
	}
	else
	{
		IIParkour* parkour = Cast<IIParkour>(OwnerCharacter->GetMesh()->GetAnimInstance());
		CheckNull(parkour);
		parkour->WallRunLeft();
	}

	UKismetSystemLibrary::MoveComponentTo
	(
		OwnerCharacter->GetCapsuleComponent(),
		StartTransform.GetLocation(),
		StartTransform.GetRotation().Rotator(),
		false,
		false,
		0.1f,
		false,
		EMoveComponentAction::Move,
		info
	);

	UKismetSystemLibrary::MoveComponentTo
	(
		OwnerCharacter->GetCapsuleComponent(),
		TargetLocation,
		StartTransform.GetRotation().Rotator(),
		false,
		false,
		1.25f,
		false,
		EMoveComponentAction::Move,
		info
	);

}

void UCParkourComponent::StopMoveToWallRun()
{
	// ExecutionFunction
}

void UCParkourComponent::SetRunnableWall(bool Enable, bool IsRight, FVector Target, FTransform Start)
{
	bCanWallRun = Enable;
	TargetLocation = Target;
	StartTransform = Start;
	bRight = IsRight;

}
