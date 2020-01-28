#ifndef IUP_V_BOX_COMP_H
#define IUP_V_BOX_COMP_H
#include "IupComp.h"
#include <cstdlib>

class IupVBoxComp : public IupComp
{
public:
	IupVBoxComp(Ihandle* children, ...) : IupComp(IupVbox(children,NULL)) {}
};
#endif
