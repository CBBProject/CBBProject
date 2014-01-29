#include <iostream>
#include <ImageViewer.H>
#include <QApplication>

int 
main(int argc,char* argv[]) try {

    QApplication application(argc,argv);

    ImageViewer imageViewer;
    imageViewer.show();
    return application.exec();

} catch (...) {
    std::cerr << "Something went wrong !!" << std::endl;
}
