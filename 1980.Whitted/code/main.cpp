#include <iostream>

#include "WhittedRayTracer.h"
#include "framework/MyApp.h"
#include "framework/PinholeCamera.h"

const char* const APP_NAME = "Recursive Ray Tracing - Turner Whitted 1980";

int main(void) {
  MyApp app;
  try {
    app.init();
    app.createWindow(1280, 720, APP_NAME);
    app.createRenderer<WhittedRayTracer>();

    app.mainLoop();

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }

  app.shutdown();
  return 0;
}