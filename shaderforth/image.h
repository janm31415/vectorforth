#pragma once

#include <immintrin.h>
#include <emmintrin.h>
#include <stdint.h>
#include <memory>

template <class T>
class image
  {
  public:

    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T value_type;

    image() : _data(nullptr), _access(nullptr), _w(0), _h(0) {}

    image(uint32_t w, uint32_t h, bool init_to_zero = true) : _data(nullptr), _access(nullptr), _w(w), _h(h), _stride(w)
      {
      if ((_w * sizeof(T)) & 31)
        {
        switch (sizeof(T))
          {
          case 1:
          {
          _stride += 32 - (_stride & 31);
          break;
          }
          case 2:
          {
          _stride += 16 - (_stride & 15);
          break;
          }
          case 4:
          {
          _stride += 8 - (_stride & 7);
          break;
          }
          default:
          {
          ++_stride;
          while ((_stride * sizeof(T)) & 31)
            ++_stride;
          break;
          }
          }
        }
      _data = (T*)(_mm_malloc(_stride*_h * sizeof(T), 32));
      _access = (T**)(malloc(_h * sizeof(T*)));
      for (uint32_t i = 0; i < h; ++i)
        _access[i] = _data + (i * _stride);
      if (init_to_zero)
        memset(_data, 0, _stride * _h * sizeof(T));
      }

    image(image&& other) : _data(nullptr), _access(nullptr), _w(0), _h(0), _stride(0)
      {
      _data = other._data;
      _access = other._access;
      _w = other._w;
      _h = other._h;
      _stride = other._stride;
      other._data = nullptr;
      other._access = nullptr;
      other._w = 0;
      other._h = 0;
      other._stride = 0;
      }

    image& operator = (image&& other)
      {
      std::swap(_data, other._data);
      std::swap(_access, other._access);
      std::swap(_w, other._w);
      std::swap(_h, other._h);
      std::swap(_stride, other._stride);
      return *this;
      }

    image(const image& other) : _data(nullptr), _access(nullptr), _w(other._w), _h(other._h), _stride(other._stride)
      {
      _data = (T*)(_mm_malloc(_stride*_h * sizeof(T), 16));
      _access = (T**)(malloc(_h * sizeof(T*)));
      for (uint32_t i = 0; i < _h; ++i)
        _access[i] = _data + (i * _stride);
      memcpy(_data, other._data, _stride * _h * sizeof(T));
      }

    void swap(image& other)
      {
      std::swap(_w, other._w);
      std::swap(_h, other._h);
      std::swap(_stride, other._stride);
      std::swap(_data, other._data);
      std::swap(_access, other._access);
      }

    image& operator = (const image& other)
      {
      image temp(other);
      swap(temp);
      return *this;
      }

    ~image()
      {
      _mm_free(_data);
      free(_access);
      }

    const uint32_t width() const
      {
      return _w;
      }

    const uint32_t height() const
      {
      return _h;
      }

    const uint32_t stride() const
      {
      return _stride;
      }

    T& operator [] (uint32_t ind)
      {
      return _data[ind];
      }

    const T& operator [] (uint32_t ind) const
      {
      return _data[ind];
      }

    T& operator () (uint32_t x, uint32_t y)
      {
      return _access[y][x];
      }

    const T& operator () (uint32_t x, uint32_t y) const
      {
      return _access[y][x];
      }

    iterator begin()
      {
      return _data;
      }

    iterator end()
      {
      return _data + _stride * _h;
      }

    const_iterator begin() const
      {
      return _data;
      }

    const_iterator end() const
      {
      return _data + _stride * _h;
      }

    T* data()
      {
      return _data;
      }

    const T* data() const
      {
      return _data;
      }

    T* row(uint32_t y)
      {
      return _access[y];
      }

    const T* row(uint32_t y) const
      {
      return _access[y];
      }

  private:
    uint32_t _w, _h, _stride;
    T* _data;
    T** _access;
  };


template <class T>
inline image<T> span_to_image(uint32_t w, uint32_t h, uint32_t stride, const T* p_image)
  {
  image<T> out(w, h, false);
  for (uint32_t y = 0; y < h; ++y)
    {
    const T* p_im = p_image + y * stride;
    T* p_out = out.data() + y * out.stride();
    for (size_t x = 0; x < w; ++x)
      *p_out++ = *p_im++;
    }
  return out;
  }

template <class T>
image<T> rotate90(const image<T>& im)
  {
  const auto w = im.width();
  const auto h = im.height();
  image<T> out(h, w, false);
  for (uint32_t x = 0; x < w; ++x)
    {
    for (uint32_t y = 0; y < h; ++y)
      {
      out(y, w - x - 1) = im(x, y);
      }
    }
  return out;
  }

template <class T>
image<T> rotate180(const image<T>& im)
  {
  const auto w = im.width();
  const auto h = im.height();
  image<T> out(w, h, false);
  for (uint32_t x = 0; x < w; ++x)
    {
    for (uint32_t y = 0; y < h; ++y)
      {
      out(w - x - 1, h - y - 1) = im(x, y);
      }
    }
  return out;
  }

template <class T>
image<T> rotate270(const image<T>& im)
  {
  const auto w = im.width();
  const auto h = im.height();
  image<T> out(h, w, false);
  for (uint32_t x = 0; x < w; ++x)
    {
    for (uint32_t y = 0; y < h; ++y)
      {
      out(h - y - 1, x) = im(x, y);
      }
    }
  return out;
  }

template <class T>
inline void put_pixel(T* raw, const int x, const int y, const uint32_t stride, const T& clr)
  {
  raw[x + y * stride] = clr;
  }

template <class T>
inline void put_pixel_checked(T* raw, const int x, const int y, const uint32_t w, const uint32_t h, const uint32_t stride, const T& clr)
  {
  if (x >= 0 && y >= 0 && x < w && y < h)
    put_pixel(raw, x, y, stride, clr);
  }

template <class T>
void draw_line(T* raw, double x1, double y1, double x2, double y2, const uint32_t w, const uint32_t h, const uint32_t stride, const T& color)
  {
  // Bresenham's line algorith
  const bool steep = (std::abs(y2 - y1) > std::abs(x2 - x1));
  if (steep)
    {
    std::swap(x1, y1);
    std::swap(x2, y2);
    }

  if (x1 > x2)
    {
    std::swap(x1, x2);
    std::swap(y1, y2);
    }

  double dx = x2 - x1;
  double dy = std::abs(y2 - y1);

  double error = dx / 2.0;
  const int ystep = (y1 < y2) ? 1 : -1;
  int y = (int)y1;

  const int maxX = (int)x2;

  for (int x = (int)x1; x < maxX; ++x)
    {
    if (steep)
      {
      put_pixel_checked(raw, y, x, w, h, stride, color);
      }
    else
      {
      put_pixel_checked(raw, x, y, w, h, stride, color);
      }
    error -= dy;
    if (error < 0)
      {
      y += ystep;
      error += dx;
      }
    }
  }