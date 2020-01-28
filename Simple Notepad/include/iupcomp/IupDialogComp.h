#ifndef IUP_DIALOG_COMP_H
#define IUP_DIALOG_COMP_H
#include "IupMenuComp.h"
#include "IupConfigComp.h"

class IupDialogComp : public IupComp
{
public:
	IupDialogComp(IupComp& child) : IupComp(IupDialog(child.handle())){}
	void menu(IupMenuComp& menu);
	void title(const char* str);
	void size(const char* sizes);
	void closeCallback(Icallback closeAction);
	void dropFileCallback(Icallback dropFileAction);
	void configuration(IupConfigComp& config);
	void keyShortcut(const char* key, Icallback action);
};

#endif
