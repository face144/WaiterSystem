#include "ServerPocket.h"

#include "Entities/RequestDirector.h"
#include "Entities/RequestListener.h"


void FServerPocket::Initialize()
{
	FPocket::Initialize();

	InstantiateEntity<FRequestListener>();
	InstantiateEntity<FRequestDirector>();
}
