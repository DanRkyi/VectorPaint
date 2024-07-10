#ifndef _VectorPaint_Shapes_h_
#define _VectorPaint_Shapes_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;
using namespace std;

class Shape {
    friend class OperationsHelper;

protected:
    String name;
    Color color;
    Color fillColor;
    int width;
    bool selected;
    Vector<Point> points;

public:
    Shape(String nm) : name(nm), width(1), selected(true), color(Black()), fillColor(White()) {}
    virtual ~Shape() {}

    void setSelect(bool sel) { selected = sel; }
    int size() { return points.size(); }
    virtual void setponit(int i, Point pnt) {
        if (i < points.size()) points[i] = pnt;
    }
    String getName() { return name; }
    void setName(String an) { name = an; }
    Color getColor() { return color; }
    void setColor(Color cl) { color = cl; }
    Color getFillColor() { return fillColor; }
    void setFillColor(Color cl) { fillColor = cl; }
    int getWidth() { return width; }
    void setWidth(int w) { width = w; }
    virtual void paint(Draw& w) {
        if (selected)
            for (int i = 0; i < size(); i++)
                w.DrawEllipse(points[i].x - 5, points[i].y - 5, 10, 10, Red());
    }
    virtual void moveto(Point to) = 0;
};

class Line : public Shape {
public:
    Line(String name = "line") : Shape(name) {
        points.push_back(Point(100, 100));
        points.push_back(Point(300, 300));
    }

    virtual void paint(Draw& w) override {
        Shape::paint(w);
        w.DrawLine(points[0], points[1], width, color);
    }

    virtual void moveto(Point to) override {
        int dx = points[0].x - points[1].x;
        int dy = points[0].y - points[1].y;
        points[0] = to;
        points[1] = Point(to);
        points[1].Offset(-dx, -dy);
    }
};

class Triangle : public Line {
public:
    Triangle(String name = "triangle") : Line(name) {
        points.push_back(Point(100, 300));
    }

    virtual void paint(Draw& w) override {
        Shape::paint(w);
        w.DrawLine(points[0], points[1], width, color);
        w.DrawLine(points[1], points[2], width, color);
        w.DrawLine(points[2], points[0], width, color);
    }

    virtual void moveto(Point to) override {
        int dx = points[0].x - points[1].x;
        int dy = points[0].y - points[1].y;
        int dx1 = points[0].x - points[2].x;
        int dy1 = points[0].y - points[2].y;
        points[0] = to;
        points[1] = Point(to);
        points[1].Offset(-dx, -dy);
        points[2] = Point(to);
        points[2].Offset(-dx1, -dy1);
    }
};

class TextShape : public Shape {
public:
    TextShape(String name = "text") : Shape(name) {
        points.push_back(Point(100, 100));
    }

    virtual void paint(Draw& w) override {
        Shape::paint(w);
        w.DrawText(points[0].x, points[0].y, name, Arial(10 + width), color);
    }

    virtual void moveto(Point to) override {
        points[0] = to;
    }
};

class Mandelbrot : public Line {
private:
    Image img;
    long double scale = 0;
    long double yscale = 0;
    const int maxIt = 100;

    int num_iters(double cx, double cy) {
        std::complex<double> z(0.0, 0.0);
        std::complex<double> c(cx, cy);
        for (int i = 0; i <= maxIt; ++i) {
            if (std::abs(z) > 2.0) return i;
            z = z * z + c;
        }
        return maxIt;
    }

public:
    Mandelbrot(String name = "mandelbrot") : Line(name) {
        update();
    }

    virtual void setponit(int i, Point pnt) override {
        Shape::setponit(i, pnt);
        update();
    }

    virtual void paint(Draw& w) override {
        Shape::paint(w);
        w.DrawImage(points[0].x, points[0].y, img);
    }

    void update() {
        int width = points[1].x - points[0].x;
        int height = points[1].y - points[0].y;
        if (width <= 0 || height <= 0) return;
        ImageBuffer ib(width, height);
        RGBA* t = ~ib;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double cx = (double)x / width * 2.0 / (1.0 + scale) - 1.5;
                double cy = (double)y / height * 2.0 / (1.0 + yscale) - 1.0;
                int iterations = num_iters(cx, cy);
                Color color = HsvColorf(double(1.0 / iterations), 1, (iterations < maxIt));
                *t++ = color;
            }
        }
        img = ib;
    }

    long double& getScale() { return scale; }
    long double& getYScale() { return yscale; }
};

class EllipseShape : public Shape {
public:
    EllipseShape(String name = "ellipse") : Shape(name) {
        points.push_back(Point(100, 100));
        points.push_back(Point(200, 150));
    }

    virtual void paint(Draw& w) override {
        Shape::paint(w);
        int width = abs(points[1].x - points[0].x);
        int height = abs(points[1].y - points[0].y);
        w.DrawEllipse(points[0].x, points[0].y, width, height, fillColor, this->width, color);
    }

    virtual void moveto(Point to) override {
        int dx = points[1].x - points[0].x;
        int dy = points[1].y - points[0].y;
        points[0] = to;
        points[1] = Point(to.x + dx, to.y + dy);
    }
};

#endif