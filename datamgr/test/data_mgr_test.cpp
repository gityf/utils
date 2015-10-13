/*
** Copyright (C) 2015 Wang Yaofu
** All rights reserved.
**
**Author:Wang Yaofu voipman@qq.com
**Description: The source file of main.
*/

#include <map>
#include <thread>
#include <iostream>
#include <unistd.h>
#include "DataMgr.h"
#include "Singleton.h"

using namespace std;
class Data {
public:
    Data() {}
    ~Data() {}
    void set(int k, int v) {
        datas_[k] = v;
    }
    int get(int k) {
        auto it = datas_.find(k);
        if (it != datas_.end()) {
            return it->second;
        }
        return 0;
    }
    void del(int k) {
        datas_.erase(k);
    }
private:
    std::map<int, int> datas_;
};

void testRunningThread() {
    DataMgr<Data>* dataMgr = Singleton<DataMgr<Data> >::Instance();
    while (true) {
        Data* dataPtr = dataMgr->getCur();
        if (dataPtr) {
            //cout << dataPtr->get(1) << endl;
            dataPtr->set(1, 2);
            dataPtr->get(1);
            dataPtr->del(1);
        }
        usleep(10);
    }
}

void testUpdatingThread() {
    DataMgr<Data>* dataMgr = Singleton<DataMgr<Data> >::Instance();
    long ii = 0;
    while (ii++ < 1000000) {
        Data* dataPtr = dataMgr->getToLoad();
        dataPtr->set(1, ii);
        dataMgr->update();
        usleep(10);
    }
}

void testCleanThread() {
    DataMgr<Data>* dataMgr = Singleton<DataMgr<Data> >::Instance();
    while (true) {
        dataMgr->garbageClean();
        sleep(2);
    }
}

int main(int argc, char* argv[]) {
    std::thread(&testRunningThread).detach();
    std::thread(&testUpdatingThread).detach();
    testCleanThread();

    return 0;
}
