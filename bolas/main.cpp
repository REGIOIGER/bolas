#include <QApplication>
#include "caja.h"
int main(int argc, char **argv) {

QApplication app (argc, argv);
Caja window;
window.show();
return app.exec();
}
