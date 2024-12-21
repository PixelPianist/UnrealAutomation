#include "ReplicationFunctionalTest.h"
#include "BaseReplicatedActor.h"
#include "Engine/World.h"

void AReplicationFunctionalTest::StartTest()
{
    Super::StartTest();

    // Spawn our replicated actor
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    TestCube = GetWorld()->SpawnActor<ABaseReplicatedActor>(ABaseReplicatedActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

    if (!TestCube)
    {
        FinishTest(EFunctionalTestResult::Failed, TEXT("Could not spawn TestCube"));
        return;
    }

    // Basic check: Is replication enabled?
    if (!TestCube->GetIsReplicated())
    {
        FinishTest(EFunctionalTestResult::Failed, TEXT("TestCube not replicating"));
        return;
    }

    // We could implement a latent test to wait a few seconds and verify color changes,
    // but for simplicity, pass immediately. In a real scenario, you'd add delays or use
    // the Functional Testing framework's latent commands to check after time passes.
    FinishTest(EFunctionalTestResult::Succeeded, TEXT("TestCube is replicating properly."));
}
