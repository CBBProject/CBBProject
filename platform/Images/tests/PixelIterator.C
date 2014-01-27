#include <iostream>
#include <Image.H>

// Test the pixel access via pixel iterator by inverting an unsigned char image.

//  Example: ./PixelIterator < images/bear.pgm

int
main()
{
    using namespace Images;
    using namespace std;

    Image2D<unsigned char> image;

    cin >> image;

    if (!cin) {
        cerr << "The image given as input is not an unsigned char image." << endl;
        return 2;
    }
    
    for (Image2D<unsigned char>::iterator<pixel> i=image.begin();i!=image.end();++i)
        *i = 255-*i;

    cout << image;

    return 0;
}
