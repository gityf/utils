/*
** Copyright (C) 2014 Wang Yaofu
** All rights reserved.
**
**Author:Wang Yaofu voipman@qq.com
**Description: The source file of config demo.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "libconfig.h++"

#define LOG_ERROR printf

template<typename T>
bool getSingleElement(libconfig::Config& cfg, const string& path){
    if (cfg.exists(path)){
        // TODO: save getting value
        T value = cfg.lookup(path.c_str());
        return true;
    }
    return false;
}

bool configInit(const string& rCurrDir) {
    std::string sFullFileName(rCurrDir + "/config/main.cfg");
    libconfig::Config cfg;
    do {
        try {
            cfg.readFile(sFullFileName.c_str());
        }
        catch (const libconfig::FileIOException& fioe) {
            LOG_ERROR("I/O error while reading file£º%s",
                sFullFileName.c_str());
            break;
        }
        catch (libconfig::ParseException& pe) {
            LOG_ERROR("Parse error at %s : %d - %s",
                pe.getFile(), pe.getLine(), pe.getError());
            break;
        }
        catch (...) {
            LOG_ERROR("Unkown error while reading file£º%s",
                sFullFileName.c_str());
            break;
        }

        try {
            getSingleElement<string>(cfg, "strCfgItem");
            getSingleElement<int>(cfg, "intCfgItem");
        }
        catch (const libconfig::SettingNotFoundException& snfe) {
            LOG_ERROR("Setting not found");
            break;
        }
        catch (...) {
            LOG_ERROR("Unkown Setting error while reading file£º%s",
                sFullFileName.c_str());
            break;
        }
        return true;
    } while (0);
    return false;
}