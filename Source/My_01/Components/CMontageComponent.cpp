#include "Components/CMontageComponent.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCMontageComponent::UCMontageComponent()
{
	DeadType = EDeadType::Max;
}

void UCMontageComponent::BeginPlay()
{
	Super::BeginPlay();

	ReadDataTable(ParkourTable, EDateTableType::Parkour);
	ReadDataTable(GrappleTable, EDateTableType::Grapple);
	ReadDataTable(LandTable, EDateTableType::Land);
	ReadDataTable(HittedTable, EDateTableType::Hitted);
	ReadDataTable(AvoidTable, EDateTableType::Avoid);
	ReadDataTable(DeadTable, EDateTableType::Dead);
	ReadDataTable(DashTable, EDateTableType::Dash);
	ReadDataTable(DoubleJumpTable, EDateTableType::DoubleJump);
	ReadDataTable(RidingTable, EDateTableType::Riding);

}

// Parkour

void UCMontageComponent::PlayGoUpWall()
{
	PlayParkourAnimMontage(EParkourMontageType::GoUpWall);
}

void UCMontageComponent::PlayJumpOver()
{
	PlayParkourAnimMontage(EParkourMontageType::JumpOver);
}

void UCMontageComponent::PlayClimb()
{
	PlayParkourAnimMontage(EParkourMontageType::Climb);
}

void UCMontageComponent::PlayJumpLeft()
{
	PlayParkourAnimMontage(EParkourMontageType::JumpLeft);
}

void UCMontageComponent::PlayJumpRight()
{
	PlayParkourAnimMontage(EParkourMontageType::JumpRight);
}

void UCMontageComponent::PlayTurnLeft()
{
	PlayParkourAnimMontage(EParkourMontageType::TurnLeft);
}

void UCMontageComponent::PlayTurnRight()
{
	PlayParkourAnimMontage(EParkourMontageType::TurnRight);
}

void UCMontageComponent::PlayJumpUp()
{
	PlayParkourAnimMontage(EParkourMontageType::JumpUp);
}

void UCMontageComponent::PlayWallRunRight()
{
	PlayParkourAnimMontage(EParkourMontageType::WallRunRight);
}

void UCMontageComponent::PlayWallRunLeft()
{
	PlayParkourAnimMontage(EParkourMontageType::WallRunLeft);
}

// Grapple

void UCMontageComponent::PlayGround()
{
	PlayGrappleAnimMontage(EGrappleMontageType::Ground);
}

void UCMontageComponent::PlayAir()
{
	PlayGrappleAnimMontage(EGrappleMontageType::Air);
}

// Land

void UCMontageComponent::PlayLand()
{
	PlayLandAnimMontage(ELandMontageType::Land);
}

// Hitted
void UCMontageComponent::PlayHitted()
{
	switch (HittedType)
	{
		case EHittedType::Left:
			PlayHittedAnimMontage(EHittedType::Left);
			break;
	
		case EHittedType::Right:
			PlayHittedAnimMontage(EHittedType::Right);
			break;
	
		case EHittedType::Knockback:
			PlayHittedAnimMontage(EHittedType::Knockback);
			break;
	
		case EHittedType::Knockdown:
			PlayHittedAnimMontage(EHittedType::Knockdown);
			break;

		case EHittedType::Arrow:
			PlayHittedAnimMontage(EHittedType::Arrow);
			break;

		case EHittedType::Parry:
			PlayHittedAnimMontage(EHittedType::Parry);
			break;

		case EHittedType::Up:
			PlayHittedAnimMontage(EHittedType::Up);
			break;

		case EHittedType::Shock_Sit:
			//CLog::Log("Hitted Shock_Sit");
			PlayHittedAnimMontage(EHittedType::Shock_Sit);
			break;

		case EHittedType::Shock_Stand:
			//CLog::Log("Hitted Shock_Stand");
			PlayHittedAnimMontage(EHittedType::Shock_Stand);
			break;

		case EHittedType::Block:
			PlayHittedAnimMontage(EHittedType::Block);
			break;
	
		case EHittedType::Max:
			break;
	
		default:
			break;
	}
}

// Avoid

void UCMontageComponent::PlayAvoid()
{
	PlayAvoidAnimMontage(EAvoidMontageType::Avoid);
}

// Dead

void UCMontageComponent::PlayDead()
{
	switch (DeadType)
	{

	case EDeadType::Dead:
		{
			PlayDeadAnimMontage(EDeadType::Dead);
		}
			break;

		case EDeadType::FlyDead:
		{
			PlayFlyDead();
		}
			break;

		default:
			break;
	}
}

