#ifndef IUP_TEXT_COMP_H
#define IUP_TEXT_COMP_H

#include "iup.h"

class IupTextComp
{
private:
	Ihandle* iupText;
public:

	IupTextComp(Ihandle* iupText) { this->iupText = iupText; }
	
	/* Setters */
	void isMultiline(bool value);
	void isExpanded(bool value);
	void setName(char* name);
	void isDirty(bool value);
	void setFont(const char* font);
	
	/* Getters */
	char* getName();
	
	/* Callbacks */
	void setCaretPositionCallback(Icallback call);
	void setValueChangedCallback(Icallback call);
	void setDropFileCallback(Icallback call);

};
#endif