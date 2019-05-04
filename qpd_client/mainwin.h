#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <QStandardItemModel>
#include <pd_client.h>
#include "connectdialog.h"
#include "eventtablemodel.h"
#include "eventdialog.h"

namespace Ui {
class MainWin;
}

/// Class MainWin - main program window
class MainWin : public QWidget
{
    Q_OBJECT

public:
    /// Constructor
    explicit MainWin(QWidget *parent = nullptr);

    /// Destructor
    ~MainWin();

private:
    /// Server message event handler
    void onMessage(std::string msg, std::vector<std::string> args);

private:
    Ui::MainWin *ui;                    ///< Qt interface
    EventTableModel model;              ///< EventTable model view
    ConnectDialog *ConnDialog;          ///< Connection dialog
    EventDialog *EvtDialog;             ///< New event dialog

    pd_client pdcli;                    ///< pd_client object
    std::string last_msg;               ///< To seve last received message
    std::vector<std::string> last_args; ///< To seve last received args

private slots:
    void BConnectClick();               ///< BConnect click slot
    void BNewClick();                   ///< BNew click slot
    void BAllClick();                   ///< BAll click slot
    void BLastClick();                  ///< BLast click slot
    void BExitClick();                  ///< BExit click slot
};

#endif // MAINWIN_H
