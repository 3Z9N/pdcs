#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QDialog>

namespace Ui {
class EventDialog;
}

/// Class EventDialog - new evend dialog
class EventDialog : public QDialog
{
    Q_OBJECT

public:
    /// Constructor
    explicit EventDialog(QWidget *parent = nullptr);

    /// Destructor
    ~EventDialog();

    /// Return selected event priority
    int priority();

    /// Return selected event info
    const QString info();

private:
    Ui::EventDialog *ui;    ///< Qt interface
};

#endif // EVENTDIALOG_H
