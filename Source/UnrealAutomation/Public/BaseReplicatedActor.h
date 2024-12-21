#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseReplicatedActor.generated.h"

UCLASS()
class UNREALAUTOMATION_API ABaseReplicatedActor : public AActor
{
    GENERATED_BODY()

public:
    ABaseReplicatedActor();

protected:
    virtual void BeginPlay() override;

public:	
    virtual void Tick(float DeltaTime) override;

    // Replicated property
    UPROPERTY(ReplicatedUsing = OnRep_CubeColor, BlueprintReadOnly, Category="Replication")
    FLinearColor CubeColor;

    UFUNCTION()
    void OnRep_CubeColor();

    // Function to request a color change from clients
    UFUNCTION(BlueprintCallable, Category="Replication")
    void RequestColorChange(FLinearColor NewColor);

    // Server implementation of changing color
    UFUNCTION(Server, Reliable)
    void ServerChangeColor(const FLinearColor& NewColor);

    // Event to be called when the color changes
    UFUNCTION(BlueprintImplementableEvent, Category="Replication")
    void OnCubeColorChanged();


    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    void ChangeColor(const FLinearColor NewColor);
};
