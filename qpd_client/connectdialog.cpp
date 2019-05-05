#include "connectdialog.h"
#include "ui_connectdialog.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include "pd_tools.h"

// Constructor
ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    setFixedSize(400, 180);

    ui->EAddress->setText("localhost");
    ui->EPort->setText("11111");
    ui->EName->setText("");
    ui->EPassword->setText("");

    load_config();
}

// Destructor
ConnectDialog::~ConnectDialog()
{
    save_config();
    delete ui;
}

// Returns selected address to server
const QString ConnectDialog::address()
{
    return ui->EAddress->text();
}

// Returns selected TCP port
const QString ConnectDialog::port()
{
    return ui->EPort->text();
}

// Returns selected user name
const QString ConnectDialog::user_name()
{
    return ui->EName->text();
}

// Returns selected user password
const QString ConnectDialog::password()
{
    return ui->EPassword->text();
}

// Loads the configuration from a file.
void ConnectDialog::load_config()
{
    // read saved config file
    DBG("Read config file...");
    const char* home = getenv ("HOME");
    if(home == nullptr) return;
    std::string fpath = home + std::string("/.qpd_client");
    if (!boost::filesystem::exists(fpath.c_str())) return;
    std::ifstream file(fpath.c_str());
    if (!file.is_open()) return;
    std::string line;
    while (getline(file, line)) {
        std::vector<std::string> v = split_string(line, "="); // key=value
        if (v.size() != 2)
            continue;
        if(v[0] == "address") {
            ui->EAddress->setText(v[1].c_str());
        }
        else if(v[0] == "port") {
            ui->EPort->setText(v[1].c_str());
        }
        else if(v[0] == "name") {
            ui->EName->setText(v[1].c_str());
        }
    }
    file.close();
}

// Saves the configuration to a file.
void ConnectDialog::save_config()
{
    DBG("Save config file...");
    const char* home = getenv ("HOME");
    if(home == nullptr) return;
    std::string fpath = home + std::string("/.qpd_client");
    std::ofstream file(fpath.c_str());
    if (!file.is_open()) return;
    file << "address=" << ui->EAddress->text().toStdString() << std::endl;
    file << "port=" << ui->EPort->text().toStdString() << std::endl;
    file << "name=" << ui->EName->text().toStdString() << std::endl;
    file.close();
}
