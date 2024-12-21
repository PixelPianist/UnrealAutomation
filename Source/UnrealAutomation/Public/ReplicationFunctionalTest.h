#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "ReplicationFunctionalTest.generated.h"

UCLASS()
class UNREALAUTOMATION_API AReplicationFunctionalTest : public AFunctionalTest
{
    GENERATED_BODY()
    
public:
    virtual void StartTest() override;

private:
    UPROPERTY()
    class ABaseReplicatedActor* TestCube;
};
