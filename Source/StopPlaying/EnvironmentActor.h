#pragma once

#include "GameFramework/Actor.h"
#include "EnvironmentActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToggle, bool, bIsEnabled);

USTRUCT(BlueprintType)
struct FBoolVector
{
	GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool X = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool Y = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool Z = false;
};

UCLASS()
class STOPPLAYING_API AEnvironmentActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnvironmentActor();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

    UPROPERTY(EditAnywhere)
    FString Name = "ActorName";
    
    UPROPERTY(EditAnywhere)
    FBoolVector ConstrainScale;
    
    UPROPERTY(EditAnywhere)
    FBoolVector ConstrainRotation;
    
    UPROPERTY(EditAnywhere)
    FBoolVector ConstrainTranslation;

    UPROPERTY(BlueprintAssignable)
    FOnToggle OnToggle;
    
    FTransform InitialTransform;
	
    /**
     * Resets this actor
     */
    UFUNCTION(BlueprintCallable, Category="EnvironmentActor")
    virtual void Reset(bool bShouldRepeat = false);

    /**
     * Toggles a state on this actor
     * The implmentation is up to the subclass
     */
    virtual void Toggle(bool bIsEnabled);

protected:
    TMap<ECollisionChannel, ECollisionResponse> InitialCollision;
    int32 RepeatReset = 0;

};
