#include "Widgets/UI_Distance.h"

void UUI_Distance::SetDistance(float InDistance)
{
	FString distance = FString::FromInt(InDistance);
	distance.AppendChar('M');
	Distance->SetText(FText::FromString(distance));
}
