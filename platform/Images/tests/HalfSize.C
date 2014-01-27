#include <iostream>
#include <Image.H>
#include <exception>
#include <typeinfo>

//  Example: ./HalfSize < images/irm.float.inr ###

using namespace std;

int
main(int argc,char *argv[]) try
{
    using namespace Images;

    Image *image;
    cin >> image;

    if (image->dimension()!=3) {
        cerr << "Only implemented for 3D images, sorry." << endl;
        return 1;
    }

    Image3D<float> input(image->size(0),image->size(1),image->size(2));

    if (image->pixel_id()==typeid(unsigned char)) {
        const Image3D<unsigned char> &tmp = *static_cast<Image3D<unsigned char>*>(image);
        input = static_cast<Image3D<float> >(tmp);
    } else if (image->pixel_id()==typeid(float)) {
        input = *static_cast<Image3D<float>*>(image);
    } else if (image->pixel_id()==typeid(unsigned int)) {
        const Image3D<unsigned int> &tmp = *static_cast<Image3D<unsigned int>*>(image);
        input = static_cast<Image3D<float> >(tmp);
    } else if (image->pixel_id()==typeid(int)) {
        const Image3D<int> &tmp = *static_cast<Image3D<int>*>(image);
        input = static_cast<Image3D<float> >(tmp);
    } else if (image->pixel_id()==typeid(unsigned short)) {
        const Image3D<unsigned short> &tmp = *static_cast<Image3D<unsigned short>*>(image);
        input = static_cast<Image3D<float> >(tmp);
    } else if (image->pixel_id()==typeid(short)) {
        const Image3D<short> &tmp = *static_cast<Image3D<short>*>(image);
        input = static_cast<Image3D<float> >(tmp);
    } else {
        cerr << "Only implemented for unsigned char, [unsigned] int, [unsigned] short and float 3D images, sorry." << endl;
        return 1;
    }

    Image3D<float> output(input.dimx()/2, input.dimy()/2, input.dimz()/2);

    for(Coord i=0;i<output.dimx();++i)
        for(Coord j=0;j<output.dimy();++j)
            for(Coord k=0;k<output.dimz();++k)
                output(i,j,k) = (input(2*i  ,2*j  ,2*k  )+
                                 input(2*i  ,2*j  ,2*k+1)+
                                 input(2*i  ,2*j+1,2*k  )+
                                 input(2*i  ,2*j+1,2*k+1)+
                                 input(2*i+1,2*j  ,2*k  )+
                                 input(2*i+1,2*j  ,2*k+1)+
                                 input(2*i+1,2*j+1,2*k  )+
                                 input(2*i+1,2*j+1,2*k+1))/8.0;

    if (argc>1 && argv[1][0]=='-' && argv[1][1]=='f')
        cout << output;
    else if (image->pixel_id()==typeid(unsigned char))
        cout << static_cast<Image3D<unsigned char> >(output);
    else if (image->pixel_id()==typeid(float))
        cout << output;
    else if (image->pixel_id()==typeid(unsigned int))
        cout << static_cast<Image3D<unsigned int> >(output);
    else if (image->pixel_id()==typeid(int))
        cout << static_cast<Image3D<int> >(output);
    else if (image->pixel_id()==typeid(unsigned short))
        cout << static_cast<Image3D<unsigned short> >(output);
    else if (image->pixel_id()==typeid(short))
        cout << static_cast<Image3D<short> >(output);

    return 0;
}
catch (const Images::Exception &e) {
    cerr << e.what() << endl;
    return e.code();
}
