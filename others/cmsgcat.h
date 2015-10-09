// msgcat.h -- message catalog runtime class

#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

class msgcat {
private:
	char soname[1100];
	void *handle;
	int*  _nmsgs;
	char** msgs;
	char** ids;
	char** boths;
	char*  _errmsg;

public:
	msgcat(char* catname);
	~msgcat();
	char* errmsg(void);
	int nmsgs(void);
	int loaded(void);
	char* id(int n);
	char* text(int n);
	char* both(int n);
};

// eof: msgcat.h
