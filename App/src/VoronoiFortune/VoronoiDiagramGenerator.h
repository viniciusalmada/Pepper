/*
 * The author of this software is Steven Fortune.  Copyright (c) 1994 by AT&T
 * Bell Laboratories.
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHORS NOR AT&T MAKE ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */

/*
 * This code was originally written by Stephan Fortune in C code.  I, Shane O'Sullivan,
 * have since modified it, encapsulating it in a C++ class and, fixing memory leaks and
 * adding accessors to the Voronoi Edges.
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHORS NOR AT&T MAKE ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */

#ifndef VORONOI_DIAGRAM_GENERATOR
#define VORONOI_DIAGRAM_GENERATOR

#include <cstddef>

#define DELETED -2

#define le 0
#define re 1

struct Freenode
{
  Freenode* nextfree;
};

struct FreeNodeArrayList
{
  Freenode* memory;
  FreeNodeArrayList* next;
};

struct Freelist
{
  Freenode* head;
  int nodesize;
};

struct Point
{
  float x, y;
};

// structure used both for sites and for vertices
struct Site
{
  Point coord;
  int refcnt;
};

struct Edge
{
  float a, b, c;
  Site* ep[2];
  Site* reg[2];
};

struct GraphEdge
{
  float x1, y1, x2, y2;
  GraphEdge* next;
};

struct Halfedge
{
  Halfedge *ELleft, *ELright;
  Edge* ELedge;
  int ELrefcnt;
  bool ELpm;
  Site* vertex;
  float ystar;
  Halfedge* PQnext;
};

class VoronoiDiagramGenerator
{
public:
  VoronoiDiagramGenerator();
  ~VoronoiDiagramGenerator();

  bool GenerateVoronoi(std::vector<float> xValues,
                       std::vector<float> yValues,
                       int numPoints,
                       float minX,
                       float maxX,
                       float minY,
                       float maxY,
                       float minDist = 0);

  void resetIterator() { iteratorEdges = allEdges; }

  bool getNext(float& x1, float& y1, float& x2, float& y2)
  {
    if (iteratorEdges == nullptr)
      return false;

    x1 = iteratorEdges->x1;
    x2 = iteratorEdges->x2;
    y1 = iteratorEdges->y1;
    y2 = iteratorEdges->y2;

    iteratorEdges = iteratorEdges->next;

    return true;
  }

private:
  void cleanup();
  void cleanupEdges();
  char* getfree(Freelist* fl);
  [[nodiscard]] bool PQempty() const;

  Halfedge** ELhash{};
  Halfedge* HEcreate(Edge* e, bool pm);

  Point PQ_min();
  Halfedge* PQextractmin();
  static void freeinit(Freelist* fl, int size);
  static void makefree(Freenode* curr, Freelist* fl);
  void geominit();
  bool voronoi();
  static void ref(Site* v);
  void deref(Site* v);
  void endpoint(Edge* e, int lr, Site* s);

  static void ELdelete(Halfedge* he);
  Halfedge* ELleftbnd(Point* p);
  static Halfedge* ELright(Halfedge* he);
  void makevertex();

  void PQinsert(Halfedge* he, Site* v, float offset);
  void PQdelete(Halfedge* he);
  bool ELinitialize();
  static void ELinsert(Halfedge* lb, Halfedge* newHe);
  Halfedge* ELgethash(int b);
  static Halfedge* ELleft(Halfedge* he);
  Site* leftreg(Halfedge* he);
  bool PQinitialize();
  int PQbucket(Halfedge* he);
  void clip_line(Edge* e);
  char* myalloc(unsigned n);
  static int right_of(Halfedge* el, Point* p);

  Site* rightreg(Halfedge* he);
  Edge* bisect(Site* s1, Site* s2);
  static float dist(Site* s, Site* t);
  Site* intersect(Halfedge* el1, Halfedge* el2);

  Site* nextone();

  void pushGraphEdge(float x1, float y1, float x2, float y2);

  void line(float x1, float y1, float x2, float y2);

  Freelist hfl{};
  Halfedge *ELleftend{}, *ELrightend{};
  int ELhashsize{};

  float xmin{}, xmax{}, ymin{}, ymax{}, deltax{}, deltay{};

  std::vector<Site> sites;
  int nsites{};
  int siteidx;
  int sqrt_nsites{};
  int nvertices{};
  Freelist sfl{};
  Site* bottomsite{};

  int nedges{};
  Freelist efl{};
  int PQhashsize{};
  Halfedge* PQhash{};
  int PQcount{};
  int PQmin{};

  int ntry{}, totalsearch{};
  float pxmin{}, pxmax{}, pymin{}, pymax{};

  float borderMinX{}, borderMaxX{}, borderMinY{}, borderMaxY{};

  FreeNodeArrayList* allMemoryList;
  FreeNodeArrayList* currentMemoryBlock;

  GraphEdge* allEdges;
  GraphEdge* iteratorEdges;

  float minDistanceBetweenSites;
};

#endif
