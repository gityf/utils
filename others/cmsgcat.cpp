// msgcat.cc -- message catalog runtime class
#include <stdlib.h>
#include "cmsgcat.h"

msgcat::msgcat(char* catname) {
    char* prefix2 = getenv("LANG");
    if (prefix2==NULL) prefix2= "zh";
#ifdef HPUX
    char *suffix="sl";
#else
    char *suffix="so";
#endif

    sprintf(soname, "./%s-%s.%s", catname, prefix2,suffix);
    _errmsg = "";

    handle = dlopen (soname, RTLD_LAZY);
    if (handle == NULL) {
        _errmsg = dlerror();
        return;
    }

    _nmsgs = (int*)dlsym(handle, "nmsgs");
    if (_nmsgs == 0) {
        _errmsg = dlerror();
        return;
    }
    msgs = (char**)dlsym(handle, "msgs");
    if (msgs == 0) {
        _errmsg = dlerror();
        return;
    }
    ids = (char**)dlsym(handle, "ids");
    if (ids == 0) {
        _errmsg = dlerror();
        return;
    }
    boths = (char**)dlsym(handle, "both");
    if (boths == 0) {
        _errmsg = dlerror();
        return;
    }
};

msgcat::~msgcat() {
    dlclose(handle);
}

char* msgcat::errmsg(void) {
    return _errmsg;
}
int msgcat::nmsgs(void) {
    return *_nmsgs;
}
int msgcat::loaded(void) {
    int rc;
    rc = handle ? 1 : 0;
    return rc;
}
char* msgcat::id(int n) {
    char* m;
    n -= 1001;
    if (n < 0)
        m = "message number too low";
    else if (n >= *_nmsgs)
        m = "message number too high";
    else
        m = ids[n];
    return m;
}
char* msgcat::text(int n) {
    char* m;
    n -= 1001;
    if (n < 0)
        m = "message number too low";
    else if (n >= *_nmsgs)
        m = "message number too high";
    else
        m = msgs[n];
    return m;
}
char* msgcat::both(int n) {
    char* m;
    n -= 1001;
    if (n < 0)
        m = "message number too low";
    else if (n >= *_nmsgs)
        m = "message number too high";
    else
        m = boths[n];
    return m;
}

// eof: msgcat.cc
