#include "Components/CStatusComponent.h"
#include "Global.h"

#include "GameFramework/CharacterMovementComponent.h"

UCStatusComponent::UCStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bCanMove = true;
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	Stamina = MaxStamina;
	Mana = MaxMana;
}

void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UCStatusComponent::SetMove()
{
	bCanMove = true;
}

void UCStatusComponent::SetStop()
{
	bCanMove = false;
}

void UCStatusComponent::SetSpeed(ECharacterSpeed InSpeed)
{
	UCharacterMovementComponent* movement = CHelpers::GetComponent<UCharacterMovementComponent>(GetOwner());

	switch (InSpeed)
	{
		case ECharacterSpeed::Walk:
			movement->MaxWalkSpeed = WalkSpeed;
			break;
	
		case ECharacterSpeed::Run:
			movement->MaxWalkSpeed = RunSpeed;
			break;

		case ECharacterSpeed::Sprint:
			movement->MaxWalkSpeed = SprintSpeed;
			break;

		case ECharacterSpeed::Max:
			break;
	
		default:
			break;
	}
}

void UCStatusComponent::SetSpeed(EHorseSpeed InSpeed)
{
	UCharacterMovementComponent* movement = CHelpers::GetComponent<UCharacterMovementComponent>(GetOwner());

	switch (InSpeed)
	{
		case EHorseSpeed::Walk:
			movement->MaxWalkSpeed = Horse_WalkSpeed;
			break;

		case EHorseSpeed::Sprint:
			movement->MaxWalkSpeed = Horse_SprintSpeed;
			break;

		case EHorseSpeed::Max:
			break;

		default:
			break;
	}
}

void UCStatusComponent::AddHealth(float InAmount)
{
	Health += InAmount;

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::SubHealth(float InAmount)
{
	Health -= InAmount;

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::AddStamina(float InAmount)
{
	Stamina += InAmount;

	Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);
}

void UCStatusComponent::SubStamina(float InAmount)
{
	Stamina -= InAmount;

	Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);
}

void UCStatusComponent::AddMana(float InAmount)
{
	Mana += InAmount;

	Mana = FMath::Clamp(Mana, 0.0f, MaxMana);
}

void UCStatusComponent::SubMana(float InAmount)
{
	Mana -= InAmount;

	Mana = FMath::Clamp(Mana, 0.0f, MaxMana);
}

