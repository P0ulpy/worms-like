#include "InputSignal.h"

template<typename T>
size_t InputSignal::Connect(const EventSignalType& eventSignalType, T* instance, void(T::*method)())
{
    return Connect(eventSignalType, [=]() { (instance->*method)(); });
}