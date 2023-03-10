#pragma once

class SPUtils
{
public:
    template<typename T>
    static T* GetSPUPlayerComponent(AActor* PlayerPawn)
    {
        if(!PlayerPawn) return nullptr;

        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);;
    }
};
