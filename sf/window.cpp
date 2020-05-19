#include "window.h"
#include <memory>
#include <thread>
#ifdef _WIN32
#include <windows.h>
#include <windowsx.h>
#include <sstream>
#include <unordered_map>
#else
#include <unistd.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <atomic>
#endif
#include <condition_variable>
#include <chrono>

#ifdef _WIN32

class keyboard_handler
  {
  enum state
    {
    DOWN,
    UP
    };

  public:

    void key_down(int key)
      {
      keystate[key] = DOWN;
      }

    void key_up(int key)
      {
      keystate[key] = UP;
      }

    bool is_down(int key) const
      {
      auto it = keystate.find(key);
      if (it != keystate.end())
        return it->second == DOWN;
      else
        return false;
      }

    bool is_up(int key) const
      {
      auto it = keystate.find(key);
      if (it != keystate.end())
        return it->second == UP;
      else
        return true;
      }

  private:
    std::unordered_map<int, state> keystate;
  };

#endif

struct WindowHandleData
  {
  ~WindowHandleData()
    {
#ifdef _WIN32
    if (w != 0 && h != 0)
      free(bytes);
#else      
    if (w != 0 && h != 0)
      free(bytes);

    if (display)
      {    
      XDestroyImage(im);
      XFreePixmap(display, pix);
      XCloseDisplay(display);
      }
#endif
    }
#ifdef _WIN32
  HWND h_wnd;
  uint8_t* bytes; // the bitmap currently being painted. Is an array of size w*h*channels (can be gray or rgba or rgb).
  keyboard_handler kh;
#else
  Display* display;
  Window win;
  Visual* visual;
  XImage* im;
  char* data; // the pixels of the screen to which we need to mipmap bytes. Is an array of size pix_w*pix_h*4 (rgba).
  uint32_t* bytes; // the bitmap currently being painted, converted to rgba. Is an array of size w*h*4 (rgba).
  Pixmap pix;
  int pix_w, pix_h; // the size of the screen
  GC gc; // Graphics context
  std::atomic<bool> quit;
#endif
  std::unique_ptr<std::thread> t;
  int w, h, channels;
  int x, y;
  std::mutex mt;
  std::condition_variable cv;
  bool initialised;
  IWindowListener* listener;
  };

#ifdef _WIN32

