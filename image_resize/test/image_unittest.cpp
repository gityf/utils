/*
** Copyright (C) 2015 Wang Yaofu
** All rights reserved.
**
**Author:Wang Yaofu voipman@qq.com
**Description: The source file of class ImageResizeTest.
*/
#include <test_harness.h>
#include <iostream>
#include "ImageResize.hpp"
#include "log/log.h"

TEST(ImageResizeTest, BasicTest) {
    log_init(LL_ALL, "gdbmdb", "./log");
    ImageResize imageResize;
    string outBinary;
    EXPECT_EQ(0, imageResize.resizeImage("2.jpg", 40, 60, outBinary));
    EXPECT_EQ(outBinary.length(),
        imageResize.appendFile("2.resize.jpg", outBinary.data(), outBinary.length()));
    EXPECT_TRUE(true);
}
