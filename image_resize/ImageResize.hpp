/*
** Copyright (C) 2014 Wang Yaofu
** All rights reserved.
**
**Author:Wang Yaofu voipman@qq.com
**Description: The header file of class ImageResize.
*/

#pragma once
#include <string>

struct size {
    bool if_resize{ false };
    int resize_width{ 0 };
    int resize_height{ 0 };
    int crop_width{ 0 };
    int crop_height{ 0 };
    int crop_offset_x{ 0 };
    int crop_offset_y{ 0 };
    double ratio{ 0 };
};

class ImageResize {
public:
    ImageResize() {}
    ~ImageResize() {}
    int resizeImage(const std::string& file, int to_width, int to_height,
        std::string& outBinary);
    std::string loadfile(const std::string& file);
    int appendFile(const std::string& file, const char* dataPtr, int len);
    int processImage(const std::string& image_blob, int to_width, int to_height,
        std::string& outBinary);

    struct size computeSize(int width, int height, int to_width, int to_height);
};


