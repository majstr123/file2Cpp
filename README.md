# file2Cpp
This tool is ment for use when developing ESP8266/ESP32 web applications, where website lives within device program memory.<br>
Input to program is any file, output is gziped version of byte array inside C/C++ header type file.


# !!! Important  !!!
Program requires instalation of 7-zip inside "C:\Program Files\7-Zip"<br>

Feel free to change to your needs.<br>

For the lazy ones there is aslo the .exe version :)<br>
Btw, this is my first git commit, so sory for any mistake! 

example output for C:\\your path\index.html and C:\\your path\css\style.css input file.<br>
![](screenshots/screenshot.png)<br>
raw file size = 10.727 bytes, compressed to gzip = 1.804 bytes -->> ~83% reduction<br>
raw file size = 13.975 bytes, compressed to gzip = 2.906 bytes -->> ~79% reduction<br>

index.h:
```c++
#ifndef INDEX_HTML_H
#define INDEX_HTML_H
const char index_html[] PROGMEM = {
  0x1F, 0x8B, 0x08, 0x08, 0xF0, 0x83, 0xC6, 0x5C, 0x04, 0x00, 0x69, 0x6E, 
  0x64, 0x65, 0x78, 0x2E, 0x68, 0x74, 0x6D, 0x6C, 0x00, 0xED, 0x5A, 0xDB, 
  0x8E, 0xD3, 0x30, 0x10, 0x7D, 0x47, 0xE2, 0x1F, 0x4C, 0x24, 0x48, 0x57, 
  0xD0, 0x14, 0x10, 0x20, 0x44, 0x2F, 0x08, 0x96, 0xE5, 0x7E, 0x13, 0x5B, 
  ... more bytes here ...
  0x2B, 0xC4, 0x3F, 0xF1, 0x0D, 0x7C, 0x19, 0x33, 0x4E, 0xD2, 0xA6, 0x4D, 
  0x7E, 0xAA, 0xC4, 0x60, 0x6F, 0xE1, 0x77, 0xBD, 0xDB, 0xDF, 0x16, 0x3F, 
  0x2A, 0x02, 0xDA, 0xF2, 0xF4, 0xB7, 0x49, 0xF9, 0xB3, 0x5E, 0x67, 0xF6, 
  0x31, 0x14, 0x56, 0xF4, 0x77, 0x9D, 0x3F, 0x01, 0xCA, 0xCC, 0xA9, 0xE5, 
  0xE7, 0x29, 0x00, 0x00};
size_t  index_html_size = 1804;
#endif
```

```c++
#ifndef STYLE_CSS_H
#define STYLE_CSS_H
const char style_css[] PROGMEM = {
  0x1F, 0x8B, 0x08, 0x08, 0xE4, 0x4A, 0xC9, 0x5C, 0x04, 0x00, 0x73, 0x74, 
  0x79, 0x6C, 0x65, 0x2E, 0x63, 0x73, 0x73, 0x00, 0xE5, 0x1B, 0xC9, 0xAE, 
  0xD3, 0x30, 0xF0, 0xCE, 0x57, 0x44, 0x20, 0xC4, 0x9A, 0xD2, 0xA6, 0x0D, 
  0x4B, 0x11, 0x12, 0x42, 0x02, 0x71, 0x00, 0x0E, 0x2C, 0x27, 0x84, 0x90, 
  ... more bytes here ...
  0xD0, 0xF8, 0xA3, 0x9D, 0x44, 0xCC, 0x28, 0xB0, 0xAF, 0x3E, 0x83, 0xE3, 
  0xC6, 0xD5, 0x0B, 0xC0, 0xEA, 0x81, 0xE6, 0xF8, 0x29, 0xDC, 0xF6, 0x01, 
  0x51, 0xBC, 0x7A, 0xFE, 0xE4, 0x9E, 0xF8, 0xAB, 0x95, 0x59, 0xDE, 0xB6, 
  0xB3, 0x1D, 0xE2, 0x5B, 0xB8, 0xEF, 0x65, 0xEA, 0x03, 0xDB, 0x97, 0x36, 
  0x00, 0x00};
size_t  style_css_size = 2906;
#endif
```


web.h:
```c++
#ifndef WEB_H
#define WEB_H

#include "index.h"
#include "style.h"
#endif
```

Example usage for esp8266 web server for index.h:
```c++
// in your programm only include web.h which automaticly already contains all other headers 
// !!! web.h is updated on every update !!!
#include "web.h"

void handleIndex() {
    server.sendHeader("content-encoding", "gzip"); // send file in gzip format
    server.send_P(200, "text/html", index_html, index_html_size);
}
void handleCss() {
    server.sendHeader("content-encoding", "gzip"); // send file in gzip format
    server.send_P(200, "text/css", style_css, style_css_size); // alternative to style_css_size is sizeof(style_css)
}
void setup() {
//...
  server.on("/index.html", handleIndex);
  server.on("/css/style.css", handleCss);
  server.begin();
//...

}
```
