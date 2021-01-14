// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMProjectile.h"
#include "GameFramework/Character.h"
#include "BMCharacter.generated.h"

UCLASS()
class BOSSMODE_API ABMCharacter : public ACharacter
{
	GENERATED_BODY()

	// �� �޽�, �ڱ⸸ �� �� ����
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FPMesh;

	// �� �޽�, �ڱ⸸ �� �� ����
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FPGunMesh;

	// 1��Ī ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPCameraComponent;

public:
	// Sets default values for this character's properties
	ABMCharacter();
	
	// �⺻ ȸ�� �ӵ�(deg/sec)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	// �⺻ ��/�Ʒ� ���� �ӵ� (deg/sec)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	// ����� �߻�ü Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<ABMProjectile> ProjectileClass;

	// �߻�ü ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile)
		class USceneComponent* ProjSpawn;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// APawn �������̽� Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// APawn �������̽� ���� Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// ���� �� �Ĺ� �̵� ó��
	void MoveForward(float Val);

	// �¿� ������
	void MoveLeftRight(float Val);

	// �¿� ��Ʈ������ �̵� ó��
	void MoveRight(float Val);

	/**
	* input axis ������ ���� ȣ��ȴ�
	* @param Rate ����ȭ�� �ӵ��̸� 1.0�� �ִ� ȸ�� �ӵ��̴�
	*/
	void TurnAtRate(float Rate);

	/**
	* �� �Ʒ��� �־��� �ӵ��� ���� ���� �Է¿��� ȣ���Ѵ�
	* @param Rate ����ȭ�� �ӵ��̸� 1.0�� �ִ� ȸ�� �ӵ��̴�
	*/
	void LookUpAtRate(float Rate);

	/** �߻�ü �߻��ϱ� */
	void OnFire();

	USceneComponent* TrackingSceneComponent;

	void OnTrack();
};
