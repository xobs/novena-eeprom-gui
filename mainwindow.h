#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "EepromDev.h"
#include <QLineEdit>
#include <QCheckBox>
#include <sstream>
#include <QComboBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool validateSerialNumber(const QString& serial_number, QString& err_str);

private slots:
    void on_serialNumberEdit_editingFinished();
    void on_lvdsDualLaneCB_toggled(bool checked);
    void on_actionSaveToEeprom_triggered(void);
    void on_actionExportToFile_triggered(void);
    void on_actionImportFromFile_triggered(void);
    void on_hdmiIgnoreSettingsCB_toggled(bool checked);
    void on_hdmiChannelPresentCB_toggled(bool checked);
    void on_lvds0ChannelPresentCB_toggled(bool checked);
    void on_lvds1ChannelPresentCB_toggled(bool checked);
    void setHdmiEnabled(bool enabled, bool ignore_settings);
    void setLVDS0Enabled(bool checked);
    void setLVDS1Enabled(bool checked);

private:
    template <typename T>
    void numToLineEdit(QLineEdit* edit, const T& n)
    {
        std::ostringstream oss;
        oss << n;
        edit->setText(oss.str().c_str());
    }

    void showError(const std::string& err_s);

    bool lineEditToMac(QLineEdit* edit, uint8_t* macp);
    bool lineEditToNum(QLineEdit* edit, uint32_t* num);
    void fieldToCheckBox(QCheckBox* cb, const uint32_t& fld);
    void fieldToComboBox(QComboBox* cb, const uint32_t& fld);
    void setLVDSDualLane(bool enabled);

    void dataToGui(novena_eeprom_data& eeprom_data);
    bool guiToData(novena_eeprom_data& eeprom_data);
    Ui::MainWindow *ui;
    EepromDev dev;
};

#endif // MAINWINDOW_H
