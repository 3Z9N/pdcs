#ifndef EVENTTABLEMODEL_H
#define EVENTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <ctime>

/// Class EventTableModel - Custom ModelView for QTableView
class EventTableModel : public QAbstractTableModel {
public:
    /// constructor
    EventTableModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {
        priorityList << "Lowermost" << "Low" << "Medium" << "High" << "Highest";
    }

    /// Returns the number of table rows
    int rowCount(const QModelIndex &) const override { return mlist.size(); }

    /// Returns the number of table columns
    int columnCount(const QModelIndex &) const override { return 3; }

    /// \brief Returns an appropriate value for the requested data.
    /// If the view requests an invalid index, an invalid variant is returned.
    /// Any valid index that corresponds to a string in the list causes that
    /// string to be returned for the display role; otherwise an invalid variant
    /// is returned.
    /// \param index - data index
    /// \param role - data role
    QVariant data(const QModelIndex &index, int role) const override {
        const auto & args = mlist[index.row()];
        if (role == Qt::TextAlignmentRole) {
            switch (index.column()) {
            case 0: return Qt::AlignCenter;
            case 1: return Qt::AlignCenter;
            case 2: return Qt::AlignLeft+Qt::AlignVCenter;
            default: return {};
            }
        }
        if (role != Qt::DisplayRole && role != Qt::EditRole) return {};

        // time number to time string
        std::time_t t = std::stol(args[0]);
        struct std::tm *ptm = std::localtime(&t);
        char tme_buf[64];
        sprintf(tme_buf, "%d.%02d.%02d - %02d:%02d:%02d",
                ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday,
                ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

        // priority number to priority string
        int priority = std::stoi(args[1].c_str());
        if(priority > priorityList.size()-1) priority = priorityList.size()-1;
        if(priority < 0) priority = 0;
        QString prio_str = priorityList[priority];

        switch (index.column()) {
        case 0: return tme_buf;
        case 1: return prio_str;
        case 2: return args[2].c_str();
        default: return {};
        }
    }

    /// \brief Returns the appropriate header string depending on the orientation of the header and the section.
    /// If anything other than the display role is requested, we return an invalid variant.
    /// \param section - index of header
    /// \param orientation - item orientation
    /// \param role - header role
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
        switch (section) {
        case 0: return "Time";
        case 1: return "Priority";
        case 2: return "Informations";
        default: return {};
        }
    }

    /// Adds new data to the list.
    void append(const std::vector<std::string> & args) {
        beginInsertRows({}, mlist.count(), mlist.count());
        mlist.append(args);
        endInsertRows();
    }

    /// Clears the whole list.
    void clear() {
        beginResetModel();
        mlist.clear();
        endResetModel();
    }

private:
    QList<std::vector<std::string>> mlist;  ///< ModelView data container
    QStringList priorityList;
};

#endif // EVENTTABLEMODEL_H
