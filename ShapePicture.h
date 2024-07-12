#ifndef _VectorPaint_Picture_h_
#define _VectorPaint_Picture_h_

#include <CtrlLib/CtrlLib.h>
#include "Shapes.h"

using namespace Upp;

// Изображение из фигур
// Хранит массив фигур изображения
// выводит изображение на форму
class ShapePicture {
private:
    Vector<Shape*> shps; // Массив фигур

public:
    ~ShapePicture() {
        for (int i = 0; i < shps.size(); i++)
            delete shps[i];
    }

    // Отрисовка всех фигур
    void paint(Draw& w) {
        for (int i = 0; i < shps.size(); i++)
            shps[i]->paint(w);
    }

    // Возвращает количество фигур
    int size() {
        return shps.size();
    }

    // Получение фигуры по индексу
    Shape* getShape(int i) {
        if (i < size())
            return shps[i];
        else
            return NULL;
    }

    // Добавление новой фигуры
    Shape* addShape(String kind) {
        Shape* sp = NULL;
        // Добавление новых типов фигур
        if (kind == "line") sp = new Line();
        if (kind == "tri") sp = new Triangle();
        if (kind == "frc") sp = new Mandelbrot();
        if (kind == "text") sp = new TextShape();
        if (kind == "ellipse") sp = new EllipseShape();

        if (sp != NULL) shps.push_back(sp);
        else Upp::PromptOK("unknown shape: " << kind);
        return sp;
    }
};

#endif
