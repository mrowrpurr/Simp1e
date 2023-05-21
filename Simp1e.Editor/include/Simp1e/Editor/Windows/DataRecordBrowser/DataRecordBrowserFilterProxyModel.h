#pragma once

#include <QSortFilterProxyModel>

namespace Simp1e::Editor::Windows::DataRecordBrowser {

    class DataRecordFilterProxyModel : public QSortFilterProxyModel {
    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override {
            QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

            if (sourceModel()->rowCount(index) > 0) {
                for (int i = 0; i < sourceModel()->rowCount(index); ++i) {
                    if (filterAcceptsRow(i, index)) {
                        return true;
                    }
                }
            }

            return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
        }
    };
}