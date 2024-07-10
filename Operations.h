#ifndef _VectorPaint_Operations_h_
#define _VectorPaint_Operations_h_

#include <VectorPaint/Shapes.h>
#include <CtrlLib/CtrlLib.h>

using namespace Upp;

// Операции преобразования фигур, поворот, масштаб
class OperationsHelper {
private:
    static Point center(Shape* shp) {
        Point c = { 0, 0 };
        for (int i = 0; i < shp->size(); i++) {
            c.x += shp->points[i].x;
            c.y += shp->points[i].y;
        }
        c.x /= shp->size();
        c.y /= shp->size();
        return c;
    }

public:
    static void rotate(Shape* shp, int angle) {
        double rad = angle * M_PI / 180;
        Point cnt = center(shp);
        for (int i = 0; i < shp->size(); i++) {
            Point p = shp->points[i];
            int x = (p.x - cnt.x) * cos(rad) - (p.y - cnt.y) * sin(rad) + cnt.x;
            int y = (p.x - cnt.x) * sin(rad) + (p.y - cnt.y) * cos(rad) + cnt.y;
            shp->points[i] = Point(x, y);
        }
    }

    static void scale(Shape* shp, double k) {
        Point cnt = center(shp);
        for (int i = 0; i < shp->size(); i++) {
            Point p = shp->points[i];
            int x = (p.x - cnt.x) * k + cnt.x;
            int y = (p.y - cnt.y) * k + cnt.y;
            shp->points[i] = Point(x, y);
        }
    }
};

#endif
