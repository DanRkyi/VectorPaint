#ifndef _VectorPaint_PictureController_h_
#define _VectorPaint_PictureController_h_

#include <CtrlLib/CtrlLib.h>
#include "Shapes.h"
#include "ShapePicture.h"

using namespace Upp;

// Контроллер изображения,
// позволяет контролировать ввод мышью,
// перемещение или измение координат фигур

class PictureController : public Ctrl {
    enum mouseInputState { none, input, movoto, inputStart, resize };

private:
    ShapePicture* picture;
    Shape* selShape = NULL;
    mouseInputState state;
    int clickCount;
    int max;
    Image cursor;

    void setCursor() {
        cursor = OverrideCursor(CtrlImg::cross());
    }

public:
    void init(ShapePicture* pic) {
        picture = pic;
        state = mouseInputState::none;
    }

    void startInput(Shape* shp) {
        state = mouseInputState::inputStart;
        selShape = shp;
        max = shp->size();
        clickCount = 0;
        setCursor();
    }

    void startMove(Shape* shp) {
        state = mouseInputState::movoto;
        selShape = shp;
        max = 1;
        clickCount = 0;
        setCursor();
    }

    void startResize(Shape* shp) {
        state = mouseInputState::resize;
        selShape = shp;
        max = shp->size();
        clickCount = 0;
        setCursor();
    }

    void Paint(Draw& w) override {
        w.DrawRect(GetSize(), White());
        picture->paint(w);
    }

    void MouseMove(Point p, dword keyflags) override {
        if (selShape != NULL)
            switch (state) {
            case mouseInputState::inputStart:
                selShape->moveto(p);
                break;
            case mouseInputState::resize:
                selShape->setponit(clickCount, p);
                break;
            case mouseInputState::movoto:
                selShape->moveto(p);
                break;
            default:;
            }
        Refresh();
    }

    void LeftDown(Point, dword) override {
        if (state == mouseInputState::inputStart)
            state = mouseInputState::resize;

        clickCount++;
        if (clickCount >= max) {
            selShape = NULL;
            state = mouseInputState::none;
            OverrideCursor(cursor);
        }

        Refresh();
    }
};

#endif
