#ifndef IUP_CONFIG_COMP_H
#define IUP_CONFIG_COMP_H
#include <iup.h>
#include "IupComp.h"
#include "IupMenuComp.h"
#include <iup_config.h>
#include <cstdio>

class IupDialogComp;

class IupConfigComp : public IupComp
{
public:
	static const int IUP_CONFIG_COMP = 1;

	IupConfigComp() : IupComp(IupConfig()) {};

	int load();
	void setAppName(const char*);
	void recentInitConfig(IupMenuComp& recentMenu, Icallback action, int limitNum);
	void dialogShowConfig(IupDialogComp& dialog, const char* group);

	int type() override { return IUP_CONFIG_COMP; }
};
#endif
