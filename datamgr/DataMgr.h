/*
** Copyright (C) 2015 Wang Yaofu
** All rights reserved.
**
**Author:Wang Yaofu voipman@qq.com
**Description: The header file of class DataMgr.
*/
#pragma once
#include <sys/time.h>
#include <vector>
#include <mutex>
#include <iostream>

namespace {
    static const int kCleanInterval = 20;
    static long kAreaCleanTimestamp = 0;
}

template<class T>
// DataMgr item
class DataMgr {
public:
    DataMgr();
    ~DataMgr();
    T* getCur();
    T* getToLoad();
    void update();
    void garbageClean();
private:
    void pushGarbage(T *ptr);
    void freeOlds();

private:
    std::mutex mutex_;
    T* cur_;
    T* toLoad_;
    std::vector<T*> youngs_;
    std::vector<T*> olds_;
};

template<typename T>
DataMgr<T>::DataMgr() {
    cur_ = NULL;
    toLoad_ = NULL;
}

template<typename T>
DataMgr<T>::~DataMgr() {
    if (cur_) {
        delete cur_;
    }
}

template<typename T>
T* DataMgr<T>::getCur() {
    return cur_;
}

template<typename T>
T* DataMgr<T>::getToLoad() {
    if (toLoad_ == NULL) {
        toLoad_ = new T();
    }
    return toLoad_;
}

template<typename T>
void DataMgr<T>::update() {
    if (toLoad_ == NULL) {
        return;
    }
    if (cur_) {
        pushGarbage(cur_);
    }
    cur_ = toLoad_;
    toLoad_ = NULL;
}

template<typename T>
void DataMgr<T>::pushGarbage(T *ptr) {
    std::unique_lock<std::mutex> lock(mutex_);
    youngs_.push_back(ptr);
}

template<typename T>
void DataMgr<T>::garbageClean() {
    time_t now = time(NULL);
    // time_t now = DateTime::secs();
    if (kAreaCleanTimestamp > now) {
        return;
    }
    kAreaCleanTimestamp = now + kCleanInterval;
    freeOlds();
    std::unique_lock<std::mutex> lock(mutex_);
    olds_.swap(youngs_);
}

template<typename T>
void DataMgr<T>::freeOlds() {
    unsigned int size = olds_.size();
    for (auto it : olds_) {
        delete it;
    }
    olds_.clear();

    if (size > 0) {
        std::cout << "-----" << size << " DataMgr.data garbage deleted" << std::endl;
    }
}