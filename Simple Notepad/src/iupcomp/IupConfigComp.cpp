#include "iupcomp/IupConfigComp.h"
#include "iupcomp/IupDialogComp.h"
#include <iup_config.h>

int IupConfigComp::load()
{
	return IupConfigLoad(iupComp);
}

void IupConfigComp::recentInitConfig(IupMenuComp& recentMenu, Icallback action, int limitNum)
{
	IupConfigRecentInit(iupComp, recentMenu.handle(), action, limitNum);
}

void IupConfigComp::dialogShowConfig(IupDialogComp& dialog, const char* group)
{
	IupConfigDialogShow(iupComp, dialog.handle(), group);
}
