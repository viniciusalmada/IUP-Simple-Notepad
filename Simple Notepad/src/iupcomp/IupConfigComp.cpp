#include "iupcomp\IupConfigComp.h"
#include <iup_config.h>

int IupConfigComp::load()
{
	return IupConfigLoad(iupComp);
}
