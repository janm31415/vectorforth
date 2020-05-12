#pragma once

#include <iostream>
#include <string>
#include <stdint.h>

/*
Simple window class that works in windows and linux. 
 - Generates a win32 or x11 window in its own thread.
 - Can paint a bitmap.
 - Can listen to the events OnClose, OnKeyDown and OnKeyUp.
*/

class IWindowListener
  {
  public:
    virtual void OnClose() {};
    virtual void OnKeyDown(int) {};
    virtual void OnKeyUp(int) {};
  };

struct WindowHandleData;
typedef WindowHandleData* WindowHandle;

WindowHandle create_window(const std::string& title, int w, int h);
WindowHandle create_window(const std::string& title, int x, int y, int w, int h);

void close_window(WindowHandle& h_wnd);

// if h is negative the image will be flipped upside down
// if channels is negative the order of the colors will be flipped (e.g. convert rgb to bgr)
void paint(WindowHandle h_wnd, const uint8_t* bytes, int w, int h, int channels);

void resize(WindowHandle h_wnd, int w, int h);

void register_listener(WindowHandle h_wnd, IWindowListener* listener);
