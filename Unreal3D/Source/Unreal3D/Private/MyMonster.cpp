// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster.h"

#include "Components/WidgetComponent.h"

#include "MyAnimInstance.h"
#include "MyStatComponent.h"
#include "MyPlayer.h"
#include "MyHpBar.h"

// Sets default values
AMyMonster::AMyMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_hpBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	_hpBarWidget->SetupAttachment(GetMesh());
	_hpBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UMyHpBar> hpBarClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/BP_MyHpBar.BP_MyHpBar_C'"));
	if (hpBarClass.Succeeded())
	{
		_hpBarWidget->SetWidgetClass(hpBarClass.Class);
	}
}

// Called when the game starts or when spawned
void AMyMonster::BeginPlay()
{
	Super::BeginPlay();
	
	auto hpBar = Cast<UMyHpBar>(_hpBarWidget->GetWidget());
	if (hpBar)
	{
		_statComponent->_hpChanged.AddUObject(hpBar, &UMyHpBar::SetHpBarValue);
	}
}

// Called every frame
void AMyMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto playerController = GetWorld()->GetFirstPlayerController();
	if (playerController)
	{
		FVector playerLocation = playerController->GetPawn()->GetActorLocation();
		float distance = FVector::Distance(GetActorLocation(), playerLocation);

		if (distance > 1000.0f)
			_hpBarWidget->SetVisibility(false);
		else
			_hpBarWidget->SetVisibility(true);
	}
}

// Called to bind functionality to input
void AMyMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMyMonster::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	_statComponent->AddCurHp(-Damage);

	if (IsDead())
	{
		auto player = Cast<AMyPlayer>(DamageCauser);
		if (player)
		{
			player->AddExp(15);
		}
	}

	return Damage;
}

