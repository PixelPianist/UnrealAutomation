#include "ReplicationFunctionalTest.h"
#include "BaseReplicatedActor.h"
#include "Engine/World.h"
#include "FunctionalTestingManager.h"
#include "Engine/Engine.h"


class FCheckColorChangeLatentCommand final : public IAutomationLatentCommand
{
public:
    FCheckColorChangeLatentCommand(AReplicationFunctionalTest* InTest, ABaseReplicatedActor* InTestCube)
        : Test(InTest), TestCube(InTestCube) {}

    virtual bool Update() override
    {
        if (TestCube->CubeColor == FLinearColor::Green)
        {
            Test->FinishTest(EFunctionalTestResult::Succeeded, TEXT("TestCube color changed successfully."));
        }
        else
        {
            Test->FinishTest(EFunctionalTestResult::Failed, TEXT("TestCube color did not change."));
        }
        return true;
    }

private:
    AReplicationFunctionalTest* Test;
    ABaseReplicatedActor* TestCube;
};

void AReplicationFunctionalTest::StartTest()
{
    Super::StartTest();

    // Delta cubes! Delta cubes! Delta cubes!
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    TestCube = GetWorld()->SpawnActor<ABaseReplicatedActor>(ABaseReplicatedActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

    // No cube? :(
    if (!TestCube)
    {
        FinishTest(EFunctionalTestResult::Failed, TEXT("Could not spawn TestCube"));
        return;
    }

    // No replication? Double :(
    if (!TestCube->GetIsReplicated())
    {
        FinishTest(EFunctionalTestResult::Failed, TEXT("TestCube not replicating"));
        return;
    }

    // Change color to green, check if it changed
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.5f));
    ADD_LATENT_AUTOMATION_COMMAND(FCheckColorChangeLatentCommand(this, TestCube));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.5f));
    ADD_LATENT_AUTOMATION_COMMAND(FExecStringLatentCommand(TEXT("TestCube->RequestColorChange(FLinearColor::Green);")));
}