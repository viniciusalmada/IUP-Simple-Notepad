#include "iupcomp/IupDialogComp.h"
#include "utils/Constants.h"

void IupDialogComp::menu(IupMenuComp& menu)
{
	IupSetAttributeHandle(iupComp, Attr::MENU, menu.handle());
}

void IupDialogComp::title(const char* str)
{
	IupSetAttribute(iupComp, Attr::TITLE, str);
}

void IupDialogComp::size(const char* sizes)
{
	IupSetAttribute(iupComp, Attr::SIZE, sizes);
}

void IupDialogComp::closeCallback(Icallback closeAction)
{
	IupSetCallback(iupComp, Attr::CLOSE_CB, closeAction);
}

void IupDialogComp::dropFileCallback(Icallback dropFileAction)
{
	IupSetCallback(iupComp, Attr::DROPFILES_CB, dropFileAction);
}

void IupDialogComp::configuration(IupConfigComp& config)
{
	IupSetAttribute(iupComp, Attr::CONFIG,(const char*) config.handle());
}

void IupDialogComp::keyShortcut(const char* key, Icallback action)
{
	IupSetCallback(iupComp, key, action);
}

void IupDialogComp::showWithConfig(IupConfigComp& config)
{
	IupConfigDialogShow(config.handle(), iupComp, Group::MAIN_WINDOW);
}

void IupDialogComp::userSize(const char* value)
{
	IupSetAttribute(iupComp, Attr::USERSIZE, value);
}