void UCMontageComponent::PlayFlyDead()
{
	PlayDeadAnimMontage(EDeadType::FlyDead);
}

// Dash

void UCMontageComponent::PlayDash()
{
	PlayDashAnimMontage(EDashMontageType::Dash);
}

// Dual - DoubleJump

void UCMontageComponent::PlayDoubleJump()
{
	PlayDoubleJumpAnimMontage(EDoubleJumpMontageType::DoubleJump);
}

// Riding

void UCMontageComponent::PlayMountLeft()
{
	PlayRidingAnimMontage(ERidingMontageType::MountLeft);
}

void UCMontageComponent::PlayMountRight()
{
	PlayRidingAnimMontage(ERidingMontageType::MountRight);
}

void UCMontageComponent::PlayDismountLeft()
{
	PlayRidingAnimMontage(ERidingMontageType::DismountLeft);
}

void UCMontageComponent::PlayDismountRight()
{
	PlayRidingAnimMontage(ERidingMontageType::DismountRight);
}

void UCMontageComponent::ReadDataTable(UDataTable * InDataTable, EDateTableType InType)
{
	switch (InType)
	{
		case EDateTableType::Parkour:
			CheckNull(ParkourTable);
			{
				TArray<FParkourMontageData*> parkourDatas;
				InDataTable->GetAllRows<FParkourMontageData>("", parkourDatas);

				for (int32 i = 0; i < (int32)EParkourMontageType::Max; i++)
				{
					for (FParkourMontageData* data : parkourDatas)
					{
						if ((EParkourMontageType)i == data->MontageType)
						{
							ParkourMontageData[i] = data;
						} // if ((EVaultMontageType)i == data->MontageType)
					} // for (FVaultMontageData * data : datas)
				} // for (int32 i = 0; i < (int32)EVaultMontageType::Max; i++)
			}
				break;

		case EDateTableType::Grapple:
			CheckNull(GrappleTable);
			{
				TArray<FGrappleMontageData*> grappleDatas;
				InDataTable->GetAllRows<FGrappleMontageData>("", grappleDatas);

				for (int32 i = 0; i < (int32)EGrappleMontageType::Max; i++)
				{
					for (FGrappleMontageData* data : grappleDatas)
					{
						if ((EGrappleMontageType)i == data->MontageType)
						{
							GrappleMontageData[i] = data;
						}
					}
				}
			}
				break;

		case EDateTableType::Land:
			CheckNull(LandTable);
			{
				TArray<FLandMontageData*> landDatas;
				InDataTable->GetAllRows<FLandMontageData>("", landDatas);

				for (int32 i = 0; i < (int32)ELandMontageType::Max; i++)
				{
					for (FLandMontageData* data : landDatas)
					{
						if ((ELandMontageType)i == data->MontageType)
						{
							LandMontageData[i] = data;
						}
					}
				}
			}
				break;

		case EDateTableType::Hitted:
			CheckNull(HittedTable);
			{
				TArray<FHittedMontageData*> hittedDatas;
				InDataTable->GetAllRows<FHittedMontageData>("", hittedDatas);

				for (int32 i = 0; i < (int32)EHittedType::Max; i++)
				{
					for (FHittedMontageData* data : hittedDatas)
					{
						if ((EHittedType)i == data->MontageType)
						{
							HittedMontageData[i] = data;
						}
					}
				}
			}
				break;

		case EDateTableType::Avoid:
			CheckNull(AvoidTable);
			{
				TArray<FAvoidMontageData*> avoidDatas;
				InDataTable->GetAllRows<FAvoidMontageData>("", avoidDatas);

				for (int32 i = 0; i < (int32)EAvoidMontageType::Max; i++)
				{
					for (FAvoidMontageData* data : avoidDatas)
					{
						if ((EAvoidMontageType)i == data->MontageType)
						{
							AvoidMontageData[i] = data;
						}
					}
				}
			}
				break;

		case EDateTableType::Dead:
			CheckNull(DeadTable);
			{
				TArray<FDeadMontageData*> deadDatas;
				InDataTable->GetAllRows<FDeadMontageData>("", deadDatas);

				for (int32 i = 0; i < (int32)EDeadType::Max; i++)
				{
					for (FDeadMontageData* data : deadDatas)
					{
						if ((EDeadType)i == data->MontageType)
						{
							DeadMontageData[i] = data;
						}
					}
				}
			}
			break;

		case EDateTableType::Dash:
			CheckNull(DashTable);
			{
				TArray<FDashMontageData*> dashDatas;
				InDataTable->GetAllRows<FDashMontageData>("", dashDatas);

				for (int32 i = 0; i < (int32)EDashMontageType::Max; i++)
				{
					for (FDashMontageData* data : dashDatas)
					{
						if ((EDashMontageType)i == data->MontageType)
						{
							DashMontageData[i] = data;
						}
					}
				}
			}
			break;

		case EDateTableType::DoubleJump:
			CheckNull(DoubleJumpTable);
			{
				TArray<FDoubleJumpMontageData*> doubleJumpDatas;
				InDataTable->GetAllRows<FDoubleJumpMontageData>("", doubleJumpDatas);

				for (int32 i = 0; i < (int32)EDoubleJumpMontageType::Max; i++)
				{
					for (FDoubleJumpMontageData* data : doubleJumpDatas)
					{
						if ((EDoubleJumpMontageType)i == data->MontageType)
						{
							DoubleJumpMontageData[i] = data;
						}
					}
				}
			}
			break;

		case EDateTableType::Riding:
			CheckNull(RidingTable);
			{
				TArray<FRidingMontageData*> ridingDatas;
				InDataTable->GetAllRows<FRidingMontageData>("", ridingDatas);

				for (int32 i = 0; i < (int32)ERidingMontageType::Max; i++)
				{
					for (FRidingMontageData* data : ridingDatas)
					{
						if ((ERidingMontageType)i == data->MontageType)
						{
							RidingMontageData[i] = data;
						}
					}
				}
			}
			break;

		case EDateTableType::Max:
			break;
		
		default:
			break;
	}
}

