#include "BaseReplicatedActor.h"
#include "Net/UnrealNetwork.h"

ABaseReplicatedActor::ABaseReplicatedActor()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
}

void ABaseReplicatedActor::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        // Set a default color on the server
        CubeColor = FLinearColor::Red;
    }
}

void ABaseReplicatedActor::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (HasAuthority())
    {
        // For demo: every 5 seconds, pick a random color
        const float TimeSeconds = GetWorld()->GetTimeSeconds();
        if (FMath::Fmod(TimeSeconds, 5.0f) < 0.016f) 
        {
            const FLinearColor RandomColor = FLinearColor::MakeRandomColor();
            ServerChangeColor(RandomColor);
        }
    }
}

void ABaseReplicatedActor::OnRep_CubeColor()
{
    OnCubeColorChanged();
}

void ABaseReplicatedActor::RequestColorChange(const FLinearColor NewColor)
{
    if (HasAuthority())
    {
        ChangeColor(NewColor);
    }
    else
    {
        ServerChangeColor(NewColor);
    }
}

void ABaseReplicatedActor::ServerChangeColor_Implementation(const FLinearColor& NewColor)
{
    ChangeColor(NewColor);
}

void ABaseReplicatedActor::ChangeColor(const FLinearColor NewColor)
{
    CubeColor = NewColor;
    OnRep_CubeColor();
}

void ABaseReplicatedActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABaseReplicatedActor, CubeColor);
}