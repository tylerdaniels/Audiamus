// This is the main DLL file.

#include "stdafx.h"

#include "FastForward.NET.h"

using namespace FastForwardNET;

void FFNet::RegisterAll( void ) {
	av_register_all();
}
