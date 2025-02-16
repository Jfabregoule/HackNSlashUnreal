#include "KnightAnimInstance.h"

void UKnightAnimInstance::NativeInitializeAnimation()
{
    // R�cup�re le personnage associ� � cet AnimInstance
    APawn* Pawn = TryGetPawnOwner();
    if (Pawn)
    {
        Character = Cast<ACharacter>(Pawn);
    }
}

void UKnightAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Character) return;

    // R�cup�re la vitesse actuelle du personnage
    FVector Velocity = Character->GetVelocity();
    Speed = Velocity.Size();  // Calcul de la norme du vecteur vitesse

    // R�cup�re la direction du mouvement
    FRotator Rotation = Character->GetActorRotation();
    Direction = CalculateDirection(Velocity, Rotation);
}
