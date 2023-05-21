#pragma once

#include <string_format.h>

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QStyle>
#include <QTextEdit>
#include <QTreeView>
#include <QVBoxLayout>
#include <QtGlobal>

#include "../IApp.h"
#include "DataFilesSelector/DataFilesListStandardItemModel_Q.h"

#ifndef Q_MOC_RUN
    #include "../Paths.h"
    #include "../Strings.h"
#endif

namespace Simp1e::Editor::Windows {

    class DataFilesSelectorWindow : public QWidget {
        Q_OBJECT

        IApp* _app;

#pragma region Widget Variables
        QVBoxLayout*                                      _layout_Window;
        QHBoxLayout*                                      _layout_DataFolder;
        QHBoxLayout*                                      _layout_DataFiles_Buttons;
        QVBoxLayout*                                      _layout_grp_DataFiles;
        QLabel                                            _lbl_WindowTitle{"Simp1e"};
        QLabel                                            _lbl_WindowTitleImage;
        QPixmap                                           _pixmap_WindowTitleImage{":/logo.png"};
        QGroupBox                                         _grp_DataFiles;
        QLineEdit                                         _txt_DataFolder;
        QPushButton                                       _btn_SelectFolder{"Select Folder"};
        QTreeView                                         _tree_DataFiles;
        DataFilesSelector::DataFilesListStandardItemModel _model_DataFiles;
        QPushButton                                       _btn_DataFiles_SetActive{"Set Active"};
        QPushButton                                       _btn_Continue{"Continue"};
#pragma endregion

    public:
        DataFilesSelectorWindow(IApp* app) : _app(app), QWidget(nullptr) {
            IDs();
            Layout();
            Events();
            Configure();
        }

    private:
#pragma region Widget Setup

        void IDs() {
            setObjectName("DataFilesSelectorWindow");
            _lbl_WindowTitle.setObjectName("WindowTitle");
            _lbl_WindowTitle.setProperty("class", "title");
            _lbl_WindowTitleImage.setObjectName("WindowTitleImage");
            _lbl_WindowTitleImage.setProperty("class", "image");
            _btn_Continue.setObjectName("Continue");
            _btn_Continue.setProperty("class", "primary");
            _btn_SelectFolder.setObjectName("SelectFolder");
            _btn_SelectFolder.setProperty("class", "primary");
            _btn_DataFiles_SetActive.setObjectName("DataFiles_SetActive");
            _btn_DataFiles_SetActive.setProperty("class", "secondary");
        }

        void Layout() {
            _layout_Window            = new QVBoxLayout();
            _layout_DataFolder        = new QHBoxLayout();
            _layout_DataFiles_Buttons = new QHBoxLayout();
            _layout_grp_DataFiles     = new QVBoxLayout();

            _layout_DataFolder->addWidget(new QLabel("Data Folder:"));
            _layout_DataFolder->addWidget(&_txt_DataFolder);
            _layout_DataFolder->setStretchFactor(&_txt_DataFolder, 1);

            _layout_DataFiles_Buttons->addWidget(&_btn_DataFiles_SetActive);
            _layout_DataFiles_Buttons->addWidget(&_btn_Continue);

            _grp_DataFiles.setLayout(_layout_grp_DataFiles);
            _layout_grp_DataFiles->addLayout(_layout_DataFolder);
            _layout_grp_DataFiles->addWidget(&_btn_SelectFolder);
            _layout_grp_DataFiles->addWidget(&_tree_DataFiles);
            _layout_grp_DataFiles->addLayout(_layout_DataFiles_Buttons);

            _layout_Window->addWidget(&_lbl_WindowTitleImage, 0, Qt::AlignCenter);
            _layout_Window->addWidget(&_lbl_WindowTitle, 0, Qt::AlignCenter);
            _layout_Window->addWidget(&_grp_DataFiles);

            setLayout(_layout_Window);
        }

        void Configure() {
            setWindowTitle(Strings::WindowTitle);
            setMinimumWidth(800);
            _lbl_WindowTitleImage.setPixmap(_pixmap_WindowTitleImage);
            _lbl_WindowTitleImage.setScaledContents(true);
            _tree_DataFiles.setModel(&_model_DataFiles);
            _tree_DataFiles.setSortingEnabled(true);
            _tree_DataFiles.setDragDropMode(QAbstractItemView::InternalMove);
            _tree_DataFiles.setDragEnabled(true);
            _tree_DataFiles.setAcceptDrops(true);
            _tree_DataFiles.setDropIndicatorShown(true);
            _tree_DataFiles.header()->setSectionResizeMode(0, QHeaderView::Stretch);
            _tree_DataFiles.header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
            _btn_DataFiles_SetActive.setDisabled(true);
            _btn_Continue.setDisabled(true);
            _txt_DataFolder.setText(GetDefaultFolderChooserPath().string().c_str());
        }

