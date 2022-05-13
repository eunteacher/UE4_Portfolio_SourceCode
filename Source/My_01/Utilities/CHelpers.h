#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

class MY_01_API CHelpers
{

public:
	// 에디터의 에셋을 얻어오는 함수
	// OutObject 매개변수로 받아올 변수가 포인터 변수이기 때문에 이중 포인터로 받아오는거임.
	// InPath 경로
	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		// FObjectFinder() : 에디터 상에 있는 에셋을 찾는 함수
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded()"); // 에러 체크

		*OutObject = asset.Object;
		
	}

	// 클래스 찾아오는 함수 
	// TSubclassOf : 에디터 상의 블루 프린트 클래스를 이름을 붙여 C++에서 사용할 수 있게 함수
	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutClass = asset.Class;
	}

public:
	// 월드 상에 배치된 액터를 가져오는 함수 
	template<typename T>
	static T* GetWorldActor(UWorld* InWorld)
	{
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors) // 현재 레벨에 있는 모든 액터를 검사 
		{
			T* temp = Cast<T>(actor);
			if (!!temp)
			{
				return temp;
			}
		}

		return NULL;
	}

	// 액터를 배열로 가져옴
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

	// 액터의 컴포넌트를 가져오는 함수 
	// InActor 찾을 컴포넌트의 Owner를 넣어주면 됨
	// 자료형 다를 시 캐스팅
	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		// StaticClass() UClass를 알 수 있음, C++ 클래스를 말함 
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}
	
	// 컴포넌트 생성 함수
	// 컴포넌트를 생성할 객체, 생성할 컴포넌트 객체, 에디터상 이름, Owner 
	template<typename T>
	static void CreateComponent(AActor* InActor, T** InComponent, FName InName, USceneComponent* InParent = NULL)
	{
		// CreateDefaultSubobject() 컴포넌트 생성 함수 
		// InActor에 InName 이름으로 InComponent를 생성
		*InComponent = InActor->CreateDefaultSubobject<T>(InName); 

		if (InParent != NULL) // 부모 객체가 없을 경우 
		{
			// SetupAttachment() 계층 구조 생성 
			(*InComponent)->SetupAttachment(InParent);

			return;
		}

		InActor->SetRootComponent((*InComponent));
	}

	// Owner 지정만 없음
	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** InComponent, FName InName)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);
	}
};