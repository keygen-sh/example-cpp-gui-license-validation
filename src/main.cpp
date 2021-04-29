#include "main_widget.h"
#include <QtWidgets>


int main(int argc, char* argv[]) {
  int ret = 0;
  QApplication app(argc, argv);

  try {
    // Add working directory to possible locations for the "platforms" directory.
    QCoreApplication::addLibraryPath(".");
    MainWidget w;

    w.show();

    // The exec() function call does not return until the UI has been closed by the user.
    ret = app.exec();
  } catch(const std::exception& e) {
    std::fprintf(stderr, "%s\n", e.what());
    return 1;
  }

  return ret;
}
