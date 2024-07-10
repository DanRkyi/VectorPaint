#ifndef _VectorPaint_Propertys_h_
#define _VectorPaint_Propertys_h_

#include <VectorPaint/Shapes.h>
#include <CtrlLib/CtrlLib.h>

using namespace Upp;

// Класс помощник ввода свойст фигур

class PropertyHelper {
private:
    static bool fractDialog(Mandelbrot* shp) {
        if (shp == NULL) return false;

        TopWindow app;
        app.SetRect(0, 0, Zx(200), Zy(120));
        EditString name;
        name <<= shp->getName();

        Button ok;
        SliderCtrl sld, sldy;
        sld.MinMax(-100, 100); sld <<= 0;
        sldy.MinMax(-100, 100); sldy <<= 0;

        app.Add(name.TopPosZ(0, 20).HSizePos());
        app.Add(sld.TopPosZ(40, 20).HSizePos());
        app.Add(sldy.TopPosZ(60, 20).HSizePos());

        ok << [&app, shp, &sld, &sldy, &name] {
            app.Close();
            shp->setName(name.GetText().ToString().ToStd());
            shp->getScale() = shp->getScale() + double(~sld) / 100.0;
            shp->getYScale() = shp->getYScale() + double(~sldy) / 100.0;
            shp->update();
        };
        app.Add(ok.SetLabel("OK").LeftPos(10, 200).BottomPos(10, 40));

        app.Run();

        return true;
    }

    // Подходит простым фигурам
    static void commDialog(Shape* shp) {
        TopWindow app;
        Button ok;
        app.SetRect(0, 0, Zx(200), Zy(90));

        EditString name; // поле ввода
        ColorPusher col; // выбор цвета
        SliderCtrl sld;  // ползунок толщины линий

        name <<= shp->getName();
        col <<= shp->getColor(); // Устанавливаем текущий цвет
        sld <<= shp->getWidth();
        sld.MinMax(1, 100);

        app.Add(name.TopPosZ(0, 20).HSizePos());
        app.Add(col.TopPosZ(20, 20).HSizePos());
        app.Add(sld.TopPosZ(40, 20).HSizePos());

        ok << [&app, shp, &name, &col, &sld] {
            app.Close();
            shp->setName(name.GetText().ToString().ToStd());
            shp->setColor(~col); // Сохраняем выбранный цвет
            shp->setWidth(~sld);
        };
        app.Add(ok.SetLabel("OK").LeftPos(10, 200).BottomPos(10, 40));

        app.Run();
    }

public:
    static void showDialog(Shape* shp) {
        // Добавить новую функцию для ввода особенных свойст вигур
        if (fractDialog(dynamic_cast<Mandelbrot*>(shp)));
        else commDialog(shp);
    }
};

#endif
