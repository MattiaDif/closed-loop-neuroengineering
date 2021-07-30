//------------------------------------------------------------------------------
//
//  MD changes to design a custom tab to enable the custom
//  architecture
//
//------------------------------------------------------------------------------

#include "MD_controlpanelcustomenabletab.h"

ControlPanelCustomArchitectureTab::ControlPanelCustomArchitectureTab(ControllerInterface* controllerInterface_, SystemState* state_,
                                               QWidget *parent) :
    QWidget(parent),
    state(state_),
    controllerInterface(controllerInterface_),
    customEnableCheckBox(nullptr)

{
    customEnableCheckBox = new QCheckBox(tr("Enable"), this);
    connect(customEnableCheckBox, SIGNAL(clicked(bool)), this, SLOT(enableCustomArch()));

    QHBoxLayout *customLayout = new QHBoxLayout;
    customLayout->addWidget(customEnableCheckBox);
    customLayout->addStretch(1);

    //QVBoxLayout *customLayout = new QVBoxLayout;
    //customLayout->addLayout(customLayout1);

    QGroupBox *customGroupBox = new QGroupBox(tr("Custom Architecture"), this);
    customGroupBox->setLayout(customLayout);
}

void ControlPanelCustomArchitectureTab::enableCustomArch()
{
    evalBoard->customArchEnable(customEnableCheckBox->isChecked());
    //cout << "  enable(1)/disable(0) cst en " << enable << endl;
    cout << ""
}
