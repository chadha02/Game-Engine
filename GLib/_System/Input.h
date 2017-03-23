#pragma once

#include <Windows.h>

#include "..\BasicTypes.h"

static int key;
namespace GLib
{
namespace Input
{
	bool Initialize();
	void Service(HRAWINPUT i_Input);
	int GetKey();
	void Shutdown();
} // namespace Input
} // namespace GLib
