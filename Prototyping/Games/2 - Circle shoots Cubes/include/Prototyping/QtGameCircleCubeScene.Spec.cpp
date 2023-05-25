#define spec_name QtGameCircleCubeScene

#include "QtGameCircleCubeScene.h"

#include <QApplication>

#include "SpecHelper.h"

using namespace Prototyping;

Describe("QtGameCircleCubeScene") {
    it("can specify cell width and height", []() {
        int          argc = 0;
        char**       argv = nullptr;
        QApplication app(argc, argv);

        QtGameCircleCubeScene scene;
        AssertThat(scene.GetCellWidth(), Equals(0));
        AssertThat(scene.GetCellHeight(), Equals(0));

        QtGameCircleCubeScene scene2(5, 8);
        AssertThat(scene2.GetCellWidth(), Equals(5));
        AssertThat(scene2.GetCellHeight(), Equals(8));
    });

    // it("Can render in a view", []() {
    //     AssertThat(1, Equals(1));
    // });
}