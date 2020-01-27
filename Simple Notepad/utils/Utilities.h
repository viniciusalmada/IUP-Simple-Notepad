#ifndef UTILS_H
#define UTILS_H

#include "iup.h"

namespace Utils
{
	const char* strFileTitle(const char* filename);

	int stringCompare(const char* l, const char* r, int case_sensitive);

	int findString(const char* str, const char* strToFind, int casesensitive);

	char* readFile(const char* filename);

	int writeFile(const char* filename, const char* str, int count);

	void newFile(Ihandle* self);

	void openFile(Ihandle* self, const char* filename);

	void saveFile(Ihandle* multitext);

	void saveasFile(Ihandle* multitext, const char* filename);

	int saveCheck(Ihandle* ih);
}
#endif
