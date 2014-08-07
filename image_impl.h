#ifndef __IMAGE_IMPL_H__
#define __IMAGE_IMPL_H__

#include <exception>

// NOTE: The following include must be outside the "sense" namespace.
#include <Magick++.h>

namespace sense {

////////////////////////////////////////////////////////////////////////////////
// Helper functions.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
bool checkSize(const Mat<eT>& mat, const u32 height, const u32 width) {
  return ((mat.n_rows == height) && (mat.n_cols == width));
}

// Convert Magick++ image to SENSE image.
template<typename eT>
void convert(ImageRGB<eT>& image, /* const */ Magick::Image& magickImage) {
  const u32 height = magickImage.rows();
  const u32 width  = magickImage.columns();
  Magick::PixelPacket* magickPixels = magickImage.getPixels(0, 0, width, height);
  image.setSize(height, width);
  for (u32 y = 0; y < height; y++)
  for (u32 x = 0; x < width; x++)
  {
    Magick::Color magickColor = magickPixels[width * y + x];
    Magick::ColorRGB magickRgb(magickColor);
    image.r(y, x) = (eT)round(magickRgb.red() * 255.0);
    image.g(y, x) = (eT)round(magickRgb.green() * 255.0);
    image.b(y, x) = (eT)round(magickRgb.blue() * 255.0);
  }
}

// Convert SENSE image to Magick++ image.
template<typename eT>
void convert(Magick::Image& magickImage, const ImageRGB<eT>& image) {
  const u32 height = image.height;
  const u32 width = image.width;
  Magick::Geometry magickGeometry(width, height);
  magickImage.size(magickGeometry);
  Magick::PixelPacket* magickPixels = magickImage.getPixels(0, 0, width, height);
  for (u32 y = 0; y < height; y++)
  for (u32 x = 0; x < width; x++)
  {
    Magick::ColorRGB magickColor(image.r(y, x) / 255.0, image.g(y, x) / 255.0, image.b(y, x) / 255.0);
    magickPixels[width * y + x] = magickColor;
  }
  magickImage.syncPixels();
}

// Convert Magick++ image to grayscale image.
template<typename eT>
void convert(Mat<eT>& mat, /* const */ Magick::Image& magickImage) {
  const u32 height = magickImage.rows();
  const u32 width  = magickImage.columns();

  Magick::PixelPacket* magickPixels = magickImage.getPixels(0, 0, width, height);
  mat.set_size(height, width);
  for (u32 y = 0; y < height; y++)
  for (u32 x = 0; x < width; x++)
  {
    Magick::Color magickColor = magickPixels[width * y + x];
    Magick::ColorGray magickGray(magickColor);
    mat(y, x) = (eT)round(magickGray.shade() * 255.0);
  }
}

// Convert grayscale image to Magick++ image.
template<typename eT>
void convert(Magick::Image& magickImage, const Mat<eT>& mat) {
  const u32 height = mat.n_rows;
  const u32 width = mat.n_cols;
  Magick::Geometry magickGeometry(width, height);
  magickImage.size(magickGeometry);
  Magick::PixelPacket* magickPixels = magickImage.getPixels(0, 0, width, height);
  for (u32 y = 0; y < height; y++)
  for (u32 x = 0; x < width; x++)
  {
    Magick::ColorGray magickColor(mat(y, x) / 255.0);
    magickPixels[width * y + x] = magickColor;
  }
  magickImage.syncPixels();
}

////////////////////////////////////////////////////////////////////////////////
// ImageRGB implementation.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
ColorSpace ImageRGB<eT>::colorSpace() const {
  return COLORSPACE_RGB;
}

template<typename eT>
void ImageRGB<eT>::setSize(const u32 newHeight, const u32 newWidth) {
  this->height = newHeight;
  this->width = newWidth;
  r.set_size(newHeight, newWidth);
  g.set_size(newHeight, newWidth);
  b.set_size(newHeight, newWidth);
}

template<typename eT>
void ImageRGB<eT>::fill(const eT r, const eT g, const eT b) {
  r.fill(r);
  g.fill(g);
  b.fill(b);
}

template<typename eT>
bool ImageRGB<eT>::check() const {
  return (checkSize(r, this->height, this->width) && checkSize(g, this->height, this->width) && checkSize(b, this->height, this->width));
}

template<typename eT>
void ImageRGB<eT>::print(ostream& stream) const {
  stream << "Height: " << this->height << endl;
  stream << "Width : " << this->width  << endl;
  stream << "R: " << endl << r << endl;
  stream << "G: " << endl << g << endl;
  stream << "B: " << endl << b << endl;
}

////////////////////////////////////////////////////////////////////////////////
// ImageNormalizedRGB implementation.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
ColorSpace ImageNormalizedRGB<eT>::colorSpace() const {
  return COLORSPACE_NORMALIZEDRGB;
}

template<typename eT>
void ImageNormalizedRGB<eT>::setSize(const u32 newHeight, const u32 newWidth) {
  this->height = newHeight;
  this->width  = newWidth;
  normalizedR.set_size(newHeight, newWidth);
  normalizedG.set_size(newHeight, newWidth);
  normalizedB.set_size(newHeight, newWidth);
}

template<typename eT>
void ImageNormalizedRGB<eT>::fill(const eT normalizedR, const eT normalizedG, const eT normalizedB) {
  normalizedR.fill(normalizedR);
  normalizedG.fill(normalizedG);
  normalizedB.fill(normalizedB);
}

template<typename eT>
bool ImageNormalizedRGB<eT>::check() const {
  return (checkSize(normalizedR, this->height, this->width) &&
          checkSize(normalizedG, this->height, this->width) &&
          checkSize(normalizedB, this->height, this->width));
}

template<typename eT>
void ImageNormalizedRGB<eT>::print(ostream& stream) const {
  stream << "Height: " << this->height << endl;
  stream << "Width : " << this->width  << endl;
  stream << "Normalized R': " << endl << normalizedR << endl;
  stream << "Normalized G': " << endl << normalizedG << endl;
  stream << "Normalized B': " << endl << normalizedB << endl;
}

////////////////////////////////////////////////////////////////////////////////
// ImageXYZ implementation.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
ColorSpace ImageXYZ<eT>::colorSpace() const {
  return COLORSPACE_XYZ;
}

template<typename eT>
void ImageXYZ<eT>::setSize(const u32 newHeight, const u32 newWidth) {
  this->height = newHeight;
  this->width  = newWidth;
  x.set_size(newHeight, newWidth);
  y.set_size(newHeight, newWidth);
  z.set_size(newHeight, newWidth);
}

template<typename eT>
void ImageXYZ<eT>::fill(const eT x, const eT y, const eT z) {
  x.fill(x);
  y.fill(y);
  z.fill(z);
}

template<typename eT>
bool ImageXYZ<eT>::check() const {
  return (checkSize(x, this->height, this->width) &&
          checkSize(y, this->height, this->width) &&
          checkSize(z, this->height, this->width));
}

template<typename eT>
void ImageXYZ<eT>::print(ostream& stream) const {
  stream << "Height: " << this->height << endl;
  stream << "Width : " << this->width  << endl;
  stream << "X: " << endl << x << endl;
  stream << "Y: " << endl << y << endl;
  stream << "Z: " << endl << z << endl;
}

////////////////////////////////////////////////////////////////////////////////
// ImageLAB implementation.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
ColorSpace ImageLAB<eT>::colorSpace() const {
  return COLORSPACE_LAB;
}

template<typename eT>
void ImageLAB<eT>::setSize(const u32 newHeight, const u32 newWidth) {
  this->height = newHeight;
  this->width  = newWidth;
  l.set_size(newHeight, newWidth);
  a.set_size(newHeight, newWidth);
  b.set_size(newHeight, newWidth);
}

template<typename eT>
void ImageLAB<eT>::fill(const eT l, const eT a, const eT b) {
  l.fill(l);
  a.fill(a);
  b.fill(b);
}

template<typename eT>
bool ImageLAB<eT>::check() const {
  return (checkSize(l, this->height, this->width) &&
          checkSize(a, this->height, this->width) &&
          checkSize(b, this->height, this->width));
}

template<typename eT>
void ImageLAB<eT>::print(ostream& stream) const {
  stream << "Height: " << this->height << endl;
  stream << "Width : " << this->width  << endl;
  stream << "L*: " << endl << l << endl;
  stream << "a*: " << endl << a << endl;
  stream << "b*: " << endl << b << endl;
}

////////////////////////////////////////////////////////////////////////////////
// ImageHSV implementation.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
ColorSpace ImageHSV<eT>::colorSpace() const {
  return COLORSPACE_HSV;
}

template<typename eT>
void ImageHSV<eT>::setSize(const u32 newHeight, const u32 newWidth) {
  this->height = newHeight;
  this->width  = newWidth;
  h.set_size(newHeight, newWidth);
  s.set_size(newHeight, newWidth);
  v.set_size(newHeight, newWidth);
}

template<typename eT>
void ImageHSV<eT>::fill(const eT h, const eT s, const eT v) {
  h.fill(h);
  s.fill(s);
  v.fill(v);
}

template<typename eT>
bool ImageHSV<eT>::check() const {
  return (checkSize(h, this->height, this->width) && checkSize(s, this->height, this->width) && checkSize(v, this->height, this->width));
}

template<typename eT>
void ImageHSV<eT>::print(ostream& stream) const {
  stream << "Height: " << this->height << endl;
  stream << "Width : " << this->width  << endl;
  stream << "H: " << endl << h << endl;
  stream << "S: " << endl << s << endl;
  stream << "V: " << endl << v << endl;
}

////////////////////////////////////////////////////////////////////////////////
// ImageYCbCr implementation.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
ColorSpace ImageYCbCr<eT>::colorSpace() const {
  return COLORSPACE_YCBCR;
}

template<typename eT>
void ImageYCbCr<eT>::setSize(const u32 newHeight, const u32 newWidth) {
  this->height = newHeight;
  this->width  = newWidth;
  y.set_size(newHeight, newWidth);
  cb.set_size(newHeight, newWidth);
  cr.set_size(newHeight, newWidth);
}

template<typename eT>
void ImageYCbCr<eT>::fill(const eT y, const eT cb, const eT cr) {
  y.fill(y);
  cb.fill(cb);
  cr.fill(cr);
}

template<typename eT>
bool ImageYCbCr<eT>::check() const {
  return (checkSize(y, this->height, this->width) && checkSize(cb, this->height, this->width) && checkSize(cr, this->height, this->width));
}

template<typename eT>
void ImageYCbCr<eT>::print(ostream& stream) const {
  stream << "Height: " << this->height << endl;
  stream << "Width : " << this->width  << endl;
  stream << "Y': " << endl << y  << endl;
  stream << "Cb: " << endl << cb << endl;
  stream << "Cr: " << endl << cr << endl;
}

////////////////////////////////////////////////////////////////////////////////
// Overloaded operators.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
ostream& operator<<(ostream& stream, const Image<eT>& image) {
  image.print(stream);
  return stream;
}

////////////////////////////////////////////////////////////////////////////////
// Functions to load and save color images in the form of Image<eT> objects.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
bool load(ImageRGB<eT>& image, const string& path) {
  // Initialize Magick++.
  Magick::InitializeMagick(NULL);

  // Load Magick++ image.
  Magick::Image magickImage;
  try {
    magickImage.read(path);
  }
  catch (const Magick::Error& error) {
    return false;
  }

  // Convert Magick++ image to SENSE image.
  convert(image, magickImage);

  return true;
}

template<typename eT>
bool load(Image<eT>& image, const string& path) {
  if (image.colorSpace() == COLORSPACE_RGB) {
    return load(static_cast<ImageRGB<eT>&>(image), path);
  }
  else {
    ImageRGB<eT> imageRgb;
    if (!load(imageRgb, path))
      return false;
    convert(image, imageRgb);
    return true;
  }
}

template<typename eT>
bool save(ImageRGB<eT>& image, const string& path) {
  if (!image.check())
    throw logic_error("Inconsistent height and width in image");
  if (image.height == 0 || image.width == 0)
    return false;

  // Initialize Magick++.
  Magick::InitializeMagick(NULL);

  // Convert SENSE image to Magick++ image.
  Magick::Image magickImage;
  convert(magickImage, image);

  // Save Magick++ image.
  try {
    magickImage.write(path);
  }
  catch (const Magick::Error& error) {
    return false;
  }

  return true;
}

template<typename eT>
bool save(const Image<eT>& image, const string& path) {
  if (image.colorSpace() == COLORSPACE_RGB) {
    return save(static_cast<const ImageRGB<eT>&>(image), path);
  }
  else {
    ImageRGB<eT> imageRgb;
    convert(imageRgb, image);
    return save(imageRgb, path);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Functions to load and save grayscale images in the form of Mat<eT> objects.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
bool load(Mat<eT>& mat, const string& path) {
  // Initialize Magick++.
  Magick::InitializeMagick(NULL);

  // Load Magick++ image.
  Magick::Image magickImage;
  try {
    magickImage.read(path);
  }
  catch (const Magick::Error& error) {
    return false;
  }

  // Convert Magick++ image to grayscale image.
  convert(mat, magickImage);

  return true;
}

template<typename eT>
bool save(const Mat<eT>& mat, const string& path) {
  // Initialize Magick++.
  Magick::InitializeMagick(NULL);

  // Convert grayscale image to Magick++ image.
  Magick::Image magickImage;
  convert(magickImage, mat);

  // Save Magick++ image.
  try {
    magickImage.write(path);
  }
  catch (const Magick::Error& error) {
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Resize color images in the form of ImageRGB<eT> objects.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
bool resize(ImageRGB<eT>& imageOut, const ImageRGB<eT>& imageIn,
            const u32 height, const u32 width) {
  if (!imageIn.check())
    throw logic_error("Inconsistent height and width in imageIn");

  // Special handling for zero height or width.
  if (height == 0 || width == 0) {
    imageOut.setSize(0, 0);
    return true;
  }

  // Convert SENSE image to Magick++ image.
  Magick::Image magickImage;
  convert(magickImage, imageIn);

  // Resize image.
  Magick::Geometry magickGeometry(width, height);
  magickGeometry.aspect(true);
  magickImage.resize(magickGeometry);

  // Convert Magick++ image to SENSE image.
  convert(imageOut, magickImage);

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Resize grayscale images in the form of Mat<eT> objects.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
bool resize(Mat<eT>& matOut, const Mat<eT>& matIn,
            const u32 height, const u32 width) {
  // Special handling for zero height or width.
  if (height == 0 || width == 0) {
    matOut.set_size(0, 0);
    return true;
  }

  // Convert grayscale image to Magick++ image.
  Magick::Image magickImage;
  convert(magickImage, matIn);

  // Resize image.
  Magick::Geometry magickGeometry(width, height);
  magickGeometry.aspect(true);
  magickImage.resize(magickGeometry);

  // Convert Magick++ image to grayscale image.
  convert(matOut, magickImage);

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Crop color images in the form of ImageRGB<eT> objects.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
bool crop(ImageRGB<eT>& imageOut, const ImageRGB<eT>& imageIn,
          const u32 yOffset, const u32 xOffset, const u32 height, const u32 width) {
  if (!imageIn.check())
    throw logic_error("Inconsistent height and width in imageIn");
  if (yOffset + height > imageIn.height || xOffset + width > imageIn.width)
    return false;

  // Special handling for zero height or width.
  if (height == 0 || width == 0) {
    imageOut.setSize(0, 0);
    return true;
  }

  // Convert SENSE image to Magick++ image.
  Magick::Image magickImage;
  convert(magickImage, imageIn);

  // Crop image.
  Magick::Geometry magickGeometry(width, height, xOffset, yOffset);
  magickImage.crop(magickGeometry);

  // Convert Magick++ image to SENSE image.
  convert(imageOut, magickImage);

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Crop grayscale images in the form of Mat<eT> objects.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
bool crop(Mat<eT>& matOut, const Mat<eT>& matIn,
          const u32 yOffset, const u32 xOffset, const u32 height, const u32 width) {
  if (yOffset + height > matIn.n_rows || xOffset + width > matIn.n_cols)
    return false;

  // Special handling for zero height or width.
  if (height == 0 || width == 0) {
    matOut.set_size(0, 0);
    return true;
  }

  // Convert grayscale image to Magick++ image.
  Magick::Image magickImage;
  convert(magickImage, matIn);

  // Crop image.
  Magick::Geometry magickGeometry(width, height, xOffset, yOffset);
  magickImage.crop(magickGeometry);

  // Convert Magick++ image to grayscale image.
  convert(matOut, magickImage);

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Threshold images in the form of Mat<eT> objects.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
bool threshold(Mat<eT>& matOut, const Mat<eT>& matIn,
               const eT cutoff, const eT belowCutoffValue /* default: 0 */, const eT aboveCutoffValue /* default: 255 */) {
  const u32 height = matIn.n_rows;
  const u32 width = matIn.n_cols;
  matOut.set_size(height, width);
  for (u32 y = 0; y < height; y++)
  for (u32 x = 0; x < width; x++)
  {
    matOut(y, x) = ((matIn(y, x) > cutoff) ? aboveCutoffValue : belowCutoffValue);
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Functions to convert image of other color space to RGB.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
void convert(ImageRGB<eT>& imageOut, const ImageNormalizedRGB<eT>& imageIn) {
  if (!imageIn.check())
    throw logic_error("Inconsistent height and width in imageIn");

  const u32 height = imageIn.height;
  const u32 width  = imageIn.width;

  imageOut.setSize(height, width);

  imageOut.r = imageIn.normalizedR;
  imageOut.g = imageIn.normalizedG;
  imageOut.b = imageIn.normalizedB;
}

template<typename eT>
void convert(ImageRGB<eT>& imageOut, const ImageXYZ<eT>& imageIn) {
  if (!imageIn.check())
    throw logic_error("Inconsistent height and width in imageIn");

  const u32 height = imageIn.height;
  const u32 width  = imageIn.width;
  const u32 n_pixs = height * width;

  imageOut.setSize(height, width);

  for (u32 i = 0; i < n_pixs; i++)
  {
    eT x = imageIn.x(i) / 100.00;
    eT y = imageIn.y(i) / 100.00;
    eT z = imageIn.z(i) / 100.00;

    eT r = ( x * ( 3.2406)) + ( y * (-1.5372)) + ( z * (-0.4986));
    eT g = ( x * (-0.9689)) + ( y * ( 1.8758)) + ( z * ( 0.0415));
    eT b = ( x * ( 0.0557)) + ( y * (-0.2040)) + ( z * ( 1.0570));

    if(r > 0.0031308) { r = ((1.055 * (pow(r, 1.0/2.4))) - 0.055); }
    else              { r = r * 12.92;                             }

    if(g > 0.0031308) { g = ((1.055 * (pow(g, 1.0/2.4))) - 0.055); }
    else              { g = g * 12.92;                             }

    if(b > 0.0031308) { b = ((1.055 * (pow(b, 1.0/2.4))) - 0.055); }
    else              { b = b * 12.92;                             }

    imageOut.r(i) = round(r * 255);  // Convert XYZ to R
    imageOut.g(i) = round(g * 255);  // Convert XYZ to G
    imageOut.b(i) = round(b * 255);  // Convert XYZ to B
  }
}

template<typename eT>
void convert(ImageRGB<eT>& imageOut, const ImageLAB<eT>& imageIn) {
  if (!imageIn.check())
    throw logic_error("Inconsistent height and width in imageIn");

  const u32 height = imageIn.height;
  const u32 width  = imageIn.width;
  const u32 n_pixs = height * width;

  ImageXYZ<eT> imageXyz(height, width);

  for (u32 i = 0; i < n_pixs; i++)
  {
    eT y = ((imageIn.l(i) + 16.0) / 116.0);
    eT x = ((imageIn.a(i) / 500.0) + y);
    eT z = (y - (imageIn.b(i) / 200.0));

    if ( pow(y, 3.0) > 0.008856 ) { y = pow(y, 3.0);                  }
    else                          { y = ( y - 16.0 / 116.0 ) / 7.787; }
    if ( pow(x, 3.0) > 0.008856 ) { x = pow(x, 3.0);                  }
    else                          { x = ( x - 16.0 / 116.0 ) / 7.787; }
    if ( pow(z, 3.0) > 0.008856 ) { z = pow(z, 3.0);                  }
    else                          { z = ( z - 16.0 / 116.0 ) / 7.787; }

    imageXyz.x(i) = x * 95.047;
    imageXyz.y(i) = y * 100.000;
    imageXyz.z(i) = z * 108.883;
  }

  convert(imageOut, imageXyz);  // Convert XYZ to RGB
}

template<typename eT>
void convert(ImageRGB<eT>& imageOut, const ImageHSV<eT>& imageIn) {
  if (!imageIn.check())
    throw logic_error("Inconsistent height and width in imageIn");

  const u32 height = imageIn.height;
  const u32 width  = imageIn.width;
  const u32 n_pixs = height * width;

  imageOut.setSize(height, width);

  for (u32 i = 0; i < n_pixs; i++)
  {
    const eT h = imageIn.h(i);
    const eT s = imageIn.s(i);
    const eT v = imageIn.v(i);

    if ( s == 0 )                       // HSV from 0 to 1
    {
      imageOut.r(i) = v * 255.0;
      imageOut.g(i) = v * 255.0;
      imageOut.b(i) = v * 255.0;
    }
    else
    {
      eT var_h = h * 6.0;
      if ( var_h == 6 ) var_h = 0;      // H must be < 1
      eT var_i = int( var_h );          // Or ... var_i = floor( var_h )
      eT var_1 = v * ( 1.0 - s );
      eT var_2 = v * ( 1.0 - s * ( var_h - var_i ) );
      eT var_3 = v * ( 1.0 - s * ( 1.0 - ( var_h - var_i ) ) );

      if      ( var_i == 0 ) { imageOut.r(i) = v     * 255 ; imageOut.g(i) = var_3 * 255 ; imageOut.b(i) = var_1 * 255 ; }
      else if ( var_i == 1 ) { imageOut.r(i) = var_2 * 255 ; imageOut.g(i) = v     * 255 ; imageOut.b(i) = var_1 * 255 ; }
      else if ( var_i == 2 ) { imageOut.r(i) = var_1 * 255 ; imageOut.g(i) = v     * 255 ; imageOut.b(i) = var_3 * 255 ; }
      else if ( var_i == 3 ) { imageOut.r(i) = var_1 * 255 ; imageOut.g(i) = var_2 * 255 ; imageOut.b(i) = v     * 255 ; }
      else if ( var_i == 4 ) { imageOut.r(i) = var_3 * 255 ; imageOut.g(i) = var_1 * 255 ; imageOut.b(i) = v     * 255 ; }
      else                   { imageOut.r(i) = v     * 255 ; imageOut.g(i) = var_1 * 255 ; imageOut.b(i) = var_2 * 255 ; }
    }
  }
}

template<typename eT>
void convert(ImageRGB<eT>& imageOut, const ImageYCbCr<eT>& imageIn) {
  if (!imageIn.check())
    throw logic_error("Inconsistent height and width in imageIn");

  const u32 height = imageIn.height;
  const u32 width  = imageIn.width;
  const u32 n_pixs = height * width;

  imageOut.setSize(height, width);

  for (u32 i = 0; i < n_pixs; i++)
  {
    const eT& y  = imageIn.y(i) ;
    const eT& cb = imageIn.cb(i);
    const eT& cr = imageIn.cr(i);

    imageOut.r(i) = round( ((1.000 * y) + ( 0.000000 * cb) + (1.402000 * cr)) * 255 );
    imageOut.g(i) = round( ((1.000 * y) + (-0.344136 * cb) - (0.714136 * cr)) * 255 );
    imageOut.b(i) = round( ((1.000 * y) + ( 1.772000 * cb) + (0.000000 * cr)) * 255 );
  }
}

template<typename eT>
void convert(ImageRGB<eT>& imageOut, const Image<eT>& imageIn) {
  switch (imageIn.colorSpace()) {
    case COLORSPACE_RGB:
      imageOut = static_cast<const ImageRGB<eT>&>(imageIn);
      break;
    case COLORSPACE_NORMALIZEDRGB:
      convert(imageOut, static_cast<const ImageNormalizedRGB<eT>&>(imageIn));
      break;
    case COLORSPACE_XYZ:
      convert(imageOut, static_cast<const ImageXYZ<eT>&>(imageIn));
      break;
    case COLORSPACE_LAB:
      convert(imageOut, static_cast<const ImageLAB<eT>&>(imageIn));
      break;
    case COLORSPACE_HSV:
      convert(imageOut, static_cast<const ImageHSV<eT>&>(imageIn));
      break;
    case COLORSPACE_YCBCR:
      convert(imageOut, static_cast<const ImageYCbCr<eT>&>(imageIn));
      break;
    default:
      throw logic_error("Unknown color space");
  }
}

////////////////////////////////////////////////////////////////////////////////
// Functions to convert image of RGB to other color space.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
void convert(ImageNormalizedRGB<eT>& imageOut, const ImageRGB<eT>& imageIn) {
  if (!imageIn.check())
    throw logic_error("Inconsistent height and width in imageIn");

  const u32 height = imageIn.height;
  const u32 width  = imageIn.width;
  const u32 n_pixs = height * width;

  imageOut.setSize(height, width);

  for (u32 i = 0; i < n_pixs; i++)
  {
    eT r            = imageIn.r(i);
    eT g            = imageIn.g(i);
    eT b            = imageIn.b(i);
    eT sum          = r + g + b;
    eT normalizedR  = (r * 255 / sum);
    eT normalizedG  = (g * 255 / sum);
    eT normalizedB  = (b * 255 / sum);

    imageOut.normalizedR(i) = normalizedR;
    imageOut.normalizedG(i) = normalizedG;
    imageOut.normalizedB(i) = normalizedB;
  }
}

template<typename eT>
void convert(ImageXYZ<eT>& imageOut, const ImageRGB<eT>& imageIn) {
  if (!imageIn.check())
    throw logic_error("Inconsistent height and width in imageIn");

  const u32 height = imageIn.height;
  const u32 width  = imageIn.width;
  const u32 n_pixs = height * width;

  imageOut.setSize(height, width);

  for (u32 i = 0; i < n_pixs; i++)
  {
    eT r = imageIn.r(i) / 255.0;
    eT g = imageIn.g(i) / 255.0;
    eT b = imageIn.b(i) / 255.0;

    if(r > 0.04045) { r = powf(((r+0.055)/1.055),2.4); }
    else            { r = r/12.92;                     }

    if(g > 0.04045) { g = powf(((g+0.055)/1.055),2.4); }
    else            { g = g/12.92;                     }

    if(b > 0.04045) { b = powf(((b+0.055)/1.055),2.4); }
    else            { b = b/12.92;                     }

    imageOut.x(i) = (r * 0.4124 * 100.0) + (g * 0.3576 * 100.0) + (b * 0.1805 * 100.0);
    imageOut.y(i) = (r * 0.2126 * 100.0) + (g * 0.7152 * 100.0) + (b * 0.0722 * 100.0);
    imageOut.z(i) = (r * 0.0193 * 100.0) + (g * 0.1192 * 100.0) + (b * 0.9505 * 100.0);
  }
}

template<typename eT>
void convert(ImageLAB<eT>& imageOut, const ImageRGB<eT>& imageIn) {
  if (!imageIn.check())
    throw logic_error("Inconsistent height and width in imageIn");

  const u32 height = imageIn.height;
  const u32 width  = imageIn.width;
  const u32 n_pixs = height * width;

  imageOut.setSize(height, width);

  ImageXYZ<eT> imageXyz;
  convert(imageXyz, imageIn);   // Convert RGB to XYZ

  // Convert XYZ to L*a*b*
  for (u32 i = 0; i < n_pixs; i++)
  {
    eT x = imageXyz.x(i) / 95.047;
    eT y = imageXyz.y(i) / 100;
    eT z = imageXyz.z(i) / 108.883;

    if ( x > 0.008856 ) { x = pow(x, (1.0/3.0));                }
    else                { x = ( 7.787 * x ) + ( 16.0 / 116.0 ); }

    if ( y > 0.008856 ) { y = pow(y, ( 1.0/3.0 ));              }
    else                { y = ( 7.787 * y ) + ( 16.0 / 116.0 ); }

    if ( z > 0.008856 ) { z = pow(z, ( 1.0/3.0 ));              }
    else                { z = ( 7.787 * z ) + ( 16.0 / 116.0 ); }

    imageOut.l(i) = ( 116 * y ) - 16;
    imageOut.a(i) = 500 * ( x - y );
    imageOut.b(i) = 200 * ( y - z );
  }
}

template<typename eT>
void convert(ImageHSV<eT>& imageOut, const ImageRGB<eT>& imageIn) {
  if (!imageIn.check())
    throw logic_error("Inconsistent height and width in imageIn");

  const u32 height = imageIn.height;
  const u32 width  = imageIn.width;
  const u32 n_pixs = height * width;

  imageOut.setSize(height, width);

  for (u32 i = 0; i < n_pixs; i++)
  {
    eT r = imageIn.r(i) / 255.0;
    eT g = imageIn.g(i) / 255.0;
    eT b = imageIn.b(i) / 255.0;

    const eT min = std::min(r, std::min(g, b));
    const eT max = std::max(r, std::max(g, b));
    eT del_Max   = max - min;

    imageOut.v(i) = max;

    if ( del_Max == 0 )                     // This is a gray, no chroma
    {
      imageOut.h(i) = 0;                    // HSV results from 0 to 1
      imageOut.s(i) = 0;
    }
    else                                    // Chromatic data
    {
      imageOut.s(i) = del_Max / max;
      eT del_R = ( ( ( max - r ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;
      eT del_G = ( ( ( max - g ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;
      eT del_B = ( ( ( max - b ) / 6.0 ) + ( del_Max / 2.0 ) ) / del_Max;

      if      ( r == max ) imageOut.h(i) = del_B - del_G;
      else if ( g == max ) imageOut.h(i) = ( 1.0 / 3.0 ) + del_R - del_B;
      else if ( b == max ) imageOut.h(i) = ( 2.0 / 3.0 ) + del_G - del_R;

      if ( imageOut.h(i) < 0 ) imageOut.h(i) += 1;
      if ( imageOut.h(i) > 1 ) imageOut.h(i) -= 1;
    }
  }
}

template<typename eT>
void convert(ImageYCbCr<eT>& imageOut, const ImageRGB<eT>& imageIn) {
  if (!imageIn.check())
    throw logic_error("Inconsistent height and width in imageIn");

  const u32 height = imageIn.height;
  const u32 width  = imageIn.width;
  const u32 n_pixs = height * width;

  imageOut.setSize(height, width);

  for (u32 i = 0; i < n_pixs; i++)
  {
    eT r = imageIn.r(i) / 255;
    eT g = imageIn.g(i) / 255;
    eT b = imageIn.b(i) / 255;

    imageOut.y(i)  = ( ( ( ( 0.299000 * r) + ( 0.587000 * g )  + ( 0.114000 * b) )  ) );
    imageOut.cb(i) = ( ( ( (-0.168736 * r) + (-0.331264 * g )  + ( 0.500000 * b) )  ) );
    imageOut.cr(i) = ( ( ( ( 0.500000 * r) + (-0.418688 * g )  + (-0.081312 * b) )  ) );
  }
}

template<typename eT>
void convert(Image<eT>& imageOut, const ImageRGB<eT>& imageIn) {
  switch (imageOut.colorSpace()) {
    case COLORSPACE_RGB: {
      static_cast<ImageRGB<eT>&>(imageOut) = imageIn;
      break;
    }
    case COLORSPACE_NORMALIZEDRGB: {
      convert(static_cast<ImageNormalizedRGB<eT>&>(imageOut), imageIn);
      break;
    }
    case COLORSPACE_XYZ: {
      convert(static_cast<ImageXYZ<eT>&>(imageOut), imageIn);
      break;
    }
    case COLORSPACE_LAB: {
      convert(static_cast<ImageLAB<eT>&>(imageOut), imageIn);
      break;
    }
    case COLORSPACE_HSV: {
      convert(static_cast<ImageHSV<eT>&>(imageOut), imageIn);
      break;
    }
    case COLORSPACE_YCBCR: {
      convert(static_cast<ImageYCbCr<eT>&>(imageOut), imageIn);
      break;
    }
    default: {
      throw logic_error("Unknown color space");
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// Functions to convert image of any color space to any other color space.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
void convert(Image<eT>& imageOut, const Image<eT>& imageIn) {
  if (imageOut.colorSpace() == COLORSPACE_RGB) {
    convert(static_cast<ImageRGB<eT>&>(imageOut), imageIn);
  }
  else if (imageIn.colorSpace() == COLORSPACE_RGB) {
    convert(imageOut, static_cast<const ImageRGB<eT>&>(imageIn));
  }
  else {
    ImageRGB<eT> imageRgb;
    convert(imageRgb, imageIn);
    convert(imageOut, imageRgb);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Functions to convert grayscale image to color image.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
void convert(ImageRGB<eT>& imageOut, const Mat<eT>& matIn) {
  // For converting grayscale image to color image, we assume that all of the
  // R, G, B channels are set to the same values.
  imageOut.setSize(matIn.n_rows, matIn.n_cols);
  imageOut.r = matIn;
  imageOut.g = matIn;
  imageOut.b = matIn;
}

template<typename eT>
void convert(Image<eT>& imageOut, const Mat<eT>& matIn) {
  if (imageOut.colorSpace() == COLORSPACE_RGB) {
    convert(static_cast<ImageRGB<eT>&>(imageOut), matIn);
  }
  else {
    ImageRGB<eT> imageRgb;
    convert(imageRgb, matIn);
    convert(static_cast<ImageRGB<eT>&>(imageOut), imageRgb);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Functions to convert color image to grayscale image.
////////////////////////////////////////////////////////////////////////////////

template<typename eT>
void convert(Mat<eT>& matOut, const ImageRGB<eT>& imageIn) {
  if (!imageIn.check())
    throw logic_error("Inconsistent height and width in imageIn");

  const u32 height = imageIn.height;
  const u32 width  = imageIn.width;

  matOut.set_size(height, width);
  matOut = 0.2126*imageIn.r + 0.7152*imageIn.g + 0.0722*imageIn.b;
}

template<typename eT>
void convert(Mat<eT>& matOut, const Image<eT>& imageIn) {
  if (imageIn.colorSpace() == COLORSPACE_RGB) {
    convert(matOut, static_cast<const ImageRGB<eT>&>(imageIn));
  }
  else {
    ImageRGB<eT> imageRgb;
    convert(imageRgb, imageIn);
    convert(matOut, static_cast<const ImageRGB<eT>&>(imageIn));
  }
}

}  /* namespace sense */

#endif  /* __IMAGE_IMPL_H__ */
