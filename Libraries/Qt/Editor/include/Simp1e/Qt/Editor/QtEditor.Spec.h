#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include "QtEditor.h"
#include "SpecHelper.h"

Describe("QtEditor") {
    it("can do something with actual QWidgets?", []() {
        int   argc = 0;
        char* argv[0];
        TheStuffFromMainGoesHere(argc, argv);
        // QApplication app{argc, argv};
        // QWidget      window;
        // window.setWindowTitle("Simple example");
        // AssertThat(window.windowTitle().toStdString(), Equals("Simple example"));
        // QVBoxLayout layout{&window};
        // QLabel      title{"TITLE!"};
        // title.setObjectName("the name");
        // layout.addWidget(&title);
        // window.setLayout(&layout);

        // // Let's introspect the children
        // auto children = window.children();
        // AssertThat(children[1]->objectName().toStdString(), Equals("TITLE!????"));
    });
}
