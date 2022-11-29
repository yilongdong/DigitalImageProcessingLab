#include "Config.h"
#include "Common/BMPImage.h"


void testLaplacianFilter(std::string const &input, std::string const &output);

int main() {
    std::string inputDir, input, outputDir, output;
    inputDir = RESOURCES_PATH(Lab4);
    outputDir = RESULT_PATH(Lab4);

    input = inputDir + "/梯度算子.bmp";
    output = outputDir + "/梯度算子.Laplacian锐化.bmp";
    fprintf(stderr, "Lab 4: from %s\n", input.c_str());
    fprintf(stderr, "\tto %s\n", output.c_str());
    testLaplacianFilter(input, output);

    return 0;
}

using matrix_t = std::vector<std::vector<int>>;
int Laplacian3x3(matrix_t const& pixels) {
    int result = 0;
    static matrix_t LaplacianTemplate1 = {
            { 0, -1, 0},
            { -1, 4, -1},
            { 0, -1, 0},
    };
    static matrix_t LaplacianTemplate2 = {
            { -1, -1, -1},
            { -1, 8, -1},
            { -1, -1, -1},
    };
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result += pixels[i][j] * LaplacianTemplate2[i][j];
        }
    }
    return result;
}

void testLaplacianFilter(std::string const &input,
                         std::string const &output) {
    INFO("局部处理：Laplacian锐化 from %s to %s", input.c_str(), output.c_str());
    // ffplay -f rawvideo -pixel_format yuv444p -s 129x129 ../asset/lena.yuv
    image::BitmapImage bitmapImage(input);
    image::BitmapImage newBitmapImage(input);
//    printf("%s", bitmapImage.strFileHeader().c_str());
//    printf("%s", bitmapImage.strInfoHeader().c_str());

    int dx[] = { -1, -1, 1, 1 , 0};
    int dy[] = { 1, -1, 1, -1 , 0};
    // 调整bmp图像的颜色分量
    int height = bitmapImage.height(), width = bitmapImage.width();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            matrix_t pixels = { {0,0,0}, {0,0,0}, {0,0,0} };
            for (int k = 0; k < 5; ++k) {
                int m = i + dx[k];
                int n = j + dy[k];
                if (m < 0) m = 0;
                if (n < 0) n = 0;
                if (m >= height) m = height - 1;
                if (n >= width) m = width - 1;
                pixels[1 + dx[k]][1 + dy[k]] = bitmapImage.lines()[m][n];
            }
            int grapScale = 0.45 * Laplacian3x3(pixels);
            newBitmapImage.lines()[i][j] = bound(0, grapScale, 255);
        }
    }
    newBitmapImage.saveToFile(output);
}