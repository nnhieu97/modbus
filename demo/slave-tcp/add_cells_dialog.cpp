#include "add_cells_dialog.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QSpinBox>

#include <QHBoxLayout>
#include <QVBoxLayout>

AddCellsDialog::AddCellsDialog(QWidget *parent)
    : QDialog(parent)
{
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}
