#ifndef _VectorPaint_MainForm_h_
#define _VectorPaint_MainForm_h_

#include <CtrlLib/CtrlLib.h>
#include "PictureController.h"
#include "ShapePicture.h"
#include "Propertys.h"
#include "Operations.h"

#define LAYOUTFILE <VectorPaint/MainForm.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS Icons
#define IMAGEFILE <VectorPaint/iconsforproject.iml>
#include <Draw/iml.h>

using namespace Upp;

// Главная форма программы
// содержит изображение
class MainForm : public WithMainFormLayout<TopWindow> {
private:
    MenuBar menu;        // Меню
    ToolBar tool;        // Панель инструментов
    ShapePicture pict;   // Объект, содержащий фигуры

    // Обновление списка фигур
    void updateList(bool newshape = false) {
        int c = nameList.GetCursor();
        nameList.Clear();
        for (int i = 0; i < pict.size(); i++) {
            nameList.Add(pict.getShape(i)->getName());
        }
        if (newshape)
            nameList.SetCursor(pict.size() - 1);
        else
            nameList.SetCursor(c);
    }

    // Обновление выбранной фигуры
    void updateSelected() {
        int c = nameList.GetCursor();
        for (int i = 0; i < pict.size(); i++)
            pict.getShape(i)->setSelect(i == c);
    }

    // Выход из программы
    void Exit() {
        if (PromptOKCancel("Exit Paint?"))
            Break();
    }

    // Настройка панели инструментов
    void ToolBar(Bar& bar) {
        AddShapeButton(bar, "Line", Icons::line(), "line");
        AddShapeButton(bar, "Triangle", Icons::triangle(), "tri");
        AddShapeButton(bar, "Fractal", Icons::fractal(), "frc");
        AddShapeButton(bar, "Text", Icons::text(), "text");
        AddShapeButton(bar, "Ellipse", Icons::ellipse(), "ellipse");

        bar.Separator();
        bar.Add(CtrlImg::remove(), [=] { Exit(); }).Tip("Exit");
    }

    // Добавление кнопки фигуры на панель инструментов
    void AddShapeButton(Bar& bar, const char* tooltip, const Image& img, const String& shapeType) {
        bar.Add(img, [=] { BarFn(shapeType); }).Tip(tooltip);
    }

    // Обработчик меню
    void MenuFn() {
        PromptOK("Fn activated!");
    }

    // Обработчик панели инструментов
    void BarFn(String shapeType) {
        Shape* shp = pict.addShape(shapeType);
        pictController.startInput(shp);
        updateList(true);
    }

public:
    MainForm() {
        CtrlLayout(*this, "Vector paint");
        Zoomable().Sizeable();

        pictController.init(&pict);

        moveSelected << [=] {
            if (nameList.GetCursor() >= 0) {
                pictController.startMove(pict.getShape(nameList.GetCursor()));
            }
        };

        resizeSelected << [=] {
            if (nameList.GetCursor() >= 0) {
                pictController.startResize(pict.getShape(nameList.GetCursor()));
            }
        };

        proprtySelected << [=] {
            int c = nameList.GetCursor();
            if (c >= 0) {
                PropertyHelper::showDialog(pict.getShape(c));
                updateList();
            }
        };

        nameList.WhenSel << [=] {
            updateSelected();
            Refresh();
        };

        rotateButton << [=] {
            int c = nameList.GetCursor();
            if (c >= 0) {
                OperationsHelper::rotate(pict.getShape(c), 10);
                Refresh();
            }
        };

        rotateLeftButton << [=] {
            int c = nameList.GetCursor();
            if (c >= 0) {
                OperationsHelper::rotate(pict.getShape(c), -10);
                Refresh();
            }
        };

        scaleButton << [=] {
            int c = nameList.GetCursor();
            if (c >= 0) {
                OperationsHelper::scale(pict.getShape(c), 1.5);
                Refresh();
            }
        };
        scaleLeftButton << [=] {
            int c = nameList.GetCursor();
            if (c >= 0) {
                OperationsHelper::scale(pict.getShape(c), 1.0 / 1.5);
                Refresh();
            }
        };

        // Добавление панели инструментов
        AddFrame(tool);
        tool.Set([=](Bar& bar) { ToolBar(bar); });
    }
};

#endif