        void Events() {
            connect(
                &_btn_SelectFolder, &QPushButton::clicked, this,
                &DataFilesSelectorWindow::on_btn_SelectFolder_clicked
            );
            connect(
                &_btn_DataFiles_SetActive, &QPushButton::clicked, this,
                &DataFilesSelectorWindow::on_btn_DataFiles_SetActive_clicked
            );
            connect(
                &_txt_DataFolder, &QLineEdit::textChanged, this,
                &DataFilesSelectorWindow::on_txt_DataFolder_textChanged
            );
            connect(
                &_tree_DataFiles, &QTreeView::clicked, this,
                &DataFilesSelectorWindow::on_tree_DataFiles_clicked
            );
            connect(
                &_model_DataFiles, &DataFilesSelector::DataFilesListStandardItemModel::itemChanged,
                this, &DataFilesSelectorWindow::on_model_DataFiles_itemChanged
            );
            connect(&_btn_Continue, &QPushButton::clicked, [&]() {
                QString message = "Data Files to Load:\n";
                for (int i = 0; i < _model_DataFiles.rowCount(); ++i) {
                    if (_model_DataFiles.item(i, 0)->checkState() == Qt::Checked) {
                        auto name      = _model_DataFiles.item(i, 0)->text();
                        auto loadOrder = _model_DataFiles.item(i, 1)->data(Qt::DisplayRole).toInt();
                        auto isActive = _model_DataFiles.item(i, 2)->data(Qt::DisplayRole).toBool();
                        if (isActive)
                            message +=
                                string_format("{} [{}] (Active)\n", name.toStdString(), loadOrder)
                                    .c_str();
                        else
                            message +=
                                string_format("{} [{}]\n", name.toStdString(), loadOrder).c_str();
                    }
                }
                QMessageBox::information(this, "Data Files", message);
                _app->StartUpUsingDataFiles({"A", "B"});
            });
        }

#pragma endregion
#pragma region Event Handlers

        void on_btn_SelectFolder_clicked() {
            auto dir = QFileDialog::getExistingDirectory(
                this, "Choose Data Files Directory", GetDefaultFolderChooserPath().string().c_str(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
            );
            if (!dir.isEmpty()) _txt_DataFolder.setText(dir);
        }

        void on_tree_DataFiles_clicked(const QModelIndex& index) {
            _btn_DataFiles_SetActive.setDisabled(false);
        }

        void on_btn_DataFiles_SetActive_clicked() {
            QModelIndexList selectedRows = _tree_DataFiles.selectionModel()->selectedRows();
            if (selectedRows.size() == 1) _model_DataFiles.setActive(selectedRows[0].row());
        }

        void on_txt_DataFolder_textChanged(const QString& newPath) { ChangeFolder(newPath); }

        void on_model_DataFiles_itemChanged(QStandardItem* item) {
            if (_model_DataFiles.IsPerformingReorder()) return;

            if (item->isCheckable() && item->checkState() == Qt::Unchecked)
                _model_DataFiles.setActive(item->row(), false);

            // If at least 1 item is checked then enable btn_Continue
            for (int i = 0; i < _model_DataFiles.rowCount(); ++i) {
                QStandardItem* checkboxItem = _model_DataFiles.item(i, 0);
                if (checkboxItem->checkState() == Qt::Checked) {
                    _btn_Continue.setDisabled(false);
                    return;
                }
            }
            _btn_Continue.setDisabled(true);
        }

#pragma endregion
#pragma region Private Functions

        std::filesystem::path GetDefaultFolderChooserPath() {
            // Might want the home dir in the future...
            // auto homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

            // But, for now, let's use the development data folder (if it exists)
            if (std::filesystem::exists(Simp1e::Editor::Paths::Development::DataFolder))
                return std::filesystem::canonical(Simp1e::Editor::Paths::Development::DataFolder);
            else
                return QStandardPaths::writableLocation(QStandardPaths::HomeLocation).toStdString();
        }

        void ChangeFolder(const QString& newPath) {
            _model_DataFiles.removeRows(0, _model_DataFiles.rowCount());
            _btn_DataFiles_SetActive.setDisabled(true);
            _btn_Continue.setDisabled(true);

            QFileInfo checkFile(newPath);
            if (checkFile.isDir() && checkFile.exists()) {
                QDir        dir(newPath);
                QStringList fileList = dir.entryList(QStringList() << "*.json", QDir::Files);

                unsigned int i = 0;
                for (const QString& file : fileList) {
                    auto* filenameColumn = new QStandardItem(file);
                    filenameColumn->setCheckable(true);
                    auto* loadOrderColumn = new QStandardItem();
                    loadOrderColumn->setData(i, Qt::DisplayRole);
                    auto* isActiveColumn = new QStandardItem();
                    isActiveColumn->setData(false, Qt::DisplayRole);
                    _model_DataFiles.appendRow({filenameColumn, loadOrderColumn, isActiveColumn});
                    ++i;
                }
                _model_DataFiles.updateOrder();

                // valid directory, so remove the invalid style
                _txt_DataFolder.setProperty("invalid", QVariant(false));
                _txt_DataFolder.style()->unpolish(&_txt_DataFolder);
                _txt_DataFolder.style()->polish(&_txt_DataFolder);
            } else {
                // invalid directory, so apply the invalid style
                _txt_DataFolder.setProperty("invalid", QVariant(true));
                _txt_DataFolder.style()->unpolish(&_txt_DataFolder);
                _txt_DataFolder.style()->polish(&_txt_DataFolder);
            }
        }
#pragma endregion
    };
}
