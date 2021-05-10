#include "LayoutIterationDialog.h"
#include "ui_LayoutIterationDialog.h"

#include <QEventLoop>
#include <QTimer>
#include <QObject>

#include "../Gui/mainscene.h"

LayoutIterationDialog::LayoutIterationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LayoutIterationDialog)
{
    ui->setupUi(this);
    ui->layoutSpinBox->setValue(3);
    setWindowTitle("Set layout iterations");
    setFocus();
    raise();
    setEnabled(true);
}

LayoutIterationDialog::~LayoutIterationDialog()
{
    delete ui;
}

int LayoutIterationDialog::GetLayoutIterations() {
    ((MainScene *)parent())->SetLayoutIteratrions(ui->layoutSpinBox->value());
    return ui->layoutSpinBox->value();
}
