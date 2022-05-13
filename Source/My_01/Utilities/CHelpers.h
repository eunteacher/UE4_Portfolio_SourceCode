#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

class MY_01_API CHelpers
{

public:
	// �������� ������ ������ �Լ�
	// OutObject �Ű������� �޾ƿ� ������ ������ �����̱� ������ ���� �����ͷ� �޾ƿ��°���.
	// InPath ���
	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		// FObjectFinder() : ������ �� �ִ� ������ ã�� �Լ�
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded()"); // ���� üũ

		*OutObject = asset.Object;
		
	}

	// Ŭ���� ã�ƿ��� �Լ� 
	// TSubclassOf : ������ ���� ��� ����Ʈ Ŭ������ �̸��� �ٿ� C++���� ����� �� �ְ� �Լ�
	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutClass = asset.Class;
	}

public:
	// ���� �� ��ġ�� ���͸� �������� �Լ� 
	template<typename T>
	static T* GetWorldActor(UWorld* InWorld)
	{
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors) // ���� ������ �ִ� ��� ���͸� �˻� 
		{
			T* temp = Cast<T>(actor);
			if (!!temp)
			{
				return temp;
			}
		}

		return NULL;
	}

	// ���͸� �迭�� ������
	template<typename T>
	static TArray<T *> GetWorldActors(UWorld* InWorld)
	{
		TArray<T *> actors;
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			T* temp = Cast<T>(actor);
			if (!!temp)
				actors.Add(temp);
		}

		return actors;
	}

	// ������ ������Ʈ�� �������� �Լ� 
	// InActor ã�� ������Ʈ�� Owner�� �־��ָ� ��
	// �ڷ��� �ٸ� �� ĳ����
	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		// StaticClass() UClass�� �� �� ����, C++ Ŭ������ ���� 
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}
	
	// ������Ʈ ���� �Լ�
	// ������Ʈ�� ������ ��ü, ������ ������Ʈ ��ü, �����ͻ� �̸�, Owner 
	template<typename T>
	static void CreateComponent(AActor* InActor, T** InComponent, FName InName, USceneComponent* InParent = NULL)
	{
		// CreateDefaultSubobject() ������Ʈ ���� �Լ� 
		// InActor�� InName �̸����� InComponent�� ����
		*InComponent = InActor->CreateDefaultSubobject<T>(InName); 

		if (InParent != NULL) // �θ� ��ü�� ���� ��� 
		{
			// SetupAttachment() ���� ���� ���� 
			(*InComponent)->SetupAttachment(InParent);

			return;
		}

		InActor->SetRootComponent((*InComponent));
	}

	// Owner ������ ����
	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** InComponent, FName InName)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);
	}
};