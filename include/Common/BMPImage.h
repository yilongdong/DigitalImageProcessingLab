#include <stdint.h>
#include <string>
#include <vector>
#include "Common/Util.h"

namespace image {

    namespace colorspace {

#pragma pack(push, 1)
        struct BGR24 {
            uint8_t B;
            uint8_t G;
            uint8_t R;
        };

        struct RGB24 {
            uint8_t R;
            uint8_t G;
            uint8_t B;
        };

        struct YUV {
            uint8_t Y;
            uint8_t U;
            uint8_t V;
        };
        using YUVPacked = YUV;

#pragma pack(pop)

    }

// 此处为了防止按4字节对齐
#pragma pack(push, 2)
    struct BitmapFileHeader {
        uint16_t bfType;      /* 文件类型，一般是BM */
        uint32_t bfSize;      /* 整个BMP文件的大小 */
        uint16_t bfReserved1; /* 保留字 = 0 */
        uint16_t bfReserved2; /* 保留字 = 0 */
        uint32_t bfOffBytes; /* 偏移数，即位图文件头+位图信息头+调色板的大小 */
    };
#pragma pack(pop)

#pragma pack(push, 4)
    struct BitmapInfoHeader {
        uint32_t biSize;  /* Size of info header */
        uint32_t biWidth; /* Width of image */
        /*
          biHeight 为正，位图自底向顶扫描，
          biHeight 为负，位图自顶向底扫描。
          如果这个值的设置和原始位图文件扫描方式不符，则图像显示可能会颠倒。
        */
        int32_t biHeight;    /* Height of image */
        uint16_t biPlanes;   /* Number of color planes */
        uint16_t biBitCount; /* Number of bits per pixel */
        /* 当biCompression成员的值是BI_RGB(0)时，它没有调色板 */
        uint32_t biCompression;   /* Type of compression to use */
        uint32_t biSizeImage;     /* Size of image data */
        uint32_t biXPelsPerMeter; /* X pixels per meter */
        uint32_t biYPelsPerMeter; /* Y pixels per meter */
        uint32_t biClrUsed;       /* Number of colors used */
        uint32_t biClrImportant;  /* Number of important colors */
    };
#pragma pack(pop)

    class BitmapImage final {
    public:
        using FileHeader = struct BitmapFileHeader;
        using InfoHeader = struct BitmapInfoHeader;

        BitmapImage() = delete;

        BitmapImage(std::string const &filename);

        ~BitmapImage();

        void loadFromFile(std::string const &filename);

        void saveToFile(std::string const &filename);

        uint32_t width() const;

        int32_t height() const;

        uint8_t *data() const;

        uint8_t *line(uint32_t index) const;

        std::vector<uint8_t *> &lines();

        uint32_t bytesPerLine() const;

        FileHeader const &fileHeader() const;

        InfoHeader const &infoHeader() const;

        std::string strFileHeader() const;

        std::string strInfoHeader() const;

    private:
        struct BitmapFileHeader m_fileHeader;
        struct BitmapInfoHeader m_infoHeader;
        uint8_t *m_colorPanel;
        uint32_t m_colorPanelSize;
        uint8_t *m_data;
        std::vector<uint8_t *> m_lines;
        uint32_t m_bytesPerLine;
    };

} // namespace image