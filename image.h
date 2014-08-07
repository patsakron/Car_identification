#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <armadillo>

using namespace std;
using namespace arma;

namespace sense {

////////////////////////////////////////////////////////////////////////////////
// Color space.
////////////////////////////////////////////////////////////////////////////////

enum ColorSpace {
  COLORSPACE_RGB = 0,
  COLORSPACE_NORMALIZEDRGB = 1,  // Normalized R'G'B'
  COLORSPACE_XYZ = 2,
  COLORSPACE_LAB = 3,  // L*a*b*
  COLORSPACE_HSV = 4,
  COLORSPACE_YCBCR = 5  // Y'CbCr
};

////////////////////////////////////////////////////////////////////////////////
// Image classes.
////////////////////////////////////////////////////////////////////////////////

// Abstract base image in some color space.

template<typename eT>
class Image {
  public:
    u32 height;
    u32 width;
    virtual ColorSpace colorSpace() const = 0;
    virtual void setSize(const u32 newHeight, const u32 newWidth) = 0;
    virtual bool check() const = 0;
    virtual void print(ostream& stream) const = 0;
};

// Image in RGB color space.

template<typename eT>
class ImageRGB: public Image<eT> {
  public:
    Mat<eT> r;
    Mat<eT> g;
    Mat<eT> b;
    ImageRGB() { setSize(0, 0); }
    ImageRGB(const u32 height, const u32 width) { setSize(height, width); }
    ColorSpace colorSpace() const;
    void setSize(const u32 newHeight, const u32 newWidth);
    void fill(const eT r, const eT g, const eT b);
    bool check() const;
    void print(ostream& stream) const;
};

// Image in Normalized R'G'B' color space.

template<typename eT>
class ImageNormalizedRGB: public Image<eT> {
  public:
    Mat<eT> normalizedR;
    Mat<eT> normalizedG;
    Mat<eT> normalizedB;
    ImageNormalizedRGB() { setSize(0, 0); }
    ImageNormalizedRGB(const u32 height, const u32 width) { setSize(height, width); }
    ColorSpace colorSpace() const;
    void setSize(const u32 newHeight, const u32 newWidth);
    void fill(const eT normalizedR, const eT normalizedG, const eT normalizedB);
    bool check() const;
    void print(ostream& stream) const;
};

// Image in XYZ color space.

template<typename eT>
class ImageXYZ: public Image<eT> {
  public:
    Mat<eT> x;
    Mat<eT> y;
    Mat<eT> z;
    ImageXYZ() { setSize(0, 0); }
    ImageXYZ(const u32 height, const u32 width) { setSize(height, width); }
    ColorSpace colorSpace() const;
    void setSize(const u32 newHeight, const u32 newWidth);
    void fill(const eT x, const eT y, const eT z);
    bool check() const;
    void print(ostream& stream) const;
};

// Image in L*a*b* color space.

template<typename eT>
class ImageLAB: public Image<eT> {
  public:
    Mat<eT> l;
    Mat<eT> a;
    Mat<eT> b;
    ImageLAB() { setSize(0, 0); }
    ImageLAB(const u32 height, const u32 width) { setSize(height, width); }
    ColorSpace colorSpace() const;
    void setSize(const u32 newHeight, const u32 newWidth);
    void fill(const eT l, const eT a, const eT b);
    bool check() const;
    void print(ostream& stream) const;
};

// Image in HSV color space.

template<typename eT>
class ImageHSV: public Image<eT> {
  public:
    Mat<eT> h;
    Mat<eT> s;
    Mat<eT> v;
    ImageHSV() { setSize(0, 0); }
    ImageHSV(const u32 height, const u32 width) { setSize(height, width); }
    ColorSpace colorSpace() const;
    void setSize(const u32 newHeight, const u32 newWidth);
    void fill(const eT h, const eT s, const eT v);
    bool check() const;
    void print(ostream& stream) const;
};

// Image in Y'CbCr color space.

template<typename eT>
class ImageYCbCr: public Image<eT> {
  public:
    Mat<eT> y;
    Mat<eT> cb;
    Mat<eT> cr;
    ImageYCbCr() { setSize(0, 0); }
    ImageYCbCr(const u32 height, const u32 width) { setSize(height, width); }
    ColorSpace colorSpace() const;
    void setSize(const u32 newHeight, const u32 newWidth);
    void fill(const eT y, const eT cb, const eT cr);
    bool check() const;
    void print(ostream& stream) const;
};

////////////////////////////////////////////////////////////////////////////////
// Overloaded operators.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
ostream& operator<<(ostream& stream, const Image<eT>& image);

////////////////////////////////////////////////////////////////////////////////
// Functions to load and save images.
////////////////////////////////////////////////////////////////////////////////

// Load and save color images in the form of Image<eT> objects.

template<typename eT>
bool load(ImageRGB<eT>& image, const string& path);

template<typename eT>
bool load(Image<eT>& image, const string& path);

template<typename eT>
bool save(ImageRGB<eT>& image, const string& path);

template<typename eT>
bool save(const Image<eT>& image, const string& path);

// Load and save grayscale images in the form of Mat<eT> objects.

template<typename eT>
bool load(Mat<eT>& mat, const string& path);

template<typename eT>
bool save(const Mat<eT>& mat, const string& path);

////////////////////////////////////////////////////////////////////////////////
// Functions to resize images.
////////////////////////////////////////////////////////////////////////////////

// Resize color images in the form of ImageRGB<eT> objects.

template<typename eT>
bool resize(ImageRGB<eT>& imageOut, const ImageRGB<eT>& imageIn,
            const u32 height, const u32 width);

// Resize grayscale images in the form of Mat<eT> objects.

template<typename eT>
bool resize(Mat<eT>& matOut, const Mat<eT>& matIn,
            const u32 height, const u32 width);

////////////////////////////////////////////////////////////////////////////////
// Functions to crop images.
////////////////////////////////////////////////////////////////////////////////

// Crop color images in the form of ImageRGB<eT> objects.

template<typename eT>
bool crop(ImageRGB<eT>& imageOut, const ImageRGB<eT>& imageIn,
          const u32 yOffset, const u32 xOffset, const u32 height, const u32 width);

// Crop grayscale images in the form of Mat<eT> objects.

template<typename eT>
bool crop(Mat<eT>& matOut, const Mat<eT>& matIn,
          const u32 yOffset, const u32 xOffset, const u32 height, const u32 width);

////////////////////////////////////////////////////////////////////////////////
// Functions to threshold images.
////////////////////////////////////////////////////////////////////////////////

// Threshold images in the form of Mat<eT> objects.

template<typename eT>
bool threshold(Mat<eT>& matOut, const Mat<eT>& matIn,
               const eT cutoff, const eT belowCutoffValue = 0, const eT aboveCutoffValue = 255);

////////////////////////////////////////////////////////////////////////////////
// Functions to convert color space of Image objects.
////////////////////////////////////////////////////////////////////////////////

// Convert other color space to RGB.

template<typename eT>
void convert(ImageRGB<eT>& imageOut, const ImageNormalizedRGB<eT>& imageIn);

template<typename eT>
void convert(ImageRGB<eT>& imageOut, const ImageXYZ<eT>& imageIn);

template<typename eT>
void convert(ImageRGB<eT>& imageOut, const ImageLAB<eT>& imageIn);

template<typename eT>
void convert(ImageRGB<eT>& imageOut, const ImageHSV<eT>& imageIn);

template<typename eT>
void convert(ImageRGB<eT>& imageOut, const ImageYCbCr<eT>& imageIn);

template<typename eT>
void convert(ImageRGB<eT>& imageOut, const Image<eT>& imageIn);

// Convert RGB to other color space.

template<typename eT>
void convert(ImageNormalizedRGB<eT>& imageOut, const ImageRGB<eT>& imageIn);

template<typename eT>
void convert(ImageXYZ<eT>& imageOut, const ImageRGB<eT>& imageIn);

template<typename eT>
void convert(ImageLAB<eT>& imageOut, const ImageRGB<eT>& imageIn);

template<typename eT>
void convert(ImageHSV<eT>& imageOut, const ImageRGB<eT>& imageIn);

template<typename eT>
void convert(ImageYCbCr<eT>& imageOut, const ImageRGB<eT>& imageIn);

template<typename eT>
void convert(Image<eT>& imageOut, const ImageRGB<eT>& imageIn);

// Convert any color space to any color space.

template<typename eT>
void convert(Image<eT>& imageOut, const Image<eT>& imageIn);

// Convert grayscale to any color space.

template<typename eT>
void convert(ImageRGB<eT>& imageOut, const Mat<eT>& matIn);

template<typename eT>
void convert(Image<eT>& imageOut, const Mat<eT>& matIn);

// Convert any color space to grayscale.

template<typename eT>
void convert(Mat<eT>& matOut, const ImageRGB<eT>& imageIn);

template<typename eT>
void convert(Mat<eT>& matOut, const Image<eT>& imageIn);

}  /* namespace sense */

#include "image_impl.h"

#endif  /* __IMAGE_H__ */
