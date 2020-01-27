#ifndef IUP_CONFIG_COMP_H
#define IUP_CONFIG_COMP_H
#include <iup.h>
#include "IupComp.h"

class IupConfigComp : public IupComp
{
public:
	IupConfigComp(Ihandle* config) : IupComp(config) { };

	int load();
};
#endif
