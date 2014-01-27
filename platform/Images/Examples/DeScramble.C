#include <stdlib.h>
#include <fstream>
#include <set>
#include <vector>
#include <Image.H>
#include <Maths/Arith.H>

//  Example: ./Scramble images/irm.inr irm.vmr 1000 ## results/Null.output results/irm.vmr

using namespace std;
using namespace Images;

template <typename Pixel>
double
norm(const Images::Image1D<Pixel>& image) {
    double norm = 0.0;
    for (typename Images::Image1D<Pixel>::template const_iterator<pixel> i=image.begin();i!=image.end();++i)
        norm += Maths::sqr(*i);
    return norm;
}

struct Edge {

    Edge() { }
    Edge(const unsigned src,const unsigned dest) {
        vertex[0] = src;
        vertex[1] = dest;
    }

    unsigned& src()        { return vertex[0]; }
    unsigned  src()  const { return vertex[0]; }
    unsigned& dest()       { return vertex[1]; }
    unsigned  dest() const { return vertex[1]; }

private:    

    unsigned vertex[2];
};

struct Score: public Edge {

    Score() { }
    Score(const unsigned src,const unsigned dest,const double score): Edge(src,dest),cost(score) { }

    unsigned index() const { return dest(); }
    double   score() const { return cost;   }

    bool operator<(const Score& s) const { return cost<s.cost; }

private:
    
    double cost;
};

inline std::ostream& operator<<(std::ostream& os,const Score& sc) {
    return os << '[' << sc.index() << ',' << sc.score() << ']';
}

namespace std {
    template<>
    struct less<const Score*> {
        bool operator()(const Score* score1,const Score* score2) const {
            if (score1->score()==score2->score()) {
                const Edge& edge1 = *score1;
                const Edge& edge2 = *score2;
                if (edge1.src()==edge2.dest())
                    return edge1.dest()<edge2.src();
                return edge1.src()<edge2.dest();
            }
            return score1->score()<score2->score();
        }
    };
}

class StraightPath {
public:

    typedef unsigned Vertex;
    typedef std::list<Vertex> Path;

private:

    typedef enum { None=0, Left=1, Right=2, Both=3 } VertexStatus;
    typedef std::list<Path>   Paths;

    struct VertexInfo {

        VertexInfo(): stat(None) { }
        VertexInfo(Paths::iterator path,const VertexStatus status): id(path),stat(status) { }

        Paths::iterator&      path()       { return id;  }
        const Paths::iterator path() const { return id;  }

        VertexStatus& status()       { return stat; }
        VertexStatus  status() const { return stat; }

        unsigned& index()       { return idx; }
        unsigned  index() const { return idx; }

    private:

        Paths::iterator id;
        VertexStatus    stat;
        unsigned        idx;
    };

public:

    StraightPath(const Score* sc,const unsigned sz): vinfo(sz),paths(),scores(sc) {
        for (unsigned i=0,index=0;i<sz;++i,index+=sz) {
            vinfo[i].index() = index;
            while (!candidates.insert(&scores[++vinfo[i].index()]).second);
        }
        construct();
    }

    const Path& operator()() const { return *(paths.begin()); }

private:

    void construct() {
        for (unsigned i=1;i<vinfo.size();) {
            std::set<const Score*>::iterator iter = candidates.begin();
            Edge e = **iter;
            VertexInfo& vi = vinfo[e.src()];
            candidates.erase(iter);
            if (connect(e))
                ++i;

            if (vi.status()!=Both)
                while (!candidates.insert(&scores[++vi.index()]).second);
        }
    }

    //  Test whether a vertex is already fully connected (ie it has two neighbors).

    bool fully_connected(const Vertex v) const { return vinfo[v].status()==Both; }

    //  Test whether adding the edge e would create a cycle or not.

    bool would_create_cycle(const Edge& e) const { return !unconnected(e.src()) && vinfo[e.src()].path()==vinfo[e.dest()].path(); }

    //  Test whether both path ends are on the same end of the paths.

    bool same_border_on_paths(const Edge& e) const { vinfo[e.src()].status()==vinfo[e.dest()].status(); }

    //  Is the vertex already connected to a pathlet ?

    bool unconnected(const Vertex v) const { return vinfo[v].status()==None; }

    //  Add a new connexion. v1 is supposed to be un-connected.

    void connect(const Vertex v1,const Vertex v2) {

        //  If both vertices are unconnected, simply create a new pathlet.

        if (unconnected(v2)) {
            Path connexion;
            connexion.push_back(v1);
            connexion.push_back(v2);
            paths.push_back(connexion);
            Paths::iterator id = --paths.end();
            vinfo[v1].path() = vinfo[v2].path() = id;
            vinfo[v1].status() = Left;
            vinfo[v2].status() = Right;
            return;
        }

        vinfo[v1].status() = vinfo[v2].status();
        vinfo[v1].path()   = vinfo[v2].path();
        if (vinfo[v2].status()==Left)
            vinfo[v2].path()->push_front(v1);
        else
            vinfo[v2].path()->push_back(v1);

        vinfo[v2].status() = Both;
    }

