#include <iostream>
#include <Images/MultiDimCounter.H>

int
main()
{
    const int hb[3]  = { 3, 5, 7 };

    Images::MultiDimCounter<3,Images::Bounds<3> > mdc(hb);

    std::cout << mdc << std::endl;

    mdc += 10;

    std::cout << mdc << std::endl;
    
    mdc -= 10;

    for (unsigned i=0;i<10;++i,++mdc)
        std::cout << mdc << std::endl;
    std::cout << mdc << std::endl;
}
