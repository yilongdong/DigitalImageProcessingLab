#include "Common/BMPImage.h"
#include "Common/Util.h"
#include <stdexcept>
#include <cstdio>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace image;

BitmapImage::BitmapImage(std::string const &filename) : m_colorPanel(NULL), m_data(NULL) {
    loadFromFile(filename);
}

BitmapImage::~BitmapImage() {
    free(m_data);
    free(m_colorPanel);
}

void BitmapImage::loadFromFile(std::string const &filename) {
    FILE *fp = fopen(filename.c_str(), "r");
    if (fp == NULL) {
        throw std::runtime_error{"open file:" + filename + " fail\n"};
    }
    size_t ret;
    ret = fread(&m_fileHeader, sizeof(struct BitmapFileHeader), 1, fp);
    if (ret != 1) {
        throw std::runtime_error{"read bitmap file header error\n"};
    }
    ret = fread(&m_infoHeader, sizeof(struct BitmapInfoHeader), 1, fp);
    if (ret != 1) {
        throw std::runtime_error{"read bitmap info header error\n"};
    }

    if (m_fileHeader.bfType != 0x4d42) {
        throw std::runtime_error("not BMP file\n");
    }
    if (m_infoHeader.biCompression != 0) {
        throw std::runtime_error("only support BI_RGB BMP file type\n");
    }
    m_colorPanelSize = m_fileHeader.bfOffBytes - sizeof(BitmapFileHeader) - sizeof(BitmapInfoHeader);
    if (m_colorPanelSize != 0) {
        m_colorPanel = (uint8_t *) malloc(m_colorPanelSize);
        ret = fread(m_colorPanel, 1, m_colorPanelSize, fp);
    }
    ret = fseek(fp, m_fileHeader.bfOffBytes, SEEK_SET);
    if (ret != 0) {
        throw std::runtime_error("fseek fail with ret = " + std::to_string(ret) + '\n');
    }
    uint32_t height = m_infoHeader.biHeight > 0 ? m_infoHeader.biHeight : -m_infoHeader.biHeight;
    uint32_t width = m_infoHeader.biWidth;
    uint32_t bytesPerPixel = m_infoHeader.biBitCount / 8;
    // 按4字节对齐
    uint32_t bytesPerLine = alignUpBit(width * bytesPerPixel, 4);
    m_bytesPerLine = bytesPerLine;
    uint32_t totalPixelDataBytes = height * bytesPerLine;
    m_data = (uint8_t *) malloc(totalPixelDataBytes);
    ret = fread(m_data, sizeof(uint8_t), totalPixelDataBytes, fp);
    if (ret != totalPixelDataBytes) {
        throw std::runtime_error(
                "not enough bytes to read for pixel data, totalPixelDataBytes = "
                + std::to_string(totalPixelDataBytes) + '\n');
    }
    fclose(fp);
    m_lines.reserve(height);
    for (uint32_t i = 0; i < height; ++i) {
        m_lines.push_back(m_data + i * bytesPerLine);
    }
}

void BitmapImage::saveToFile(std::string const &filename) {
    FILE *fp = fopen(filename.c_str(), "w");
    if (fp == NULL) {
        throw std::runtime_error("fail to open file: " + filename + "\n try 'find Resources -type f | xargs -n 1 -I file chmod a+rw file'");
    }
    fwrite(&m_fileHeader, sizeof(m_fileHeader), 1, fp);
    fwrite(&m_infoHeader, sizeof(m_infoHeader), 1, fp);
    fwrite(m_colorPanel, 1, m_colorPanelSize, fp);
    fwrite(m_data, m_bytesPerLine, m_lines.size(), fp);
    fclose(fp);
}

std::string BitmapImage::strFileHeader() const {
    static char buffer[1024 * 4];
    snprintf(buffer, sizeof(buffer),
             "FileHeader:\n\tbfSize = %u\n"
             "\tbfReserved1 = %u\n"
             "\tbfReserved2 = %u\n"
             "\tbfOffBits = %u\n",
             m_fileHeader.bfSize, m_fileHeader.bfReserved1,
             m_fileHeader.bfReserved2, m_fileHeader.bfOffBytes);
    return buffer;
}

std::string BitmapImage::strInfoHeader() const {
    static char buffer[1024 * 4];
    snprintf(buffer, sizeof(buffer),
             "InfoHeader:\n\tbiSize = %u\n"
             "\tbiWidth = %u\n"
             "\tbiHeight = %d\n"
             "\tbiPlanes = %u\n"
             "\tbiBitCount = %u\n"
             "\tbiCompression = %u\n"
             "\tbiSizeImage = %u\n"
             "\tbiXPelsPerMeter = %u\n"
             "\tbiYPelsPerMeter = %u\n"
             "\tbiClrUsed = %u\n"
             "\tbiClrImportant = %u\n",
             m_infoHeader.biSize, m_infoHeader.biWidth, m_infoHeader.biHeight,
             m_infoHeader.biPlanes, m_infoHeader.biBitCount,
             m_infoHeader.biCompression, m_infoHeader.biSizeImage,
             m_infoHeader.biXPelsPerMeter, m_infoHeader.biYPelsPerMeter,
             m_infoHeader.biClrUsed, m_infoHeader.biClrImportant);
    return {buffer};
}

uint32_t BitmapImage::width() const { return m_infoHeader.biWidth; }

int32_t BitmapImage::height() const { return m_infoHeader.biHeight; }

uint8_t *BitmapImage::data() const { return m_data; }

uint8_t *BitmapImage::line(uint32_t index) const { return m_lines.at(index); }

std::vector<uint8_t *> &BitmapImage::lines() { return m_lines; }

uint32_t BitmapImage::bytesPerLine() const { return m_bytesPerLine; }

BitmapImage::FileHeader const &BitmapImage::fileHeader() const { return m_fileHeader; }

BitmapImage::InfoHeader const &BitmapImage::infoHeader() const { return m_infoHeader; }