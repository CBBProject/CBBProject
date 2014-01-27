#include <cbbAbstractImage.h>

#include <Image.H>
#include <Images/Maps.H>

#include <cbbView.h>
#include <cbbView_p.h>

cbbView::cbbView(): dtkAbstractView() {
    DTK_D(cbbView);

    d->scene = new QGraphicsScene;
    
    d->view = new QGraphicsView;
    d->view->setScene(d->scene);
    d->view->setRenderHint(QPainter::Antialiasing,true);
}

void cbbView::addImage(cbbAbstractImage* I) {

  qDebug() << Q_FUNC_INFO;

    DTK_D(cbbView);

    if(!I)
        return;

    const Images::Image* image = I->image();

    if (!image || image->dimension()!=2)
        return;

    using namespace Images;
    using namespace Images::Pixels;

    typedef RGB<unsigned char> RGBPixel;
    typedef Polymorphic::MultiDim<2> Images2D;
    typedef Images2D::MultiType<float,Images2D::MultiType<double,Images2D::MultiType<unsigned char,Images2D::MultiType<unsigned> > > > Scalar2D;
    typedef Images2D::MultiType<RGBPixel,Scalar2D> Accepted;

    typedef RGB<double>        RGBDouble;
    Accepted::Converter<RGBDouble> convert;
    const Image2D<RGBDouble>& im = convert(image);

    const IntensityMap<RGBDouble> map(im);
    QImage qim(im.dimx(),im.dimy(),QImage::Format_RGB888);
    const Image2D<RGBDouble>::Shape& shape = im.shape();
    for (typename Image2D<RGBDouble>::Shape::const_iterator i=shape.begin();i!=shape.end();++i) {
        RGBPixel pix = map(im(i));
        qim.setPixel(i(1),i(2),(pix.red() << 8 | pix.green()) << 8 | pix.blue());
    }

    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(qim));
    d->scene->addItem(item);
    d->view->show();
}

QWidget *cbbView::widget() {
    DTK_D(cbbView);
    return d->view;
}
