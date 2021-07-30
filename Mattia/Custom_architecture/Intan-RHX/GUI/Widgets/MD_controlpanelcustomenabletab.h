#ifndef MD_CONTROLPANELCUSTOMENABLETAB_H
#define MD_CONTROLPANELCUSTOMENABLETAB_H


#include <QtWidgets>
#include "controllerinterface.h"
#include "systemstate.h"
#include "rhxcontroller.h"

class ControlPanelCustomArchitectureTab : public QWidget
{
    Q_OBJECT
public:
    explicit ControlPanelCustomArchitectureTab(ControllerInterface* controllerInterface_, SystemState* state_, QWidget *parent = nullptr);
    void customArchEnable();

private slots:
    void enableCustomArch();
    //void setTriggerSource(int index);
    //void setTriggerPolarity(int index);
    //void setTriggerPosition(int index);


private:
    SystemState* state;
    ControllerInterface* controllerInterface;

    QCheckBox *customEnableCheckBox;

    RHXController *evalBoard;

    //QComboBox *customSourceComboBox;
    //QComboBox *customPolarityComboBox;
    //QComboBox *customPositionComboBox;
};


#endif // MD_CONTROLPANELCUSTOMENABLETAB_H


