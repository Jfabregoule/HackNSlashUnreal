#include "EPC_NPC.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidget.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"

AEPC_NPC::AEPC_NPC()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));
	HealthBarComponent->SetupAttachment(RootComponent);
}

void AEPC_NPC::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (!m_weaponType) {
		return;
	}

	Execute_SetCurrentWeapon(this, GetWorld()->SpawnActor<AEP_BaseWeapon>(m_weaponType, GetActorLocation(), GetActorRotation(), SpawnParams));

}

void AEPC_NPC::SetHealth(float NewHealth)
{
	Super::SetHealth(NewHealth);

	UHealthBarWidget* healthbar = Cast<UHealthBarWidget>(HealthBarComponent->GetWidget());

	if (healthbar)
	{
		healthbar->UpdateHealthBar(NewHealth, GetMaxHealth());
	}

	if (Health <= 0)
	{
		Destroy();
	}
}

void AEPC_NPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEPC_NPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UBehaviorTree* AEPC_NPC::GetBehaviorTree() const
{
	return Tree;
}

float AEPC_NPC::GetMinAttackRange() const
{
	return MinAttackRange;
}

float AEPC_NPC::GetMaxAttackRange() const
{
	return MaxAttackRange;
}
