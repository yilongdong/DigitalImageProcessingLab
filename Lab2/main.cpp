#include "Config.h"
#include "Common/BMPImage.h"

void testGrayscaleInversion(std::string const &input, std::string const &output);

void testGammaCorrection(std::string const &input, std::string const &output, double gamma);

int main() {
    std::string inputDir, input, outputDir, output;
    inputDir = RESOURCES_PATH(Lab2);
    outputDir = RESULT_PATH(Lab2);

    input = inputDir + "/灰度变换.bmp";
    output = outputDir + "/灰度变换.灰度反转.bmp";
    fprintf(stderr, "Lab 2: from %s\n", input.c_str());
    fprintf(stderr, "\tto %s\n", output.c_str());
    testGrayscaleInversion(input, output);

    input = inputDir + "/伽马校正.bmp";
    output = outputDir + "/伽马校正.gamma=0.4.bmp";
    fprintf(stderr, "Lab 2: from %s\n", input.c_str());
    fprintf(stderr, "\tto %s\n", output.c_str());
    testGammaCorrection(input, output, 0.4);

    output = outputDir + "/伽马校正.gamma=0.6.bmp";
    fprintf(stderr, "Lab 2: from %s\n", input.c_str());
    fprintf(stderr, "\tto %s\n", output.c_str());
    testGammaCorrection(input, output, 0.6);

    output = outputDir + "/伽马校正.gamma=0.8.bmp";
    fprintf(stderr, "Lab 2: from %s\n", input.c_str());
    fprintf(stderr, "\tto %s\n", output.c_str());
    testGammaCorrection(input, output, 0.8);
    return 0;
}
void testGrayscaleInversion(std::string const &input,
                            std::string const &output) {
    INFO("点处理：灰度反转变换 from %s to %s", input.c_str(), output.c_str());
    // ffplay -f rawvideo -pixel_format yuv444p -s 129x129 ../asset/lena.yuv
    image::BitmapImage bitmapImage(input);

    using BGR24 = image::colorspace::BGR24;

//    uint32_t pixelsNum = bitmapImage.width() * bitmapImage.height();
//    std::vector<uint8_t> YPlanar(pixelsNum);
//    std::vector<uint8_t> UPlanar(pixelsNum);
//    std::vector<uint8_t> VPlanar(pixelsNum);
    for (int i = 0; i < bitmapImage.height(); ++i) {
        BGR24 *rgbLine = (BGR24 *)(bitmapImage.lines().at(bitmapImage.height() - 1 - i));
        for (uint32_t j = 0; j < bitmapImage.width(); ++j) {
            BGR24 &rgb = rgbLine[j];
            int Y,U,V;
            std::tie(Y,U,V) = RGB2YUV(rgb.R, rgb.G, rgb.B);
            Y = 235 + 16 - Y;
            std::tie(rgb.R, rgb.G, rgb.B) = YUV2RGB(Y, U, V);
//            YPlanar[(i * bitmapImage.width()) + j] = Y;
//            UPlanar[(i * bitmapImage.width()) + j] = U;
//            VPlanar[(i * bitmapImage.width()) + j] = V;
        }
    }
    bitmapImage.saveToFile(output);


//    FILE *fp = fopen(output.c_str(), "w");
//    fwrite(YPlanar.data(), 1, YPlanar.size(), fp);
//    fwrite(UPlanar.data(), 1, UPlanar.size(), fp);
//    fwrite(VPlanar.data(), 1, VPlanar.size(), fp);
//    fclose(fp);
}

void testGammaCorrection(std::string const &input, std::string const &output,
                         double gamma) {
    INFO("点处理：伽马校正变换 from %s to %s", input.c_str(), output.c_str());
    image::BitmapImage bitmapImage(input);

    // 调整bmp图像的颜色分量
    using BGR24 = image::colorspace::BGR24;
    uint32_t pixelPerLine = bitmapImage.bytesPerLine() / sizeof(BGR24);
    for (int i = 0; i < bitmapImage.height(); ++i) {
        BGR24 *pixels = (BGR24 *)(bitmapImage.line(i));
        for (uint32_t j = 0; j < pixelPerLine; ++j) {
            pixels[j].R = pow(1.0 * pixels[j].R / 255.0 , 1.0 / gamma) * 255;
            pixels[j].G = pow(1.0 * pixels[j].G / 255.0 , 1.0 / gamma) * 255;
            pixels[j].B = pow(1.0 * pixels[j].B / 255.0 , 1.0 / gamma) * 255;
        }
    }

    bitmapImage.saveToFile(output);
}