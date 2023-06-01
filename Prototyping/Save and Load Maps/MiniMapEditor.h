#pragma once

#include <Simp1e/UI/Qt.h>

#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QStyle>
#include <QToolBar>
#include <QWidget>
#include <filesystem>

namespace MiniMapEditor {

    class Editor : public QMainWindow {
        std::filesystem::path _logoPath;

    public:
        Editor() {
            _logoPath = std::filesystem::path(std::getenv("Openclipart")) / "274099.png";
            setWindowTitle("Mini Map Editor");
            SetupMenu();
            SetupToolbar();
        }

        static int Run() {
            Editor editor;
            editor.show();
            return Simp1e::UI::Qt::Run();
        }

    private:
        void OnMapNew() { QMessageBox::information(this, "New Map", "New Map"); }
        void OnMapLoad() { QMessageBox::information(this, "Load Map", "Load Map"); }
        void OnMapSave() { QMessageBox::information(this, "Save Map", "Save Map"); }
        void OnQuit() { QMessageBox::information(this, "Quit", "Quit"); }
        void OnAbout() {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Mini Map Editor");
            msgBox.setText("Mini Map Editor\n(prototype alpha 0.0.1)");
            QPixmap pixmap(_logoPath.string().c_str());
            msgBox.setIconPixmap(pixmap.scaled(256, 256, Qt::KeepAspectRatio));
            msgBox.exec();
        }

        void SetupToolbar() {
            auto* toolbar = addToolBar("Main Toolbar");
            toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

            auto newAction  = toolbar->addAction("New");
            auto loadAction = toolbar->addAction("Load");
            auto saveAction = toolbar->addAction("Save");
            toolbar->addSeparator();
            auto helpAction = toolbar->addAction("Help");
            auto quitAction = toolbar->addAction("Quit");

            newAction->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
            loadAction->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
            saveAction->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
            helpAction->setIcon(style()->standardIcon(QStyle::SP_DialogHelpButton));
            quitAction->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));

            QObject::connect(newAction, &QAction::triggered, this, &Editor::OnMapNew);
            QObject::connect(loadAction, &QAction::triggered, this, &Editor::OnMapLoad);
            QObject::connect(saveAction, &QAction::triggered, this, &Editor::OnMapSave);
            QObject::connect(helpAction, &QAction::triggered, this, &Editor::OnAbout);
            QObject::connect(quitAction, &QAction::triggered, this, &Editor::OnQuit);
        }

        void SetupMenu() {
            auto* menu = menuBar();

            auto fileMenu = menu->addMenu("&File");
            auto fileNew  = fileMenu->addAction("New");
            auto fileLoad = fileMenu->addAction("Load");
            auto fileSave = fileMenu->addAction("Save");
            fileMenu->addSeparator();
            auto fileQuit = fileMenu->addAction("Quit");

            auto helpMenu  = menu->addMenu("Help");
            auto helpAbout = helpMenu->addAction("About");

            QObject::connect(fileNew, &QAction::triggered, this, &Editor::OnMapNew);
            QObject::connect(fileLoad, &QAction::triggered, this, &Editor::OnMapLoad);
            QObject::connect(fileSave, &QAction::triggered, this, &Editor::OnMapSave);
            QObject::connect(fileQuit, &QAction::triggered, this, &Editor::OnQuit);
            QObject::connect(helpAbout, &QAction::triggered, this, &Editor::OnAbout);

            fileNew->setShortcut(QKeySequence::New);
            fileLoad->setShortcut(QKeySequence::Open);
            fileSave->setShortcut(QKeySequence::Save);
            fileQuit->setShortcut(QKeySequence::Quit);
            helpAbout->setShortcut(QKeySequence::HelpContents);
        }
    };
}