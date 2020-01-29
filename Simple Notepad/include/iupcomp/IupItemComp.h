#ifndef IUP_ITEM_COMP_H
#define IUP_ITEM_COMP_H
#include "IupComp.h"

class IupItemComp : public IupComp
{
public:
	static const int IUP_ITEM_COMP = 5;
	IupItemComp(const char* title) : IupComp(IupItem(title,nullptr)) {}
	IupItemComp(const char* title, Icallback action, const char* image = nullptr);

	int type() override { return IUP_ITEM_COMP; }
};

#endif
