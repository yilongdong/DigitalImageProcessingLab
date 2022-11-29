#include "Config.h"
#include "Common/BMPImage.h"

void testMeanFilter(std::string const &input, std::string const &output);

void testMedianFilter(std::string const &input, std::string const &output);


int main() {
    std::string inputDir, input, outputDir, output;
    inputDir = RESOURCES_PATH(Lab3);
    outputDir = RESULT_PATH(Lab3);

    input = inputDir + "/噪声图像.bmp";
    output = outputDir + "/噪声图像.均值滤波.bmp";
    fprintf(stderr, "Lab 3: from %s\n", input.c_str());
    fprintf(stderr, "\tto %s\n", output.c_str());
    testMeanFilter(input, output);

    input = inputDir + "/噪声图像.bmp";
    output = outputDir + "/噪声图像.中值滤波.bmp";
    fprintf(stderr, "Lab 3: from %s\n", input.c_str());
    fprintf(stderr, "\tto %s\n", output.c_str());
    testMedianFilter(input, output);
    return 0;
}

uint8_t meanRGB(std::vector<uint8_t> const &pixels) {
    double sum = 0;
    for (uint8_t pixel: pixels) {
        sum += pixel;
    }
    return round(sum / pixels.size());
}

uint8_t medianRGB(std::vector<uint8_t> &pixels) {
    std::nth_element(pixels.begin(), pixels.begin() + (pixels.size() / 2), pixels.end());
    return *(pixels.begin() + (pixels.size() / 2));
}

void testMeanFilter(std::string const &input,
                    std::string const &output) {
    INFO("局部处理：均值滤波 from %s to %s", input.c_str(), output.c_str());
    // ffplay -f rawvideo -pixel_format yuv444p -s 129x129 ../asset/lena.yuv
    image::BitmapImage bitmapImage(input);

    int dx[] = {-1, -1, 1, 1, 0};
    int dy[] = {1, -1, 1, -1, 0};
    // 调整bmp图像的颜色分量
    int height = bitmapImage.height(), width = bitmapImage.width();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::vector<uint8_t> pixelsInNeighborhood;
            for (int k = 0; k < 5; ++k) {
                int m = i + dx[k];
                int n = j + dy[k];
                if (m < 0 || n < 0 || m >= height || n >= width) {
                    continue;
                }
                uint8_t pixel = bitmapImage.lines()[m][n];
                pixelsInNeighborhood.push_back(pixel);
            }
            bitmapImage.lines()[i][j] = meanRGB(pixelsInNeighborhood);
        }
    }
    bitmapImage.saveToFile(output);
}

void testMedianFilter(std::string const &input, std::string const &output) {
    INFO("局部处理：中值滤波 from %s to %s", input.c_str(), output.c_str());
    image::BitmapImage bitmapImage(input);

    int dx[] = {-1, -1, 1, 1, 0};
    int dy[] = {1, -1, 1, -1, 0};
    // 调整bmp图像的颜色分量
    int height = bitmapImage.height(), width = bitmapImage.width();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::vector<uint8_t> pixelsInNeighborhood;
            for (int k = 0; k < 5; ++k) {
                int m = i + dx[k];
                int n = j + dy[k];
                if (m < 0 || n < 0 || m >= height || n >= width) {
                    continue;
                }
                uint8_t pixel = bitmapImage.lines()[m][n];
                pixelsInNeighborhood.push_back(pixel);
            }
            bitmapImage.lines()[i][j] = medianRGB(pixelsInNeighborhood);
        }
    }
    bitmapImage.saveToFile(output);
}
