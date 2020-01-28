#ifndef IUP_H_BOX_H
#define IUP_H_BOX_H
#include "IupComp.h"
#include <cstdlib>

class IupHBoxComp : public IupComp
{
public:
	IupHBoxComp(Ihandle* children,...) : IupComp(IupHbox(children, NULL)) {};
};

#endif
