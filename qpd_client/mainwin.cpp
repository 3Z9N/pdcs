#include "mainwin.h"
#include "ui_mainwin.h"
#include <QMessageBox>


// Constructor
MainWin::MainWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWin)
{
    ui->setupUi(this);
    ui->StatusLabel->setText("Disonnected");
    ConnDialog = new ConnectDialog(this);
    EvtDialog = new EventDialog(this);

    // set TableView
    ui->TableView->setModel(&model);
    ui->TableView->setColumnWidth(0,160);
    ui->TableView->setColumnWidth(1,100);
    ui->TableView->setColumnWidth(2,240);

    // connect signals with slots
    connect(ui->BConnect,   SIGNAL(clicked()), this, SLOT(BConnectClick()));
    connect(ui->BNew,       SIGNAL(clicked()), this, SLOT(BNewClick()));
    connect(ui->BAll,       SIGNAL(clicked()), this, SLOT(BAllClick()));
    connect(ui->BLast,      SIGNAL(clicked()), this, SLOT(BLastClick()));
    connect(ui->BExit,      SIGNAL(clicked()), this, SLOT(BExitClick()));
    connect(ui->BCloseInfo, SIGNAL(clicked()), ui->EventInfo, SLOT(hide()));

    ui->EventInfo->hide();
}

// Destructor
MainWin::~MainWin()
{
    delete ConnDialog;
    delete EvtDialog;
    delete ui;
    pdcli.close();
}

// Server message event handler
void MainWin::onMessage(std::string msg, std::vector<std::string> args)
{
    last_msg = msg;
    last_args = args;
    DBG("msg = %s, args.size = %ld", msg.c_str(), args.size());

    if(msg == "NEW") {
        model.append(args);
        ui->EventInfo->show();
    }
    else if(msg == "ALL") {
        model.append(args);
    }
    else if(msg == "LAST") {
        model.clear();
        model.append(args);
    }
}

// BConnect click slot
void MainWin::BConnectClick()
{
    // disconnect if client connected and cleanup pdcli
    if(pdcli.connected()) {
        pdcli.close();
        ui->BConnect->setText("Connect");
        ui->StatusLabel->setText("Disonnected");
        ui->BNew->setEnabled(false);
        ui->BAll->setEnabled(false);
        ui->BLast->setEnabled(false);
        model.clear();
        return;
    }

    // execute connection dialog
    int res = ConnDialog->exec();
    if(res == QDialog::Rejected) return; // dialog rejected

    // create connection
    pdcli.set_on_message([this](const std::string& msg, const std::vector<std::string>& args)
            {
                onMessage(msg, args);
            } );
    pdcli.connect(ConnDialog->address().toStdString(), ConnDialog->port().toStdString());
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // wait for client running

    if(!pdcli.connected()) { // not connected - close pdcli
        DBG("client not connected - close");
        pdcli.close();
        QMessageBox::warning(this, "Connection", "Can't connect to " +
                             ConnDialog->address() + ":" + ConnDialog->port() +
                             "\n\nThe wrong address or port or server is turned off.\n Check and try again.",
                             QMessageBox::Ok);
        ui->StatusLabel->setText("Disconnected");
        return;
    }
    DBG("client connected...");

    // login to server
    pdcli.login(ConnDialog->user_name().toStdString(), ConnDialog->password().toStdString());
    // wait for the server response
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::string s = last_args.size() ? last_args[0] : "";
    if(!pdcli.connected() || s != "ACCEPTED") {   // server close connection - login rejected
        DBG("connection rejected");
        pdcli.close();
        ui->StatusLabel->setText(s.c_str());
        QMessageBox::warning(this, "Connection",
                             "Login was rejected. Check your username and password and try again.",
                             QMessageBox::Ok);
        return;
    }
    DBG("login accepted");
    ui->BConnect->setText("Disconnect");
    ui->StatusLabel->setText("Connected to: "+ConnDialog->address()+":"+ConnDialog->port());
    ui->BNew->setEnabled(true);
    ui->BAll->setEnabled(true);
    ui->BLast->setEnabled(true);
}

// BNew click slot
void MainWin::BNewClick()
{
    // execute event dialog
    int res = EvtDialog->exec();
    if(res == QDialog::Rejected) return; // dialog rejected
    if(EvtDialog->info().isEmpty()) {
        QMessageBox::warning(this, "New",
                             "The information is empty!\nYou must write something.",
                             QMessageBox::Ok);
        return;
    }
    pdcli.add_new_event(EvtDialog->priority(), EvtDialog->info().toStdString());
}

// BAll click slot
void MainWin::BAllClick()
{
    model.clear();
    pdcli.get_all_events();
}

// BLast click slot
void MainWin::BLastClick()
{
    model.clear();
    pdcli.get_last_event();
}

// BExit click slot
void MainWin::BExitClick()
{
    this->close();
}
