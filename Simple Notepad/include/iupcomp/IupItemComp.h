#ifndef IUP_ITEM_COMP_H
#define IUP_ITEM_COMP_H
#include "IupComp.h"

class IupItemComp : public IupComp
{
public:
	IupItemComp(Ihandle* iupItem) : IupComp(iupItem) {}
	IupItemComp(const char* title, Icallback action, const char* image = nullptr);
	IupItemComp(const char* title, const char* name = nullptr);

};

#endif
