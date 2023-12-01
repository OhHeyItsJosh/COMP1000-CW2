#include "serialisation.h"

//template<typename T>
//SerialisationHandler<T>::SerialisationHandler(std::function<std::string(T)> onSerialise, std::function<T(std::string)> onDeserialise)
//	: m_onSerialise(onSerialise), m_onDeserialise(onDeserialise)
//{}
//
//template<typename T>
//SerialisationHandler<T>::SerialisationHandler() 
//	: m_onSerialise(SerialisationHandler::standardSerialise), m_onDeserialise(SerialisationHandler::standardDeserialise) 
//{}