    bool connect(Edge& e) {

        // Do not create the edge if it would create a cycle.

        if (fully_connected(e.src()) || fully_connected(e.dest()) || would_create_cycle(e))
            return false;

        //  If one of the vertices is unconnected, simply connect it properly.

        if (unconnected(e.dest()))
            std::swap(e.src(),e.dest());

        if (unconnected(e.src())) {
            connect(e.src(),e.dest());
            return true;
        }

        //  Both edges are on a border of different paths.
                
        if (vinfo[e.src()].status()!=Right) //   If there is a right border, put it first.
            std::swap(e.src(),e.dest());

        //  We need to reverse one path (first or second depending on the border type).

        if (vinfo[e.src()].status()==vinfo[e.dest()].status()) {
            const bool IsLeft = vinfo[e.src()].status()==Left;
            const Vertex v1 = IsLeft ? e.src() : e.dest();
            const Vertex v2 = *(IsLeft ? --vinfo[v1].path()->end() : vinfo[v1].path()->begin());
            vinfo[v1].path()->reverse();
            std::swap(vinfo[v1].status(),vinfo[v2].status());
        }

        vinfo[e.src()].path()->splice(vinfo[e.src()].path()->end(),*(vinfo[e.dest()].path()));
        paths.erase(vinfo[e.dest()].path());

        vinfo[e.dest()].path() = vinfo[e.src()].path();
        vinfo[e.src()].status() = vinfo[e.dest()].status() = Both;

        for (Path::const_iterator i=vinfo[e.src()].path()->begin();i!=vinfo[e.src()].path()->end();++i)
            vinfo[*i].path() = vinfo[e.src()].path();

        const Vertex v1 = *(vinfo[e.src()].path()->begin());
        const Vertex v2 = *(--vinfo[e.src()].path()->end());

        return true;
    }

    std::vector<VertexInfo> vinfo;
    Paths                   paths;
    const Score*            scores;
    std::set<const Score*>  candidates;
};

//  The user writes any function template depending upon the Pixel type
//  and the Dimension of the image.

template <typename Pixel,unsigned>
Image*
DeScramble(const Image* im,std::vector<unsigned>& perm) {

    typedef typename ImageType<2,Pixel>::type  IMAGE;
    typedef typename ImageType<1,Pixel>::type  Line;
    typedef typename ImageType<1,double>::type FLine;
    
    const IMAGE& image = *reinterpret_cast<const IMAGE*>(im);

    const unsigned dimy = image.dimy();

    Score scores[dimy][dimy];
    for (unsigned i=0;i<dimy;++i) {
        const FLine line1 = static_cast<FLine>(image(Range::all(),i));
        scores[i][i] = Score(i,i,0.0);
        for (unsigned j=i+1;j<dimy;++j) {
            const FLine line2 = static_cast<FLine>(image(Range::all(),j));
            const FLine diff  = line1-line2;
            const double score = norm(diff);
            scores[i][j] = Score(i,j,score);
            scores[j][i] = Score(j,i,score);
        }
    }

    for (unsigned i=0;i<dimy;++i)
        std::sort(&scores[i][0],&scores[i][dimy]);

    StraightPath path(&scores[0][0],dimy);
    const StraightPath::Path& permutation = path();

    IMAGE* res = new IMAGE(image.shape());
    unsigned ind = 0;
    for (StraightPath::Path::const_iterator i=permutation.begin();i!=permutation.end();++i,++ind)
        (*res)(Range::all(),ind) = image(Range::all(),*i);
    
    return res;
}

//  Declare  a handle, MainHandle, to that function template.

DeclarePolymorphicHandle(DeScramble,Image* (*prototype)(const Image*,std::vector<unsigned>&));

int
main(const int argc,const char* argv[]) try {

    using namespace Images;

    Image* image;

    std::ifstream ifs(argv[1]);
    std::ofstream ofs(argv[2]);
    std::ifstream ifp(argv[3]);

    ifs >> image;

    typedef Polymorphic::Types<float,double,unsigned char>::Dimensions<2> Switch;
    Switch item(image->pixel_id(),image->dimension());

    std::vector<unsigned> perm;
    typedef std::istream_iterator<unsigned> iit;
    back_insert_iterator<vector<unsigned> > inserter(perm);

    std::copy(iit(ifp),iit(),inserter);

    Image* res = (*item.Function<DeScrambleHandle>())(image,perm);

    for (std::vector<unsigned>::const_iterator i=perm.begin();i!=perm.end();++i)
        std::cout << *i << ' ';
    std::cout << std::endl;
    
    ofs << res;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
