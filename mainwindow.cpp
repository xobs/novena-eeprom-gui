#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <cstdio>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (!dev.loadFromDevice())
    {
        showError(dev.error());
    }
    dataToGui(dev.data());
}

void MainWindow::showError(const std::string& err_s)
{
    QMessageBox msg_box;
    msg_box.setText(err_s.c_str());
    msg_box.exec();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fieldToCheckBox(QCheckBox* cb, const uint32_t& fld)
{
    cb->setChecked(fld != 0);
}

void MainWindow::fieldToComboBox(QComboBox* cb, const uint32_t& fld)
{
    cb->setCurrentIndex(fld == 0 ? 0 : 1);
}

void MainWindow::dataToGui(novena_eeprom_data& eeprom_data)
{
    numToLineEdit(ui->serialNumberEdit, eeprom_data.serial);

    char macbuf[20];
    snprintf(macbuf, sizeof(macbuf)-1, "%02x:%02x:%02x:%02x:%02x:%02x"
             , eeprom_data.mac[0], eeprom_data.mac[1], eeprom_data.mac[2]
            , eeprom_data.mac[3], eeprom_data.mac[4], eeprom_data.mac[5]);
    ui->macAddressEdit->setText(macbuf);

    numToLineEdit(ui->eepromPageSizeEdit, (int) eeprom_data.page_size);
    numToLineEdit(ui->eepromSizeEdit, eeprom_data.eeprom_size);
    numToLineEdit(ui->eepromOopsOffsetEdit, eeprom_data.eepromoops_offset);
    numToLineEdit(ui->eepromOopsSizeEdit, eeprom_data.eepromoops_length);

    fieldToCheckBox(ui->es8328CB, eeprom_data.features & EepromDev::ES8328_F);
    fieldToCheckBox(ui->senokoCB, eeprom_data.features & EepromDev::Senoko_F);
    fieldToCheckBox(ui->pciExpressCB, eeprom_data.features & EepromDev::Pcie_F);
    fieldToCheckBox(ui->gigabitEthernetCB, eeprom_data.features & EepromDev::Gbit_F);
    fieldToCheckBox(ui->eepromOopsCB, eeprom_data.features & EepromDev::EepromOops_F);

    numToLineEdit(ui->lvds0ClockEdit, eeprom_data.lvds1.frequency);
    numToLineEdit(ui->lvds0WidthEdit, eeprom_data.lvds1.hactive);
    numToLineEdit(ui->lvds0HeightEdit, eeprom_data.lvds1.vactive);
    numToLineEdit(ui->lvds0HBackEdit, eeprom_data.lvds1.hback_porch);
    numToLineEdit(ui->lvds0HFrontEdit, eeprom_data.lvds1.hfront_porch);
    numToLineEdit(ui->lvds0HSyncLenEdit, eeprom_data.lvds1.hsync_len);
    numToLineEdit(ui->lvds0VBackEdit, eeprom_data.lvds1.vback_porch);
    numToLineEdit(ui->lvds0VFrontEdit, eeprom_data.lvds1.vfront_porch);
    numToLineEdit(ui->lvds0VSyncLenEdit, eeprom_data.lvds1.vsync_len);
    fieldToCheckBox(ui->lvds0ChannelPresentCB, eeprom_data.lvds1.flags & channel_present);
    fieldToComboBox(ui->lvds0VSyncPolarityCB, eeprom_data.lvds1.flags & vsync_polarity);
    fieldToComboBox(ui->lvds0HSyncPolarityCB, eeprom_data.lvds1.flags & hsync_polarity);
    fieldToComboBox(ui->lvds0DataMappingCB, eeprom_data.lvds1.flags & mapping_jeida);
    fieldToComboBox(ui->lvds0DataWidthCB, eeprom_data.lvds1.flags & data_width_8bit);

    fieldToCheckBox(ui->lvdsDualLaneCB, eeprom_data.lvds1.flags & dual_channel);
    setLVDSDualLane(eeprom_data.lvds1.flags & dual_channel);

    numToLineEdit(ui->lvds1ClockEdit, eeprom_data.lvds2.frequency);
    numToLineEdit(ui->lvds1WidthEdit, eeprom_data.lvds2.hactive);
    numToLineEdit(ui->lvds1HeightEdit, eeprom_data.lvds2.vactive);
    numToLineEdit(ui->lvds1HBackEdit, eeprom_data.lvds2.hback_porch);
    numToLineEdit(ui->lvds1HFrontEdit, eeprom_data.lvds2.hfront_porch);
    numToLineEdit(ui->lvds1HSyncLenEdit, eeprom_data.lvds2.hsync_len);
    numToLineEdit(ui->lvds1VBackEdit, eeprom_data.lvds2.vback_porch);
    numToLineEdit(ui->lvds1VFrontEdit, eeprom_data.lvds2.vfront_porch);
    numToLineEdit(ui->lvds1VSyncLenEdit, eeprom_data.lvds2.vsync_len);
    fieldToCheckBox(ui->lvds1ChannelPresentCB, eeprom_data.lvds2.flags & channel_present);
    fieldToComboBox(ui->lvds1VSyncPolarityCB, eeprom_data.lvds2.flags & vsync_polarity);
    fieldToComboBox(ui->lvds1HSyncPolarityCB, eeprom_data.lvds2.flags & hsync_polarity);
    fieldToComboBox(ui->lvds1DataMappingCB, eeprom_data.lvds2.flags & mapping_jeida);
    fieldToComboBox(ui->lvds1DataWidthCB, eeprom_data.lvds2.flags & data_width_8bit);

    numToLineEdit(ui->hdmiClockEdit, eeprom_data.hdmi.frequency);
    numToLineEdit(ui->hdmiWidthEdit, eeprom_data.hdmi.hactive);
    numToLineEdit(ui->hdmiHeightEdit, eeprom_data.hdmi.vactive);
    numToLineEdit(ui->hdmiHBackEdit, eeprom_data.hdmi.hback_porch);
    numToLineEdit(ui->hdmiHFrontEdit, eeprom_data.hdmi.hfront_porch);
    numToLineEdit(ui->hdmiHSyncLenEdit, eeprom_data.hdmi.hsync_len);
    numToLineEdit(ui->hdmiVBackEdit, eeprom_data.hdmi.vback_porch);
    numToLineEdit(ui->hdmiVFrontEdit, eeprom_data.hdmi.vfront_porch);
    numToLineEdit(ui->hdmiVSyncLenEdit, eeprom_data.hdmi.vsync_len);
    fieldToCheckBox(ui->hdmiChannelPresentCB, eeprom_data.hdmi.flags & channel_present);
    fieldToComboBox(ui->hdmiVSyncPolarityCB, eeprom_data.hdmi.flags & vsync_polarity);
    fieldToComboBox(ui->hdmiHSyncPolarityCB, eeprom_data.hdmi.flags & hsync_polarity);
    fieldToComboBox(ui->hdmiDataMappingCB, eeprom_data.hdmi.flags & mapping_jeida);
    fieldToComboBox(ui->hdmiDataWidthCB, eeprom_data.hdmi.flags & data_width_8bit);
}

void MainWindow::setLVDSDualLane(bool enabled)
{
    ui->lvds1Tab->setEnabled(!enabled);
    ui->lvds1ClockEdit->setEnabled(!enabled);
    ui->lvds1WidthEdit->setEnabled(!enabled);
    ui->lvds1HeightEdit->setEnabled(!enabled);
    ui->lvds1HBackEdit->setEnabled(!enabled);
    ui->lvds1HFrontEdit->setEnabled(!enabled);
    ui->lvds1HSyncLenEdit->setEnabled(!enabled);
    ui->lvds1VBackEdit->setEnabled(!enabled);
    ui->lvds1VFrontEdit->setEnabled(!enabled);
    ui->lvds1VSyncLenEdit->setEnabled(!enabled);
    ui->lvds1ChannelPresentCB->setEnabled(!enabled);
    ui->lvds1VSyncPolarityCB->setEnabled(!enabled);
    ui->lvds1HSyncPolarityCB->setEnabled(!enabled);
    ui->lvds1DataMappingCB->setEnabled(!enabled);
    ui->lvds1DataWidthCB->setEnabled(!enabled);
}

bool MainWindow::guiToData(novena_eeprom_data& eeprom_data)
{
    if (!lineEditToNum(ui->serialNumberEdit, "Serial Number", &eeprom_data.serial)) return false;

    if (!lineEditToMac(ui->macAddressEdit, "MAC Address", eeprom_data.mac)) return false;

    /*numToLineEdit(ui->eepromPageSizeEdit, (int) eeprom_data.page_size);
    numToLineEdit(ui->eepromSizeEdit, eeprom_data.eeprom_size);
    numToLineEdit(ui->eepromOopsOffsetEdit, eeprom_data.eepromoops_offset);
    numToLineEdit(ui->eepromOopsSizeEdit, eeprom_data.eepromoops_length);

    fieldToCheckBox(ui->es8328CB, eeprom_data.features & EepromDev::ES8328_F);
    fieldToCheckBox(ui->senokoCB, eeprom_data.features & EepromDev::Senoko_F);
    fieldToCheckBox(ui->pciExpressCB, eeprom_data.features & EepromDev::Pcie_F);
    fieldToCheckBox(ui->gigabitEthernetCB, eeprom_data.features & EepromDev::Gbit_F);
    fieldToCheckBox(ui->eepromOopsCB, eeprom_data.features & EepromDev::EepromOops_F);

    numToLineEdit(ui->lvds0ClockEdit, eeprom_data.lvds1.frequency);
    numToLineEdit(ui->lvds0WidthEdit, eeprom_data.lvds1.hactive);
    numToLineEdit(ui->lvds0HeightEdit, eeprom_data.lvds1.vactive);
    numToLineEdit(ui->lvds0HBackEdit, eeprom_data.lvds1.hback_porch);
    numToLineEdit(ui->lvds0HFrontEdit, eeprom_data.lvds1.hfront_porch);
    numToLineEdit(ui->lvds0HSyncLenEdit, eeprom_data.lvds1.hsync_len);
    numToLineEdit(ui->lvds0VBackEdit, eeprom_data.lvds1.vback_porch);
    numToLineEdit(ui->lvds0VFrontEdit, eeprom_data.lvds1.vfront_porch);
    numToLineEdit(ui->lvds0VSyncLenEdit, eeprom_data.lvds1.vsync_len);
    fieldToCheckBox(ui->lvds0ChannelPresentCB, eeprom_data.lvds1.flags & channel_present);
    fieldToComboBox(ui->lvds0VSyncPolarityCB, eeprom_data.lvds1.flags & vsync_polarity);
    fieldToComboBox(ui->lvds0HSyncPolarityCB, eeprom_data.lvds1.flags & hsync_polarity);
    fieldToComboBox(ui->lvds0DataMappingCB, eeprom_data.lvds1.flags & mapping_jeida);
    fieldToComboBox(ui->lvds0DataWidthCB, eeprom_data.lvds1.flags & data_width_8bit);

    fieldToCheckBox(ui->lvdsDualLaneCB, eeprom_data.lvds1.flags & dual_channel);
    setLVDSDualLane(eeprom_data.lvds1.flags & dual_channel);

    numToLineEdit(ui->lvds1ClockEdit, eeprom_data.lvds2.frequency);
    numToLineEdit(ui->lvds1WidthEdit, eeprom_data.lvds2.hactive);
    numToLineEdit(ui->lvds1HeightEdit, eeprom_data.lvds2.vactive);
    numToLineEdit(ui->lvds1HBackEdit, eeprom_data.lvds2.hback_porch);
    numToLineEdit(ui->lvds1HFrontEdit, eeprom_data.lvds2.hfront_porch);
    numToLineEdit(ui->lvds1HSyncLenEdit, eeprom_data.lvds2.hsync_len);
    numToLineEdit(ui->lvds1VBackEdit, eeprom_data.lvds2.vback_porch);
    numToLineEdit(ui->lvds1VFrontEdit, eeprom_data.lvds2.vfront_porch);
    numToLineEdit(ui->lvds1VSyncLenEdit, eeprom_data.lvds2.vsync_len);
    fieldToCheckBox(ui->lvds1ChannelPresentCB, eeprom_data.lvds2.flags & channel_present);
    fieldToComboBox(ui->lvds1VSyncPolarityCB, eeprom_data.lvds2.flags & vsync_polarity);
    fieldToComboBox(ui->lvds1HSyncPolarityCB, eeprom_data.lvds2.flags & hsync_polarity);
    fieldToComboBox(ui->lvds1DataMappingCB, eeprom_data.lvds2.flags & mapping_jeida);
    fieldToComboBox(ui->lvds1DataWidthCB, eeprom_data.lvds2.flags & data_width_8bit);

    numToLineEdit(ui->hdmiClockEdit, eeprom_data.hdmi.frequency);
    numToLineEdit(ui->hdmiWidthEdit, eeprom_data.hdmi.hactive);
    numToLineEdit(ui->hdmiHeightEdit, eeprom_data.hdmi.vactive);
    numToLineEdit(ui->hdmiHBackEdit, eeprom_data.hdmi.hback_porch);
    numToLineEdit(ui->hdmiHFrontEdit, eeprom_data.hdmi.hfront_porch);
    numToLineEdit(ui->hdmiHSyncLenEdit, eeprom_data.hdmi.hsync_len);
    numToLineEdit(ui->hdmiVBackEdit, eeprom_data.hdmi.vback_porch);
    numToLineEdit(ui->hdmiVFrontEdit, eeprom_data.hdmi.vfront_porch);
    numToLineEdit(ui->hdmiVSyncLenEdit, eeprom_data.hdmi.vsync_len);
    fieldToCheckBox(ui->hdmiChannelPresentCB, eeprom_data.hdmi.flags & channel_present);
    fieldToComboBox(ui->hdmiVSyncPolarityCB, eeprom_data.hdmi.flags & vsync_polarity);
    fieldToComboBox(ui->hdmiHSyncPolarityCB, eeprom_data.hdmi.flags & hsync_polarity);
    fieldToComboBox(ui->hdmiDataMappingCB, eeprom_data.hdmi.flags & mapping_jeida);
    fieldToComboBox(ui->hdmiDataWidthCB, eeprom_data.hdmi.flags & data_width_8bit);*/
    return true;
}

bool MainWindow::lineEditToNum(QLineEdit* edit, const char* field_name, uint32_t* num)
{
    std::string src_s = edit->text().toStdString();
    const char* str = src_s.c_str();
    char* str_e;

    unsigned long ul = strtoul(str, &str_e, 10);
    if (str_e > str)
    {
        *num = (uint32_t) ul;
        return true;
    }
    else
    {
        std::ostringstream oss;
        oss << "Error parsing " << field_name << " should be integer.";
        showError(oss.str());
        return false;
    }
}

bool MainWindow::lineEditToMac(QLineEdit* edit, const char* field_name, uint8_t* macp)
{
    std::string src_s = edit->text().toStdString();
    const char* str = src_s.c_str();

    int i;
    char *mac = (char*) macp;
    for (i=0; i<6; i++) {
        if (!isdigit(str[0]) &&
            (tolower(str[0]) < 'a' || tolower(str[0]) > 'f')) {
            std::ostringstream ostr;
            ostr << "Invalid character in " << field_name << "'" << str[0] << "'";
            return false;
        }
        if (!isdigit(str[1]) &&
            (tolower(str[1]) < 'a' || tolower(str[1]) > 'f')) {
            std::ostringstream ostr;
            ostr << "Invalid character in " << field_name << "'" << str[1] << "'";
            return false;
        }

        *mac = (uint8_t) strtoul(str, NULL, 16);
        mac++;
        str+=2;
        if (*str == '-' || *str == ':' || *str == '.')
            str++;
    }
    if (*str) {
        std::ostringstream ostr;
        ostr << "Extra character in " << field_name << "'" << *str << "'";
        return false;
    }
    return 0;
}

bool MainWindow::validateSerialNumber(const QString& serial_number, QString& err_str)
{
    for (int i = 0; i < serial_number.size(); ++i)
    {
        QChar c = serial_number.at(i);
        if (!(c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9'))
        {
            err_str = "In Serial Number only hexadecimal digits should be used.";
            return false;
        }
    }
    return true;
}

void MainWindow::on_serialNumberEdit_editingFinished()
{
    QString serial_number = ui->serialNumberEdit->text();
    QString err_str;
    if (!validateSerialNumber(serial_number, err_str))
    {
        QMessageBox msg_box;
        msg_box.setText(err_str);
        msg_box.exec();
    }
}

void MainWindow::on_lvdsDualLaneCB_toggled(bool checked)
{
    setLVDSDualLane(checked);
}
