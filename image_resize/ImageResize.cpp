/*
** Copyright (C) 2014 Wang Yaofu
** All rights reserved.
**
**Author:Wang Yaofu voipman@qq.com
**Description: The source file of class ImageResize.
*/

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <memory>
#include <Magick++.h>
#include <magick/geometry.h>
#include "ImageResize.hpp"
#include "log/log.h"
using namespace Magick;

#define RET_ERROR -1
#define RET_OK     0

int ImageResize::resizeImage(const std::string& file, int to_width, int to_height,
    std::string& outBinary) {
    string image_binary = loadfile(file);
    if (image_binary.empty()) {
        LOG_WARN("open image file:%s failed.", file.c_str());
        return RET_ERROR;
    }
    return processImage(image_binary, to_width, to_height, outBinary);
}

std::string ImageResize::loadfile(const std::string& file) {
    size_t buffer_length = 0;
    std::ifstream data_file(file, std::ifstream::binary);
    if (data_file.is_open()) {
        std::stringstream buffer;
        buffer << data_file.rdbuf();
        return buffer.str();
    }
    else {
        return "";
    }
}

int ImageResize::appendFile(const string& file, const char* dataPtr, int len) {
    std::ofstream ofs(file, std::ofstream::app | std::ofstream::binary);
    if (ofs.is_open() && ofs.good()) {
        ofs.write(dataPtr, len);
    }
    return len;
}

int ImageResize::processImage(const std::string& image_blob, int to_width, int to_height,
    std::string& outBinary) {
    Image image;
    try {
        image.read(Blob(image_blob.c_str(), image_blob.size()));
    }
    catch (...) {
        LOG_WARN("read image data into memory faild.");
        return RET_ERROR;
    }

    int height = image.rows();
    int width = image.columns();

    size final_size;
    bool if_use_raw_image_ = false;

    if (!if_use_raw_image_) {
        // resize and crop the iamge according the input to_width and to_height
        final_size = computeSize(width, height, to_width, to_height);

        if (final_size.if_resize) {
            try {
                image.resize(Geometry(final_size.resize_width, final_size.resize_height));
            }
            catch (...) {
                LOG_WARN("resize image faild.");
                return RET_ERROR;
            }
        }

        try {
            image.crop(Geometry(final_size.crop_width, final_size.crop_height,
                final_size.crop_offset_x, final_size.crop_offset_y));
        }
        catch (...) {
            LOG_WARN("crop image faild.");
            return RET_ERROR;
        }
    }
    Blob blob;
    image.write(&blob);
    outBinary.assign(static_cast<const char*>(blob.data()), blob.length());
    return RET_OK;
}

struct size ImageResize::computeSize(
    int width, int height, int to_width, int to_height) {

    double ratio_height = (double)to_height / height;
    double ratio_width = (double)to_width / width;
    double ratio = ratio_height;

    // set the gravity to the center of the image
    enum zero_offset {
        x,
        y
    };
    enum zero_offset cord = y;

    // we only resize the larger ratio
    if (ratio_height < ratio_width) {
        ratio = ratio_width;
        cord = x;
    }

    size final_size;

    // if ratio > 1, that means we need enlarge the image
    // but we do not enlarge iamge, we only shrink the image
    final_size.if_resize = true;
    if (ratio > 1) {
        final_size.if_resize = false;
    }

    if (cord == x) {
        if (final_size.if_resize) {
            final_size.resize_height = height * ratio + 1;
            final_size.resize_width = to_width;
            final_size.crop_height = to_height;
            final_size.crop_width = to_width;

            final_size.crop_offset_y = (final_size.resize_height - to_height) >> 1;
        }
        else {
            final_size.crop_height = to_height / ratio;
            final_size.crop_width = width;

            final_size.crop_offset_y = (height - final_size.crop_height) >> 1;
        }
    }
    else {
        if (final_size.if_resize) {
            final_size.resize_height = to_height;
            final_size.resize_width = width * ratio + 1;
            final_size.crop_height = to_height;
            final_size.crop_width = to_width;

            final_size.crop_offset_x = (final_size.resize_width - to_width) >> 1;
        }
        else {
            final_size.crop_height = height;
            final_size.crop_width = to_width / ratio;

            final_size.crop_offset_x = (width - final_size.crop_width) >> 1;
        }
    }
    final_size.ratio = ratio;
    return final_size;
}



