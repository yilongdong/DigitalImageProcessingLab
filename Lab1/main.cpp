#include "Config.h"
#include "Common/BMPImage.h"
#include <algorithm>

void testChangeBMPColor(std::string const &input, std::string const &output);

void testRGB2YUV444Planar24bit(std::string const &input,
                               std::string const &output);

void testRGB2YUV444Packed24bit(std::string const &input,
                               std::string const &output);

int main() {
    std::string input, outputDir, output;
    input = RESOURCES_PATH(Lab1) + "/lena.bmp";
    outputDir = RESULT_PATH(Lab1);

    {
        image::BitmapImage bitmapImage(input);
        fprintf(stderr, "image witdh = %u, height = %u\n", bitmapImage.width(), bitmapImage.height());
    }

    fprintf(stderr, "Lab 1: from %s\n", input.c_str());

    output = outputDir + "/lena.onlyR.bmp";
    fprintf(stderr, "\tto %s\n", output.c_str());
    testChangeBMPColor(input, output);

    output = outputDir + "/lena.yuv444planar24bit.yuv";
    fprintf(stderr, "\tto %s\n", output.c_str());
    testRGB2YUV444Planar24bit(input, output);

    output = outputDir + "/lena.yuv444packed24bit.yuv";
    fprintf(stderr, "\tto %s\n", output.c_str());
    testRGB2YUV444Packed24bit(input, output);
    return 0;
}

void testChangeBMPColor(std::string const &input, std::string const &output) {
    image::BitmapImage bitmapImage(input);

    // 调整bmp图像的颜色分量
    using BGR24 = image::colorspace::BGR24;
    uint32_t pixelPerLine = bitmapImage.bytesPerLine() / sizeof(BGR24);
    for (int i = 0; i < bitmapImage.height(); ++i) {
        BGR24 *pixels = (BGR24 *) (bitmapImage.line(i));
        for (uint32_t j = 0; j < pixelPerLine; ++j) {
            pixels[j].B = 0;
            pixels[j].G = 0;
        }
    }

    bitmapImage.saveToFile(output);
}

void testRGB2YUV444Planar24bit(std::string const &input,
                               std::string const &output) {
    // ffplay -f rawvideo -pixel_format yuv444p -s 129x129 ../asset/lena.yuv
    image::BitmapImage bitmapImage(input);
    using BGR24 = image::colorspace::BGR24;

    uint32_t pixelsNum = bitmapImage.width() * bitmapImage.height();
    std::vector<uint8_t> YPlanar(pixelsNum);
    std::vector<uint8_t> UPlanar(pixelsNum);
    std::vector<uint8_t> VPlanar(pixelsNum);
    for (int i = 0; i < bitmapImage.height(); ++i) {
        BGR24 *rgbLine = (BGR24 *)(bitmapImage.lines().at(bitmapImage.height() - 1 - i));
        for (uint32_t j = 0; j < bitmapImage.width(); ++j) {
            BGR24 &rgb = rgbLine[j];
            uint8_t Y, U, V;
            std::tie(Y, U, V) = RGB2YUV(rgb.R, rgb.G, rgb.B);
            YPlanar[(i * bitmapImage.width()) + j] = Y;
            UPlanar[(i * bitmapImage.width()) + j] = U;
            VPlanar[(i * bitmapImage.width()) + j] = V;
        }
    }


    FILE *fp = fopen(output.c_str(), "w");
    fwrite(YPlanar.data(), 1, YPlanar.size(), fp);
    fwrite(UPlanar.data(), 1, UPlanar.size(), fp);
    fwrite(VPlanar.data(), 1, VPlanar.size(), fp);
    fclose(fp);
}

void testRGB2YUV444Packed24bit(std::string const &input,
                               std::string const &output) {
    // 这个格式需要使用yuv工具查看，但是查看后发现显示两帧，一帧全绿色，一帧正常lena，原因暂不清楚
    image::BitmapImage bitmapImage(input);
    // 计算YUV颜色分量
    using BGR24 = image::colorspace::BGR24;
    using YUV = image::colorspace::YUVPacked;
    std::vector<YUV> YUVPackedData(bitmapImage.height() * bitmapImage.width());
    for (int i = bitmapImage.height() - 1; i >= 0; --i) {
        BGR24 *rgbLine = (BGR24 *)(bitmapImage.lines().at(i));
        for (uint32_t j = 0; j < bitmapImage.width(); ++j) {
            BGR24 &rgb = rgbLine[j];
            uint8_t Y, U, V;
            std::tie(Y, U, V) = RGB2YUV(rgb.R, rgb.G, rgb.B);
            YUV yuv { .Y = Y, .U = U, .V = V };
            YUVPackedData.push_back(yuv);
        }
    }

    FILE *fp = fopen(output.c_str(), "w");
    fwrite(YUVPackedData.data(), sizeof(YUV), YUVPackedData.size(), fp);
    fclose(fp);
}