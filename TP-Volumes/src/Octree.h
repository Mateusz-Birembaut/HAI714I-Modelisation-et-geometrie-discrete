#ifndef OCTREE_H
#define OCTREE_H

#include "Vec3.h"
#include <vector>
#include <map>

struct Octant {
    std::vector<Vec3> points; // points dans cet octant
    std::vector<Vec3> normals; // normales dans cet octant

    Vec3 min; // bas gauche
    Vec3 max; // coordonée haut droite
    Octant *children[8]; // adresse des fils
    bool isLeaf = true; // est-ce une feuille
    unsigned long id; // id unique de l'octant

    Vec3 pos = Vec3(0,0,0); // position moyenne des points dans cet octant
    Vec3 normal = Vec3(0,0,0); // normale moyenne des points dans cet octant

    bool contains(const Vec3& point) const {
    return (point[0] >= min[0] && point[0] <= max[0] &&
            point[1] >= min[1] && point[1] <= max[1] &&
            point[2] >= min[2] && point[2] <= max[2]);
    }

    Octant(const Vec3& min, const Vec3& max){
        this->points = std::vector<Vec3>();
        this->min = min;
        this->max = max;
        this->isLeaf = true;

        for(int i = 0; i < 8; i++) {
            children[i] = nullptr;
        }
    } 
};


class Octree {
public:

    Octant* base_octant; // octants (sous cubes)

    int nbVerticicesPerOctants; // resolution de l'octree

    Octree(const int nbVerticicesPerOctants,const Vec3& min, const Vec3& max);
    Octree();

    void addPoint(Octant& octant, const Vec3& point, const Vec3& normal); // ajoute un point et sa normale à l'octree
    void subdivide(Octant& octant); // divisen un octant en 8 sous octants quand on dépasse le nombre de points par octant

    void printOctree(const Octant& octant, int depth) const;
    void print() const;

    void drawOctant(const Octant& octant) const; // dessine l'octree
    void draw() const; // dessine un octant

    void SummPosAndNormalRecursive(Octant& octant);
    void SummPosAndNormal();

    int getOctantIndex(const Vec3 point); // retourne l'id unique de l'octant contenant le point
    int getOctantIndexRecursive(Octant &octant, const Vec3 point);

    void traverseLeaves(Octant& octant, std::map<int, int>& idToVertexIndex, int& newIndex, std::vector<Vec3>& vertices, std::vector<Vec3>& normals);
    // parcours les feuilles de l'octree pour réindexer les triangles vers les indices des sommets correctes
    // et ajoute les sommets moyens et sa normales dans "vertices" et "normals"

private:

};

#endif // OCTREE_H