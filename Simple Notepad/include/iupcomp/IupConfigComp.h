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
	IupConfigComp(Ihandle* configH) : IupComp(configH)
	{
		printf("Config address: %d", int(configH));
	};

	int load();
	void recentInitConfig(IupMenuComp& recentMenu, Icallback action, int limitNum);
	void dialogShowConfig(IupDialogComp& dialog, const char* group);
};
#endif