void UCMontageComponent::PlayParkourAnimMontage(EParkourMontageType InType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FParkourMontageData * data = ParkourMontageData[(int32)InType];

	if (data != NULL)
	{
		if (data->AnimMontage != NULL)
		{
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
		}
	}
}

void UCMontageComponent::PlayGrappleAnimMontage(EGrappleMontageType InType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FGrappleMontageData* data = GrappleMontageData[(int32)InType];

	if (data != NULL)
	{
		if (data->AnimMontage != NULL)
		{
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
		}
	}
}

void UCMontageComponent::PlayLandAnimMontage(ELandMontageType InType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FLandMontageData* data = LandMontageData[(int32)InType];

	if (data != NULL)
	{
		if (data->AnimMontage != NULL)
		{
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
		}
	}
}

void UCMontageComponent::PlayHittedAnimMontage(EHittedType InType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FHittedMontageData* data = HittedMontageData[(int32)InType];

	if (data != NULL)
	{
		if (data->AnimMontage != NULL)
		{
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
		}
	}
}

void UCMontageComponent::PlayAvoidAnimMontage(EAvoidMontageType InType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FAvoidMontageData* data = AvoidMontageData[(int32)InType];

	if (data != NULL)
	{
		if (data->AnimMontage != NULL)
		{
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
		}
	}
}

void UCMontageComponent::PlayDeadAnimMontage(EDeadType InType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FDeadMontageData* data = DeadMontageData[(int32)InType];

	if (data != NULL)
	{
		if (data->AnimMontage != NULL)
		{
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
		}
	}
}

void UCMontageComponent::PlayDashAnimMontage(EDashMontageType InType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FDashMontageData* data = DashMontageData[(int32)InType];

	if (data != NULL)
	{
		if (data->AnimMontage != NULL)
		{
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
		}
	}
}

void UCMontageComponent::PlayDoubleJumpAnimMontage(EDoubleJumpMontageType InType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FDoubleJumpMontageData* data = DoubleJumpMontageData[(int32)InType];

	if (data != NULL)
	{
		if (data->AnimMontage != NULL)
		{
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
		}
	}
}

void UCMontageComponent::PlayRidingAnimMontage(ERidingMontageType InType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FRidingMontageData* data = RidingMontageData[(int32)InType];

	if (data != NULL)
	{
		if (data->AnimMontage != NULL)
		{
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
		}
	}
}

void UCMontageComponent::StopRidingAnimMontage(ERidingMontageType InType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FRidingMontageData* data = RidingMontageData[(int32)InType];

	if (data != NULL)
	{
		if (data->AnimMontage != NULL)
		{
			character->StopAnimMontage(data->AnimMontage);
		}
	}
}

void UCMontageComponent::SetHittedAndDeadType(EHittedType InHittedType, EDeadType InDeadType)
{
	HittedType = InHittedType;
	DeadType = InDeadType;
}

