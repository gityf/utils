/*
** Copyright (C) 2014 Wang Yaofu
** All rights reserved.
**
**Description: The source file of demo.
*/

#include "demo.pb.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

int appendFile(const string& file, const char* dataPtr, int len) {
    std::ofstream ofs(file, std::ofstream::app | std::ofstream::binary);
    if (ofs.is_open() && ofs.good()) {
        ofs.write(dataPtr, len);
    }
    return len;
}

int main(int argc, char *argv[]) {
    demo::VarintMsg* varintMsg = new demo::VarintMsg();
    varintMsg->set_argi32(0x41);
    varintMsg->set_argi64(0x12345678);
    varintMsg->set_argui32(0x332211);
    varintMsg->set_argui64(0x998877);
    varintMsg->set_argsi32(-100);
    varintMsg->set_argsi64(-200);
    varintMsg->add_argbool(true);
    varintMsg->add_argbool(false);
    varintMsg->set_argenum(demo::SECOND_PRICE);

    demo::Bit64* bit64 = new demo::Bit64();
    bit64->set_argfixed64(0x123456);
    bit64->set_argsfixed64(-100);
    bit64->set_argdouble(3.1415926);

    demo::Bit32* bit32 = new demo::Bit32();
    bit32->set_argfixed32(0x1234);
    bit32->set_argsfixed32(-10);
    bit32->set_argfloat(3.1415);

    demo::LenPayload* lenPayload = new demo::LenPayload();
    lenPayload->add_argstrlist("String 1.");
    lenPayload->add_argstrlist("String 2.");
    lenPayload->set_allocated_argvarintmsg(varintMsg);
    lenPayload->set_allocated_argbit64(bit64);
    lenPayload->set_allocated_argbit32(bit32);
    std::string content;
    lenPayload->SerializeToString(&content);
    appendFile("pb.bin", content.data(), content.length());
    delete lenPayload;
    return 0;
}
