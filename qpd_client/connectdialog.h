#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

namespace Ui {
class ConnectDialog;
}

/// Class ConnectDialog - connection dialog
class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    /// Constructor
    explicit ConnectDialog(QWidget *parent = nullptr);

    /// Destructor
    ~ConnectDialog();

    /// Returns selected address to server
    const QString address();

    /// Returns selected TCP port
    const QString port();

    /// Returns selected user name
    const QString user_name();

    /// Returns selected user password
    const QString password();

    /// Loads the configuration from a file.
    void load_config();

    /// Saves the configuration to a file.
    void save_config();

private:
    Ui::ConnectDialog *ui;  ///< Qt interface
};

#endif // CONNECTDIALOG_H
