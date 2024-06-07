#include "Widgets/UI_SubMarine.h"

void UUI_SubMarine::NativeOnInitialized()
{
	Gears.Add(Gear);
	Gears.Add(Gear2);
	Gears.Add(Gear3);
}

void UUI_SubMarine::SelectGear(int32 SelectIndex)
{
	for (int32 i = 0;  i < Gears.Num(); i++)
	{
		if (i == SelectIndex)
			Gears[i]->SetBackgroundColor(FLinearColor(1, 0, 0));
		else
			Gears[i]->SetBackgroundColor(FLinearColor(1, 1, 1));
	}
}

void UUI_SubMarine::SelectAuto(bool IsAuto)
{
	if (IsAuto)
		Auto->SetBackgroundColor(FLinearColor(1, 0, 0));
	else
		Auto->SetBackgroundColor(FLinearColor(1, 1, 1));
}
