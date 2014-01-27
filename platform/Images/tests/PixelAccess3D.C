#include <iostream>
#include <Image.H>

int main() {

	using namespace Images;
	using namespace std;

	Dimension nx = 4;
	Dimension ny = 3;
	Dimension nz = 2;

	float *data = new float[nx*ny*nz];
	for(int i=0;i<nx*ny*nz;++i)
        data[i]=i;
    
	Image3D<float> image(nx,ny,nz,data);

	for(int k=0;k<image.dimz();k++)
        for(int j=0;j<image.dimy();j++)
            for(int i=0;i<image.dimx();i++) {
                const float value = image(i,j,k);
                cout << "value(" << i << "," << j << "," << k << ")=" << value << endl;
            }

	return 0;
}
