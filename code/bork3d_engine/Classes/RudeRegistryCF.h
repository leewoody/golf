#ifndef _H_RudeRegistryCF
#define _H_RudeRegistryCF

#include "RudeRegistry.h"

class RudeRegistryCF : public RudeRegistry
{
public:
	RudeRegistryCF(void);
	~RudeRegistryCF(void);

	int QueryByte(const TCHAR *app, const TCHAR *name, void *buffer, int *buffersize);
	int SetByte(const TCHAR *app, const TCHAR *name, void *buffer, int buffersize);

private:


};

#endif