#pragma once

#include <Simp1e/UI/Qt.h>

#include <QAction>
#include <QApplication>
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QStyle>
#include <QToolBar>
#include <QWidget>
#include <filesystem>

namespace MiniMapEditor {

    struct IEditor {};

    class EditorPreferences : public QDialog {
        QFormLayout* _layout;
        QLabel       _lbl_DataFolder{"Data Folder:"};
        QLineEdit    _txt_DataFolder;
        QPushButton  _btn_Browse;
        QPushButton  _btn_Accept{"Accept"};
        QPushButton  _btn_Cancel{"Cancel"};

    public:
        EditorPreferences() : QDialog() {
            Configure();
            Layout();
            Events();
        }

    private:
        void Configure() {
            setWindowTitle("Preferences");
            _txt_DataFolder.setText(GetDefaultPath().string().c_str());
            _btn_Browse.setIcon(style()->standardIcon(QStyle::SP_DirIcon));
        }

        void Layout() {
            setMinimumWidth(600);
            _layout = new QFormLayout(this);
            _layout->addRow(&_lbl_DataFolder, &_txt_DataFolder);
            _layout->addRow("", &_btn_Browse);
            _layout->addRow(&_btn_Accept, &_btn_Cancel);
        }

        void Events() {
            QObject::connect(&_btn_Accept, &QPushButton::clicked, this, &QDialog::accept);
            QObject::connect(&_btn_Cancel, &QPushButton::clicked, this, &EditorPreferences::close);
            QObject::connect(
                &_btn_Browse, &QPushButton::clicked, this, &EditorPreferences::ChooseFolder
            );
            QObject::connect(
                &_txt_DataFolder, &QLineEdit::textChanged, this,
                &EditorPreferences::OnFolderPathChanged
            );
        }

        std::filesystem::path GetDefaultPath() {
            return std::filesystem::path{
                       QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                           .toStdString()} /
                   "MiniMapEditor";
        }

        void ChooseFolder() {
            auto directory = QFileDialog::getExistingDirectory(
                this, "Choose Data Folder", _txt_DataFolder.text()
            );
            if (!directory.isEmpty()) _txt_DataFolder.setText(directory);
        }

        void OnFolderPathChanged() {
            if (std::filesystem::exists(_txt_DataFolder.text().toStdString()))
                _btn_Accept.setEnabled(true);
            else _btn_Accept.setEnabled(false);
        }
    };

    class Editor : public QMainWindow {
        EditorPreferences     _preferences;
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
        void OnMapPreferences() { _preferences.show(); }
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
            auto preferencesAction = toolbar->addAction("Preferences");
            auto helpAction        = toolbar->addAction("Help");
            auto quitAction        = toolbar->addAction("Quit");

            newAction->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
            loadAction->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
            saveAction->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
            preferencesAction->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
            helpAction->setIcon(style()->standardIcon(QStyle::SP_DialogHelpButton));
            quitAction->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));

            QObject::connect(newAction, &QAction::triggered, this, &Editor::OnMapNew);
            QObject::connect(loadAction, &QAction::triggered, this, &Editor::OnMapLoad);
            QObject::connect(saveAction, &QAction::triggered, this, &Editor::OnMapSave);
            QObject::connect(
                preferencesAction, &QAction::triggered, this, &Editor::OnMapPreferences
            );
            QObject::connect(helpAction, &QAction::triggered, this, &Editor::OnAbout);
            QObject::connect(quitAction, &QAction::triggered, this, &Editor::OnQuit);
        }

        void SetupMenu() {
            auto* menu = menuBar();

            auto fileMenu        = menu->addMenu("&File");
            auto fileNew         = fileMenu->addAction("New");
            auto fileLoad        = fileMenu->addAction("Load");
            auto fileSave        = fileMenu->addAction("Save");
            auto filePreferences = fileMenu->addAction("Preferences");
            fileMenu->addSeparator();
            auto fileQuit = fileMenu->addAction("Quit");

            auto helpMenu  = menu->addMenu("Help");
            auto helpAbout = helpMenu->addAction("About");

            QObject::connect(fileNew, &QAction::triggered, this, &Editor::OnMapNew);
            QObject::connect(fileLoad, &QAction::triggered, this, &Editor::OnMapLoad);
            QObject::connect(fileSave, &QAction::triggered, this, &Editor::OnMapSave);
            QObject::connect(filePreferences, &QAction::triggered, this, &Editor::OnMapPreferences);
            QObject::connect(fileQuit, &QAction::triggered, this, &Editor::OnQuit);
            QObject::connect(helpAbout, &QAction::triggered, this, &Editor::OnAbout);

            fileNew->setShortcut(QKeySequence::New);
            fileLoad->setShortcut(QKeySequence::Open);
            fileSave->setShortcut(QKeySequence::Save);
            filePreferences->setShortcut(QKeySequence::Preferences);
            fileQuit->setShortcut(QKeySequence::Quit);
            helpAbout->setShortcut(QKeySequence::HelpContents);
        }
    };
}