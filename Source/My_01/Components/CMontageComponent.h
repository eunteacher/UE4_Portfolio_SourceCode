#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CMontageComponent.generated.h"

UENUM(BlueprintType)
enum class EParkourMontageType : uint8
{
	GoUpWall, JumpOver, Climb, JumpLeft, JumpRight, TurnLeft, TurnRight, JumpUp, WallRunRight, WallRunLeft, Max
};

USTRUCT(BlueprintType)
struct FParkourMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EParkourMontageType MontageType;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;
};

UENUM(BlueprintType)
enum class EGrappleMontageType : uint8
{
	Ground, Air, Max
};

USTRUCT(BlueprintType)
struct FGrappleMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EGrappleMontageType MontageType;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;
};

UENUM(BlueprintType)
enum class EHittedType : uint8
{
	Left, Right, Knockback, Knockdown, Arrow, Parry, Up, Shock_Sit, Shock_Stand, Block, Max
};

USTRUCT(BlueprintType)
struct FHittedMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EHittedType MontageType;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;
};

UENUM(BlueprintType)
enum class EDeadType : uint8
{
	Takedown, Dead, FlyDead, Max
};

USTRUCT(BlueprintType)
struct FDeadMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EDeadType MontageType;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;
};

UENUM(BlueprintType)
enum class ELandMontageType : uint8
{
	Land, Max
};

USTRUCT(BlueprintType)
struct FLandMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		ELandMontageType MontageType;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;
};

UENUM(BlueprintType)
enum class EAvoidMontageType : uint8
{
	Avoid, Max
};

USTRUCT(BlueprintType)
struct FAvoidMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EAvoidMontageType MontageType;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;
};

UENUM(BlueprintType)
enum class EDashMontageType : uint8
{
	Dash, Max
};

USTRUCT(BlueprintType)
struct FDashMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EDashMontageType MontageType;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;
};

UENUM(BlueprintType)
enum class EDoubleJumpMontageType : uint8
{
	DoubleJump, Max
};

USTRUCT(BlueprintType)
struct FDoubleJumpMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EDoubleJumpMontageType MontageType;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;
};

UENUM(BlueprintType)
enum class ERidingMontageType : uint8
{
	MountLeft, MountRight, DismountLeft, DismountRight, Max
};

USTRUCT(BlueprintType)
struct FRidingMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		ERidingMontageType MontageType;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;
};

UENUM(BlueprintType)
enum class EDateTableType : uint8
{
	Parkour, Grapple, Land, Hitted, Avoid, Dead, Dash, DoubleJump, Riding, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_01_API UCMontageComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* ParkourTable;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* GrappleTable;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* LandTable;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* HittedTable;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* AvoidTable;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* DeadTable;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* DashTable;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* DoubleJumpTable;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* RidingTable;

public:
	FORCEINLINE EDeadType GetDeadType() { return DeadType; }

public:	
	UCMontageComponent();

protected:
	virtual void BeginPlay() override;
	
public:
	// Parkour
	// GetUp, JumpOver, Climb, JumpLeft, JumpRight, TurnLeft, TurnRight, JumpUp,
	void PlayGoUpWall();
	void PlayJumpOver();
	void PlayClimb();
	void PlayJumpLeft();
	void PlayJumpRight();
	void PlayTurnLeft();
	void PlayTurnRight();
	void PlayJumpUp();
	void PlayWallRunRight();
	void PlayWallRunLeft();

	// Grapple
	// 	Ground, Air, Max
	void PlayGround();
	void PlayAir();

	// Land
	// Land, Max
	void PlayLand();

	// Hitted
	// Left, Right, Knockback, Knockdown, Max
	void PlayHitted();

	// Avoid
	void PlayAvoid();

	// Dead
	void PlayDead();
	void PlayFlyDead();

	// Dash
	void PlayDash();

	// Dual - DoubleJump
	void PlayDoubleJump();

	// Riding
	void PlayMountLeft();
	void PlayMountRight();
	void PlayDismountLeft();
	void PlayDismountRight();

private:
	void ReadDataTable(UDataTable * InDataTable, EDateTableType InType);

	void PlayParkourAnimMontage(EParkourMontageType InType);
	void PlayGrappleAnimMontage(EGrappleMontageType InType);
	void PlayLandAnimMontage(ELandMontageType InType);
	void PlayHittedAnimMontage(EHittedType InType);
	void PlayAvoidAnimMontage(EAvoidMontageType InType);
	void PlayDeadAnimMontage(EDeadType InType);
	void PlayDashAnimMontage(EDashMontageType InType);
	void PlayDoubleJumpAnimMontage(EDoubleJumpMontageType InType);
	void PlayRidingAnimMontage(ERidingMontageType InType);

	void StopRidingAnimMontage(ERidingMontageType InType);

public:
	void SetHittedAndDeadType(EHittedType InHittedType, EDeadType InDeadType);

private:
	FParkourMontageData* ParkourMontageData[(int32)EParkourMontageType::Max];
	FGrappleMontageData* GrappleMontageData[(int32)EGrappleMontageType::Max];
	FLandMontageData* LandMontageData[(int32)ELandMontageType::Max];
	FHittedMontageData* HittedMontageData[(int32)EHittedType::Max];
	FAvoidMontageData* AvoidMontageData[(int32)EAvoidMontageType::Max];
	FDeadMontageData* DeadMontageData[(int32)EDeadType::Max];
	FDashMontageData* DashMontageData[(int32)EDashMontageType::Max];
	FDoubleJumpMontageData* DoubleJumpMontageData[(int32)EDoubleJumpMontageType::Max];
	FRidingMontageData* RidingMontageData[(int32)ERidingMontageType::Max];

	EHittedType HittedType;
	EDeadType DeadType;

};
