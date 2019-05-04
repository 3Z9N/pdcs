#include "eventdialog.h"
#include "ui_eventdialog.h"

// Constructor
EventDialog::EventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventDialog)
{
    ui->setupUi(this);
    setFixedSize(356, 145);

    /// CBPriority setup
    ui->CBPriority->addItem("Lowermost");
    ui->CBPriority->addItem("Low");
    ui->CBPriority->addItem("Medium");
    ui->CBPriority->addItem("High");
    ui->CBPriority->addItem("Highest");
    ui->CBPriority->setCurrentIndex(0);
}

// Destructor
EventDialog::~EventDialog()
{
    delete ui;
}

// Return selected event priority
int EventDialog::priority()
{
    return ui->CBPriority->currentIndex();
}

// Return selected event info
const QString EventDialog::info()
{
    return ui->EInfo->text();
}
