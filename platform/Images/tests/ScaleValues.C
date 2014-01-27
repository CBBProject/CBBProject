#include <Image.H>
#include <iostream>

int main()
{
    using namespace Images;

    const Dimension nx=4;
    const Dimension ny=3;
    const Dimension nz=2;

    float *data = new float[nx*ny*nz];
    for(int i=0;i<nx*ny*nz;i++)
        data[i]=i;

    const Image3D<float> image(nx,ny,nz,data);

    for(Coord k=0;k<image.dimz();k++)
        for(Coord j=0;j<image.dimy();j++)
            for(Coord i=0;i<image.dimx();i++)
                std::cout << "value(" << i << "," << j << "," << k << ")="
                          << image(i,j,k) << std::endl;

    return 0;
}
