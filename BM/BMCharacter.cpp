// Fill out your copyright notice in the Description page of Project Settings.


#include "BMCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABMCharacter::ABMCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �ݸ��� ĸ�� ũ�� ����
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	//ĳ���� ȸ�� �ӵ� ����
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	// CameraComponent ����
	FPCameraComponent = CreateDefaultSubobject <UCameraComponent>(TEXT("FirstPersonCamera"));
	FPCameraComponent->SetupAttachment(GetCapsuleComponent());

	// ī�޶� ��ġ ����
	FPCameraComponent->AddRelativeLocation(FVector(0, 0, 64.0f));
	FPCameraComponent->bUsePawnControlRotation = true;

	// �����ϴ� �������� �� �� �ִ� �޽� ����
	FPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	FPMesh->SetOnlyOwnerSee(true);		// FPCameraComponent�� �����ϱ� ����
	FPMesh->SetupAttachment(FPCameraComponent);
	FPMesh->bCastDynamicShadow = false; // ���� ���ȸ� �ִ� �׸��ڸ� ���� ���ϱ�����
	FPMesh->CastShadow = false;			// ���� ���ȸ� �ִ� �׸��ڸ� ���� ���ϱ�����

	FPGunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FPGunMesh->SetOnlyOwnerSee(true);	// ���� ���� �÷��̾ �޽ø� �� ���̴�.
	FPGunMesh->bCastDynamicShadow = false;
	FPGunMesh->CastShadow = false;
	FPGunMesh->SetupAttachment(FPMesh, TEXT("GripPoint"));// , EAttachLocation::SnapToTargetIncludingScale, true);

	ProjSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawn"));
	ProjSpawn->SetupAttachment(FPGunMesh);
}

// Called when the game starts or when spawned
void ABMCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABMCharacter::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		// �ش���ġ�� �̵� �߰�
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void ABMCharacter::MoveLeftRight(float Val)
{
	if (Val != 0.0f)
	{
		// �ش���ġ�� �̵� �߰�
		//AddMovementInput()
	}
}

void ABMCharacter::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		// �ش���ġ�� �̵� �߰�
		AddMovementInput(GetActorRightVector(), Val);
	}
}

void ABMCharacter::TurnAtRate(float Rate)
{
	// �ӵ� �������� �� �������� ��Ÿ�� ����Ѵ�.
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABMCharacter::LookUpAtRate(float Rate)
{
	// �ӵ� �������� �� �������� ��Ÿ�� ����Ѵ�.
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABMCharacter::OnFire()
{
	// �߻�ü �߻� �õ�
	if (ProjectileClass != nullptr)
	{
		if (GetWorld() != nullptr)
		{
			ABMProjectile* ThisProjectile = GetWorld()->SpawnActor<ABMProjectile>(ProjectileClass, ProjSpawn->GetComponentLocation(), ProjSpawn->GetComponentRotation());

			ThisProjectile->GetProjectileMovement()->HomingTargetComponent = TrackingSceneComponent;
		}
	}
}

void ABMCharacter::OnTrack()
{
	FVector MousePos;
	FVector MouseDir;
	FHitResult Hit;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel2);
	APlayerController* playerController = Cast<APlayerController>(GetController());

	if (GEngine->GameViewport != nullptr && playerController != nullptr)
	{
		FVector2D ScreenPos = GEngine->GameViewport->Viewport->GetSizeXY();

		playerController->DeprojectScreenPositionToWorld(ScreenPos.X / 2.0f, ScreenPos.Y / 2.0f, MousePos, MouseDir);
		MouseDir *= 100000.0f;

		GetWorld()->LineTraceSingleByObjectType(Hit, MousePos, MouseDir, ObjectQueryParams);

		if (Hit.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("TRACE HIT WITH %s"), *(Hit.GetActor()->GetName()));
			TrackingSceneComponent = Cast<USceneComponent>(Hit.GetActor()->GetComponentByClass(USceneComponent::StaticClass()));
		}
		
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Trace"));
			TrackingSceneComponent = nullptr;
		}
	}
}

// Called to bind functionality to input �Է��ϴ� ����� ���ε� �ϱ� ���� ȣ��
void ABMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAxis("MoveForward", this, &ABMCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABMCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ABMCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ABMCharacter::LookUpAtRate);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ABMCharacter::OnFire);
	InputComponent->BindAction("Track",IE_Pressed, this, &ABMCharacter::OnTrack);
}