namespace
  {
  static int window_id = 0;

  LRESULT CALLBACK _wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
    switch (msg)
      {
      case WM_MOUSEMOVE:
      {
      WindowHandle wh = (WindowHandle)GetWindowLongPtr(hwnd, GWLP_USERDATA);
      if (wh && wh->listener)
        {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        wh->listener->OnMouseMove(xPos, yPos);
        }
      break;
      }
      case WM_LBUTTONDOWN:
      {
      WindowHandle wh = (WindowHandle)GetWindowLongPtr(hwnd, GWLP_USERDATA);
      if (wh && wh->listener)
        {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        wh->listener->OnMouseLeftButtonDown(xPos, yPos);
        }
      break;
      }
      case WM_LBUTTONUP:
      {
      WindowHandle wh = (WindowHandle)GetWindowLongPtr(hwnd, GWLP_USERDATA);
      if (wh && wh->listener)
        {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        wh->listener->OnMouseLeftButtonUp(xPos, yPos);
        }
      break;
      }
      case WM_KEYDOWN:
      {
      WindowHandle wh = (WindowHandle)GetWindowLongPtr(hwnd, GWLP_USERDATA);
      if (wh && wh->listener && !wh->kh.is_down((int)wParam))
        {
        wh->kh.key_down((int)wParam);
        wh->listener->OnKeyDown((int)wParam);
        }
      break;
      }
      case WM_KEYUP:
      {
      WindowHandle wh = (WindowHandle)GetWindowLongPtr(hwnd, GWLP_USERDATA);
      if (wh && wh->listener)
        {
        wh->kh.key_up((int)wParam);
        wh->listener->OnKeyUp((int)wParam);
        }
      break;
      }
      case WM_ERASEBKGND:
        // Do not erase the background to avoid flickering
        // We'll redraw the full window anyway
        break;
      case WM_CLOSE:
      {
      WindowHandle wh = (WindowHandle)GetWindowLongPtr(hwnd, GWLP_USERDATA);
      if (wh && wh->listener)
        wh->listener->OnClose();
      DestroyWindow(hwnd);
      break;
      }
      case WM_SIZE:
      {
      InvalidateRect(hwnd, NULL, TRUE);
      break;
      }
      case WM_DESTROY:
        PostQuitMessage(0);
        break;
      case WM_PAINT:
      {
      WindowHandle wh = (WindowHandle)GetWindowLongPtr(hwnd, GWLP_USERDATA);
      if (wh && wh->w && wh->h)
        {
        RECT rect;
        GetClientRect(hwnd, &rect);
        PAINTSTRUCT ps;
        HDC hdc;
        HDC hdcMem;
        HGDIOBJ oldBitmap;
        hdc = BeginPaint(hwnd, &ps);
        hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbm = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
        oldBitmap = SelectObject(hdcMem, hbm);

        BITMAPINFO* pbmi = (BITMAPINFO*)alloca(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
        ZeroMemory(pbmi, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
        pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        if (wh->channels == 1)
          {
          pbmi->bmiHeader.biBitCount = 8;
          for (int i = 0; i < 256; ++i)
            {
            pbmi->bmiColors[i].rgbRed = BYTE(i);
            pbmi->bmiColors[i].rgbGreen = BYTE(i);
            pbmi->bmiColors[i].rgbBlue = BYTE(i);
            pbmi->bmiColors[i].rgbReserved = 0;
            }
          }
        else if (wh->channels == 4)
          {
          pbmi->bmiHeader.biBitCount = 32;
          }
        else
          pbmi->bmiHeader.biBitCount = 24;
        pbmi->bmiHeader.biCompression = BI_RGB;
        pbmi->bmiHeader.biPlanes = 1;
        pbmi->bmiHeader.biWidth = wh->w;
        pbmi->bmiHeader.biHeight = -wh->h;
        wh->mt.lock();
        SetStretchBltMode(hdcMem, COLORONCOLOR);
        StretchDIBits(hdcMem,
          0, 0, rect.right - rect.left, rect.bottom - rect.top,
          0, 0, wh->w, wh->h,
          wh->bytes, pbmi, 0, SRCCOPY);
        wh->mt.unlock();
        BitBlt(hdc, 0, 0,
          rect.right - rect.left, rect.bottom - rect.top,
          hdcMem, 0, 0,
          SRCCOPY);
        DeleteObject(hbm);
        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);
        EndPaint(hwnd, &ps);
        }      
      break;
      }
      default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
      }
    return 0;
    }

  HWND _create_window(const std::string& title, int x, int y, int w, int h)
    {
    std::stringstream class_name_ss;
    class_name_ss << "jam_window_" << window_id++;
    std::string class_name = class_name_ss.str();

    HINSTANCE hInstance = GetModuleHandle(0);

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = _wnd_proc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = class_name.c_str();
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
      {
      return NULL;
      }

    // Step 2: Creating the Window

    HWND hwnd;

    hwnd = CreateWindowEx(
      WS_EX_CLIENTEDGE,
      class_name.c_str(),
      title.c_str(),
      WS_OVERLAPPEDWINDOW,
      x, y, w, h,
      NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
      {
      return NULL;
      }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return hwnd;
    }

  void _create_window_with_message_loop(HWND* h_wnd, WindowHandle user_data, const std::string& title, int x, int y, int w, int h)
    {
    user_data->mt.lock();
    *h_wnd = _create_window(title, x, y, w, h);
    SetWindowLongPtr(*h_wnd, GWLP_USERDATA, (LONG_PTR)user_data);
    user_data->initialised = true;
    user_data->cv.notify_all();
    user_data->mt.unlock();

    MSG Msg;
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
      {      
      TranslateMessage(&Msg);
      DispatchMessage(&Msg);      
      }
    }

  std::unique_ptr<std::thread> _create_threaded_window(HWND* h_wnd, WindowHandle user_data, const std::string& title, int x, int y, int w, int h)
    {
    user_data->initialised = false;
    std::unique_ptr<std::thread> res(new std::thread(_create_window_with_message_loop, h_wnd, user_data, title, x, y, w, h));
    std::unique_lock<std::mutex> lk(user_data->mt);
    if (!user_data->initialised)
      user_data->cv.wait(lk, [&] {return user_data->initialised; });
    return res;
    }
  }

void resize(WindowHandle h_wnd, int w, int h)
  {
  RECT rect;
  GetWindowRect(h_wnd->h_wnd, &rect);
  SetWindowPos(h_wnd->h_wnd, NULL, rect.left, rect.top, w, h, NULL);
  InvalidateRect(h_wnd->h_wnd, NULL, TRUE);
  }

#else

namespace
  {
  
  void _scale_line(uint32_t* Target, const uint32_t* Source, int SrcWidth, int TgtWidth)
    {
    // image scaling with Bresenham, Thiadmer Riemersma, Dr.Dobb's.
    int NumPixels = TgtWidth;
    int IntPart = SrcWidth / TgtWidth;
    int FractPart = SrcWidth % TgtWidth;
    int E = 0;
    while (NumPixels-- > 0)
      {
      *Target++ = *Source;
      Source += IntPart;
      E += FractPart;
      if (E >= TgtWidth)
        {
        E -= TgtWidth;
        Source++;
        }
      }
    } 
    
  void _scale(uint32_t* Target, int TgtWidth, int TgtHeight, const uint32_t* Source, int SrcWidth, int SrcHeight)
    {    
    // image scaling with Bresenham, Thiadmer Riemersma, Dr.Dobb's.    
    int NumPixels = TgtHeight;
    int IntPart = (SrcHeight / TgtHeight) * SrcWidth;
    int FractPart = SrcHeight % TgtHeight;
    int E = 0;
    const uint32_t* PrevSource = nullptr;
    while (NumPixels-- > 0)
      {
      
      if (Source == PrevSource)
        {
        memcpy(Target, Target-TgtWidth, TgtWidth*sizeof(uint32_t));
        }
      else
        {
        _scale_line(Target, Source, SrcWidth, TgtWidth);
        PrevSource = Source;
        }
      
      Target += TgtWidth;
      Source += IntPart;
      E += FractPart;
      if (E >= TgtHeight)
        {
        E -= TgtHeight;
        Source += SrcWidth;
        }        
      }
    }
  
  static bool XThreads_initialized = false;

  GC create_gc(Display* display, Window win, int reverse_video)
    {
    GC gc;
    XGCValues values;
    int screen_num = DefaultScreen(display);
    unsigned long valuemask = 0;

    gc = XCreateGC(display, win, valuemask, &values);

    if (reverse_video)
      {
      XSetForeground(display, gc, WhitePixel(display, screen_num));
      XSetBackground(display, gc, BlackPixel(display, screen_num));
      }
    else
      {
      XSetForeground(display, gc, BlackPixel(display, screen_num));
      XSetBackground(display, gc, WhitePixel(display, screen_num));
      }

    unsigned int line_width = 2;
    int line_style = LineSolid;
    int cap_style = CapButt;
    int join_style = JoinBevel;

    XSetLineAttributes(display, gc, line_width, line_style, cap_style, join_style);
    XSetFillStyle(display, gc, FillSolid);

    return gc;
    }

  void _create_window_with_message_loop(WindowHandle user_data, const std::string& title, int x, int y, int w, int h)
    {
    user_data->mt.lock();
    user_data->display = XOpenDisplay(0);
    int screen_num = DefaultScreen(user_data->display);

    user_data->win = XCreateSimpleWindow(user_data->display, RootWindow(user_data->display, screen_num),
      x, y, w, h, 2, BlackPixel(user_data->display, screen_num),
      WhitePixel(user_data->display, screen_num));

    user_data->visual = DefaultVisual(user_data->display, screen_num);
    user_data->gc = create_gc(user_data->display, user_data->win, 0);

    XStoreName(user_data->display, user_data->win, title.c_str());
    XMapWindow(user_data->display, user_data->win);
    XSync(user_data->display, False);
    XSelectInput(user_data->display, user_data->win, ExposureMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask); // Set the events we want to receive
    
    user_data->pix = XCreatePixmap(user_data->display, DefaultRootWindow(user_data->display), w, h, DefaultDepth(user_data->display, screen_num)); // Pixmap for off-screen rendering
    user_data->pix_w = w;
    user_data->pix_h = h;
    
    user_data->data = (char*)malloc(sizeof(uint8_t)*w*h * 4); // Pointer to memory for off-screen rendering
    memset(user_data->data, 0, sizeof(uint8_t)*w*h * 4); // Initialize to black
    user_data->im = XCreateImage(user_data->display, user_data->visual, DefaultDepth(user_data->display, screen_num), ZPixmap, 0, user_data->data, w, h, 32, 0); // Container for user_data->data
    XPutImage(user_data->display, user_data->pix, user_data->gc, user_data->im, 0, 0, 0, 0, user_data->pix_w, user_data->pix_h); // Draw the XImage on the Pixmap
    
    Atom wmDeleteMessage = XInternAtom(user_data->display, "WM_DELETE_WINDOW", False); // When the close button is pressed, the window manager should send this to us as a event (ClientMessage).
    XSetWMProtocols(user_data->display, user_data->win, &wmDeleteMessage, 1);   
    
    user_data->quit = false;
    user_data->initialised = true;
    user_data->cv.notify_all();
    user_data->mt.unlock();

    while (!user_data->quit)
      {
      auto tic = std::chrono::high_resolution_clock::now();
      while (user_data->display && XPending(user_data->display))
        {
        XEvent ev;
        XNextEvent(user_data->display, &ev);
        //printf("event type: %d\n", ev.type); 
        switch (ev.type)
          {
          case Expose:
          {
          user_data->mt.lock();
          XCopyArea(user_data->display, user_data->pix, user_data->win, user_data->gc, 0, 0, user_data->pix_w, user_data->pix_h, 0, 0); // Copy the pixmap to the window for on-screen rendering.
          user_data->mt.unlock();
          break;
          }
          case KeyPress:
          {
          if (user_data->listener)
            user_data->listener->OnKeyDown(ev.xkey.keycode);
          break;
          }
          case KeyRelease:
          {
          unsigned short is_retriggered = 0;
          
          if (XEventsQueued(user_data->display, QueuedAfterReading))
            {
            XEvent nev;
            XPeekEvent(user_data->display, &nev);
            
            if (nev.type == KeyPress && nev.xkey.time == ev.xkey.time && nev.xkey.keycode == ev.xkey.keycode)
              {
              XNextEvent(user_data->display, &ev);
              is_retriggered = 1;
              }
            }
          
          
          if (!is_retriggered && user_data->listener)
            user_data->listener->OnKeyUp(ev.xkey.keycode);
          break;
          }
          case ClientMessage:
          {
          user_data->mt.lock();
          user_data->quit = true;
          if (user_data->listener)
            user_data->listener->OnClose();
          XDestroyImage(user_data->im);
          XFreePixmap(user_data->display, user_data->pix);
          XCloseDisplay(user_data->display);
          user_data->display = nullptr;
          user_data->listener = nullptr;
          user_data->mt.unlock();
          break;
          }
          case ConfigureNotify:
          {
          XConfigureEvent xce = ev.xconfigure;
          if (xce.width != user_data->pix_w || xce.height != user_data->pix_h)
            {
            user_data->mt.lock();
            XFreePixmap(user_data->display, user_data->pix); // Free pixmap  
            XDestroyImage(user_data->im); // Free XImage (and user_data->data)
            user_data->data = (char*)malloc(sizeof(uint8_t)*xce.width*xce.height * 4); // Allocate new memory
            user_data->im = XCreateImage(user_data->display, user_data->visual, DefaultDepth(user_data->display, screen_num), ZPixmap, 0, user_data->data, xce.width, xce.height, 32, 0);   // Create container for user_data->data as XImage.
            user_data->pix = XCreatePixmap(user_data->display, DefaultRootWindow(user_data->display), xce.width, xce.height, DefaultDepth(user_data->display, screen_num)); // Create Pixmap
            user_data->pix_w = xce.width;
            user_data->pix_h = xce.height;
            if (user_data->w && user_data->h)
              _scale((uint32_t*)user_data->data, user_data->pix_w, user_data->pix_h, (uint32_t*)user_data->bytes, user_data->w, user_data->h); // Mipmap user bitmap (user_data->bytes) to screen bitmap
            else
              memset(user_data->data, 0, sizeof(uint8_t)*xce.width*xce.height * 4);
            XPutImage(user_data->display, user_data->pix, user_data->gc, user_data->im, 0, 0, 0, 0, user_data->pix_w, user_data->pix_h); // Copy XImage to Pixmap
            XFlush(user_data->display);
            user_data->mt.unlock();            
            }
          break;
          }
          default: break;
          }
        }
      auto toc = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double, std::micro> time_elapsed = (toc-tic);
      if (time_elapsed < std::chrono::duration<double, std::micro>(1000))
        std::this_thread::sleep_for(std::chrono::duration<double, std::micro>(1000)-time_elapsed);
      }
    }

  std::unique_ptr<std::thread> _create_threaded_window(WindowHandle user_data, const std::string& title, int x, int y, int w, int h)
    {
    user_data->initialised = false;
    std::unique_ptr<std::thread> res(new std::thread(_create_window_with_message_loop, user_data, title, x, y, w, h));
    std::unique_lock<std::mutex> lk(user_data->mt);
    if (!user_data->initialised)
      user_data->cv.wait(lk, [&] {return user_data->initialised; });
    return res;
    }
  }

void resize(WindowHandle h_wnd, int w, int h)
  {
  XResizeWindow(h_wnd->display, h_wnd->win, w, h);
  }

#endif

void close_window(WindowHandle& h_wnd)
  {
  if (h_wnd)
    {
#ifdef _WIN32
    PostMessage(h_wnd->h_wnd, WM_CLOSE, NULL, NULL);
#else
    h_wnd->mt.lock();
    if (h_wnd->listener)
      h_wnd->listener->OnClose();    
    h_wnd->quit = true;
    h_wnd->mt.unlock();
#endif
    h_wnd->t->join();
    delete h_wnd;
    h_wnd = nullptr;
    }
  }

WindowHandle create_window(const std::string& title, int x, int y, int w, int h)
  {
#ifdef _WIN32
  WindowHandle handle = new WindowHandleData();
  handle->h_wnd = nullptr;
  handle->bytes = nullptr;
#else
  if (!XThreads_initialized)
    {
    XInitThreads(); // This should be called exactly once at the beginning of an XLib multithreaded application.
    XThreads_initialized = true;
    }
  WindowHandle handle = new WindowHandleData();
  handle->display = nullptr;
  handle->im = nullptr;
  handle->pix_w = 0;
  handle->pix_h = 0;
  handle->data = nullptr;
  handle->bytes = nullptr;
#endif
  handle->listener = nullptr;
  handle->w = 0;
  handle->h = 0;
  handle->x = x;
  handle->y = y;
  handle->channels = 0;
#ifdef _WIN32
  handle->t = _create_threaded_window(&(handle->h_wnd), handle, title, x, y, w, h);
#else
  handle->t = _create_threaded_window(handle, title, x, y, w, h);
#endif
  return handle;
  }

WindowHandle create_window(const std::string& title, int w, int h)
  {
#ifdef _WIN32
  return create_window(title, CW_USEDEFAULT, CW_USEDEFAULT, w, h);
#else
  return create_window(title, 0, 0, w, h);
#endif
  }

namespace
  {
  void _process_pixels(uint8_t* dst, const uint8_t* src, int width, int height, int stride, int channels, bool bFlipColors, bool bFlip)
    {
    int SrcWidthInbytes = width * channels;
    int DstWidthInBytes = stride * channels;
    int SrcnumBytes = SrcWidthInbytes * height;

    if (!bFlipColors)
      {
      if (bFlip)
        {
        for (int y = 0; y < height; ++y)
          {
          memcpy(dst + (y * DstWidthInBytes), src + ((height - y - 1) * SrcWidthInbytes), SrcWidthInbytes);
          }
        }
      else {
        for (int y = 0; y < height; ++y)
          {
          memcpy(dst + (y * DstWidthInBytes), src + (y * SrcWidthInbytes), SrcWidthInbytes);
          }
        }
      }
    else {
      if (bFlip)
        {
        int x = 0;
        int y = (height - 1) * SrcWidthInbytes;
        src += y;

        for (int i = 0; i < SrcnumBytes; i += channels)
          {
          if (x >= width)
            {
            x = 0;
            src -= SrcWidthInbytes * 2;
            dst += (stride - width);
            }

          for (int j = channels - 1; j >= 0; --j)
            {
            *dst = *(src + j);
            ++dst;
            }

          src += channels;
          ++x;
          }
        }
      else 
        {
        for (int y = 0; y < height; ++y)
          {
          uint8_t* p_dst = dst + y * stride;
          const uint8_t* p_src = src + y * width;
          for (int x = 0; x < width; ++x)
            {
            for (int j = channels - 1; j >= 0; --j)
              {
              *p_dst = *(p_src + j);
              ++p_dst;
              }
            p_src += channels;
            }
          }
        }
      }
    }

  void _process_pixels_32(uint32_t* dst, const uint8_t* src, int width, int height, int channels, bool bFlipColors, bool bFlip)
    {    
    if (channels == 4)
      return _process_pixels((uint8_t*)dst, src, width, height, width, channels, bFlipColors, bFlip);
    else if (channels == 1)
      {
      for (int y = 0; y < height; ++y)
        {
        uint32_t* p_dst = bFlip ? dst + (height - y - 1)*width : dst + y * width;
        const uint8_t* p_src = src + y * width;
        for (int x = 0; x < width; ++x)
          {
          uint32_t clr = 0xff000000 | ((uint32_t)*p_src << 16) | ((uint32_t)*p_src << 8) | ((uint32_t)*p_src);
          *p_dst = clr;
          ++p_dst;
          ++p_src;
          }
        }
      }
    else
      throw std::runtime_error("channels other than 1 or 4 are not supported on Linux");      
    }
    
  
  } // namespace

void paint(WindowHandle h_wnd, const uint8_t* bytes, int w, int h, int channels)
  {
#ifndef _WIN32
  if (h_wnd->quit)
    return;
#endif
  bool flip_topdown = false;
  if (h < 0)
    {
    flip_topdown = true;
    h = -h;
    }
  bool flip_colors = false;
  if (channels < 0)
    {
    flip_colors = true;
    channels = -channels;
    }
#ifdef _WIN32
  h_wnd->mt.lock();
  int stride = (w + 3) & ~3;
  if (h_wnd->w != w || h_wnd->h != h || h_wnd->channels != channels)
    {    
    if (h_wnd->channels == 0)
      {
      h_wnd->bytes = (uint8_t*)malloc(sizeof(uint8_t)*stride*h*channels);
      }
    else
      {
      h_wnd->bytes = (uint8_t*)realloc(h_wnd->bytes, sizeof(uint8_t)*stride*h*channels);
      }
    h_wnd->w = w;
    h_wnd->h = h;
    h_wnd->channels = channels;
    }
  _process_pixels(h_wnd->bytes, bytes, w, h, stride, channels, flip_colors, flip_topdown);
  h_wnd->mt.unlock();
  InvalidateRect(h_wnd->h_wnd, NULL, TRUE);
#else
  h_wnd->mt.lock(); 

  if (h_wnd->w != w || h_wnd->h != h)
    {
    if (h_wnd->w == 0)
      {
      h_wnd->bytes = (uint32_t*)malloc(sizeof(uint32_t)*w*h);
      }
    else
      {
      h_wnd->bytes = (uint32_t*)realloc(h_wnd->bytes, sizeof(uint32_t)*w*h);
      }
    }

  h_wnd->w = w;
  h_wnd->h = h;
  h_wnd->channels = channels;
  
  
  _process_pixels_32((uint32_t*)h_wnd->bytes, bytes, w, h, channels, flip_colors, flip_topdown);   
  
  _scale((uint32_t*)h_wnd->data, h_wnd->pix_w, h_wnd->pix_h, (uint32_t*)h_wnd->bytes, w, h);
  
  XPutImage(h_wnd->display, h_wnd->pix, h_wnd->gc, h_wnd->im, 0, 0, 0, 0, h_wnd->pix_w, h_wnd->pix_h);
  
  XFlush(h_wnd->display);
  
  XEvent ev;   
  ev.type = Expose;
  int res = XSendEvent(h_wnd->display, h_wnd->win, True, ExposureMask, (XEvent*)&ev);
  
  h_wnd->mt.unlock();
#endif
  }

void register_listener(WindowHandle h_wnd, IWindowListener* listener)
  {
  h_wnd->listener = listener;
  }