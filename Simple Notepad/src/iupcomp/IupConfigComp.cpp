#include "iupcomp/IupConfigComp.h"
#include "iupcomp/IupDialogComp.h"
#include <iup_config.h>
#include "utils/Constants.h"

int IupConfigComp::load()
{
	return IupConfigLoad(iupComp);
}

void IupConfigComp::setAppName(const char* appName)
{
	IupSetAttribute(iupComp, Attr::APP_NAME, appName);
}

void IupConfigComp::dialogShowConfig(IupDialogComp& dialog, const char* group)
{
	IupConfigDialogShow(iupComp, dialog.handle(), group);
}
