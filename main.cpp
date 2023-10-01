
// Implementation of Octree in c++
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#define TLF 0
#define TRF 1
#define BRF 2
#define BLF 3
#define TLB 4
#define TRB 5
#define BRB 6
#define BLB 7

// Structure of a point
struct Point {
    int x;
    int y;
    int z;
    Point()
        : x(-1), y(-1), z(-1)
    {
    }

    Point(int a, int b, int c)
        : x(a), y(b), z(c)
    {
    }
};

// Octree class
class Octree {

    Point* point;
    Point* tlf, * brb;
    vector<Octree*> children;

public:
    
    Octree()
    {
        point = new Point();
    }

   
    Octree(int x, int y, int z)
    {
        point = new Point(x, y, z);
    }

   
    Octree(int x1, int y1, int z1, int x2, int y2, int z2)
    {
        
        if (x2 < x1
            || y2 < y1
            || z2 < z1) {
            
            return;
        }

        point = nullptr;
        tlf
            = new Point(x1, y1, z1);
        brb
            = new Point(x2, y2, z2);

       
        children.assign(8, nullptr);
        for (int i = TLF;
            i <= BLB;
            ++i)
            children[i] = new Octree();
    }
    void insert(int x,
        int y,
        int z)
    {

        
        if (find(x, y, z)) {
            cout << "Punto ya en el arbol" << endl;
            return;
        }

        if (x < tlf->x
            || x > brb->x
            || y < tlf->y
            || y > brb->y
            || z < tlf->z
            || z > brb->z) {
            cout << "Punto fuera de limite" << endl;
            return;
        }

        int midx = (tlf->x
            + brb->x)
            / 2;
        int midy = (tlf->y
            + brb->y)
            / 2;
        int midz = (tlf->z
            + brb->z)
            / 2;

        int pos = -1;

        if (x <= midx) {
            if (y <= midy) {
                if (z <= midz)
                    pos = TLF;
                else
                    pos = TLB;
            }
            else {
                if (z <= midz)
                    pos = BLF;
                else
                    pos = BLB;
            }
        }
        else {
            if (y <= midy) {
                if (z <= midz)
                    pos = TRF;
                else
                    pos = TRB;
            }
            else {
                if (z <= midz)
                    pos = BRF;
                else
                    pos = BRB;
            }
        }

        if (children[pos]->point == nullptr) {
            children[pos]->insert(x, y, z);
            return;
        }

     
        else if (children[pos]->point->x == -1) {
            delete children[pos];
            children[pos] = new Octree(x, y, z);
            return;
        }
        else {
            int x_ = children[pos]->point->x,
                y_ = children[pos]->point->y,
                z_ = children[pos]->point->z;
            delete children[pos];
            children[pos] = nullptr;
            if (pos == TLF) {
                children[pos] = new Octree(tlf->x,
                    tlf->y,
                    tlf->z,
                    midx,
                    midy,
                    midz);
            }

            else if (pos == TRF) {
                children[pos] = new Octree(midx + 1,
                    tlf->y,
                    tlf->z,
                    brb->x,
                    midy,
                    midz);
            }
            else if (pos == BRF) {
                children[pos] = new Octree(midx + 1,
                    midy + 1,
                    tlf->z,
                    brb->x,
                    brb->y,
                    midz);
            }
            else if (pos == BLF) {
                children[pos] = new Octree(tlf->x,
                    midy + 1,
                    tlf->z,
                    midx,
                    brb->y,
                    midz);
            }
            else if (pos == TLB) {
                children[pos] = new Octree(tlf->x,
                    tlf->y,
                    midz + 1,
                    midx,
                    midy,
                    brb->z);
            }
            else if (pos == TRB) {
                children[pos] = new Octree(midx + 1,
                    tlf->y,
                    midz + 1,
                    brb->x,
                    midy,
                    brb->z);
            }
            else if (pos == BRB) {
                children[pos] = new Octree(midx + 1,
                    midy + 1,
                    midz + 1,
                    brb->x,
                    brb->y,
                    brb->z);
            }
            else if (pos == BLB) {
                children[pos] = new Octree(tlf->x,
                    midy + 1,
                    midz + 1,
                    midx,
                    brb->y,
                    brb->z);
            }
            children[pos]->insert(x_, y_, z_);
            children[pos]->insert(x, y, z);
        }
    }

    bool find(int x, int y, int z)
    {
        
        if (x < tlf->x
            || x > brb->x
            || y < tlf->y
            || y > brb->y
            || z < tlf->z
            || z > brb->z)
            return 0;
        
        int midx = (tlf->x
            + brb->x)
            / 2;
        int midy = (tlf->y
            + brb->y)
            / 2;
        int midz = (tlf->z
            + brb->z)
            / 2;

        int pos = -1;

        if (x <= midx) {
            if (y <= midy) {
                if (z <= midz)
                    pos = TLF;
                else
                    pos = TLB;
            }
            else {
                if (z <= midz)
                    pos = BLF;
                else
                    pos = BLB;
            }
        }
        else {
            if (y <= midy) {
                if (z <= midz)
                    pos = TRF;
                else
                    pos = TRB;
            }
            else {
                if (z <= midz)
                    pos = BRF;
                else
                    pos = BRB;
            }
        }

        if (children[pos]->point == nullptr) {
            return children[pos]->find(x, y, z);
        }

        else if (children[pos]->point->x == -1) {
            return 0;
        }
        else {

           
            if (x == children[pos]->point->x
                && y == children[pos]->point->y
                && z == children[pos]->point->z)
                return 1;
        }
        return 0;
    }
    Point* Getpoint() {
        return point;
    }
    vector<Octree*> Getchildren() {
        return children;
    }
};


int main()
{
    Octree tree(0, 0, 0,1500, 1500, 1500);//desplaze puntos en 500 

    
    ifstream file("points2.txt");// poner el nombre del txt de los puntos ejem "nombre.txt"
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }
    string line;
    Octree octree;
    while (getline(file, line)) {
        istringstream iss(line);
        string token;
        vector<int> values;

        while (getline(iss, token, ',')) {
            int value;
            if (istringstream(token) >> value) {
                values.push_back(value);
            }
        }

        if (values.size() == 3) {
            Point point(values[0], values[1], values[2]);
            cout << values[0] << " , " << values[1] << " , " << values[2] << endl;
            tree.insert(values[0]+500,values[1]+500,values[2]+500);//desplazamiento en 500 a rango positivo
            
        }
    }

    file.close();
    Octree* p = &tree;
    for (; !( * p).Getpoint(); p = p->Getchildren()[1]) {

    }
    cout <<p->Getpoint() << endl;// una rama
    
   


    return 0;
}
