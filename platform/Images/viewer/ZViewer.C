#include <iostream>
#include <Images/ImageViewer.H>
#include <QApplication>

int 
main(int argc,char* argv[]) try {

    QApplication application(argc,argv);

    ImageViewer imageViewer;

    if (argc==2)
        imageViewer.open(argv[1]);

    imageViewer.show();
    return application.exec();

} catch (...) {
    std::cerr << "Something went wrong !!" << std::endl;
}
