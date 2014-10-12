#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <cstdio>
#include <sstream>
#include <QFileDialog>
#include <QFile>

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

    if (eeprom_data.features & feature_bootsrc_sata) {
        ui->rootFsMMC->setChecked(0);
        ui->rootFsSATA->setChecked(1);
    }
    else {
        ui->rootFsMMC->setChecked(1);
        ui->rootFsSATA->setChecked(0);
    }

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
    fieldToCheckBox(ui->hdmiIgnoreSettingsCB, eeprom_data.hdmi.flags & ignore_settings);

    setLVDS0Enabled(eeprom_data.lvds1.flags & channel_present);
    setLVDS1Enabled(eeprom_data.lvds2.flags & channel_present);
    setLVDSDualLane(eeprom_data.lvds1.flags & dual_channel);
    setHdmiEnabled(eeprom_data.hdmi.flags & channel_present,
                   eeprom_data.hdmi.flags & ignore_settings);
}

void MainWindow::setLVDSDualLane(bool enabled)
{
    ui->lvds1Tab->setEnabled(!enabled);
    ui->lvds1ChannelPresentCB->setEnabled(!enabled);
    setLVDS1Enabled(ui->lvds1ChannelPresentCB->isChecked());
}


void MainWindow::setLVDS0Enabled(bool enabled)
{
    ui->lvds0ClockEdit->setEnabled(enabled);
    ui->lvds0WidthEdit->setEnabled(enabled);
    ui->lvds0HeightEdit->setEnabled(enabled);
    ui->lvds0HBackEdit->setEnabled(enabled);
    ui->lvds0HFrontEdit->setEnabled(enabled);
    ui->lvds0HSyncLenEdit->setEnabled(enabled);
    ui->lvds0VBackEdit->setEnabled(enabled);
    ui->lvds0VFrontEdit->setEnabled(enabled);
    ui->lvds0VSyncLenEdit->setEnabled(enabled);
    ui->lvds0VSyncPolarityCB->setEnabled(enabled);
    ui->lvds0HSyncPolarityCB->setEnabled(enabled);
    ui->lvds0DataMappingCB->setEnabled(enabled);
    ui->lvds0DataWidthCB->setEnabled(enabled);
    ui->lvdsDualLaneCB->setEnabled(enabled);

    if (ui->lvdsDualLaneCB->isChecked()) {
        if (enabled) {
            setLVDSDualLane(true);
        }
        else {
            setLVDSDualLane(false);
            setLVDS1Enabled(ui->lvds1ChannelPresentCB->isChecked());
        }
    }
}

void MainWindow::setLVDS1Enabled(bool enabled)
{
    ui->lvds1ClockEdit->setEnabled(enabled);
    ui->lvds1WidthEdit->setEnabled(enabled);
    ui->lvds1HeightEdit->setEnabled(enabled);
    ui->lvds1HBackEdit->setEnabled(enabled);
    ui->lvds1HFrontEdit->setEnabled(enabled);
    ui->lvds1HSyncLenEdit->setEnabled(enabled);
    ui->lvds1VBackEdit->setEnabled(enabled);
    ui->lvds1VFrontEdit->setEnabled(enabled);
    ui->lvds1VSyncLenEdit->setEnabled(enabled);
    ui->lvds1VSyncPolarityCB->setEnabled(enabled);
    ui->lvds1HSyncPolarityCB->setEnabled(enabled);
    ui->lvds1DataMappingCB->setEnabled(enabled);
    ui->lvds1DataWidthCB->setEnabled(enabled);
}

void MainWindow::setHdmiEnabled(bool enabled, bool ignore_settings)
{
    ui->hdmiIgnoreSettingsCB->setEnabled(enabled);
    if (enabled && ignore_settings)
            enabled = false;

    ui->hdmiClockEdit->setEnabled(enabled);
    ui->hdmiWidthEdit->setEnabled(enabled);
    ui->hdmiHeightEdit->setEnabled(enabled);
    ui->hdmiHBackEdit->setEnabled(enabled);
    ui->hdmiHFrontEdit->setEnabled(enabled);
    ui->hdmiHSyncLenEdit->setEnabled(enabled);
    ui->hdmiVBackEdit->setEnabled(enabled);
    ui->hdmiVFrontEdit->setEnabled(enabled);
    ui->hdmiVSyncLenEdit->setEnabled(enabled);
    ui->hdmiVSyncPolarityCB->setEnabled(enabled);
    ui->hdmiHSyncPolarityCB->setEnabled(enabled);
    ui->hdmiDataWidthCB->setEnabled(enabled);
}

bool MainWindow::guiToData(novena_eeprom_data& eeprom_data)
{
    eeprom_data.version = 2;
    memcpy(eeprom_data.signature, NOVENA_SIGNATURE, sizeof(eeprom_data.signature));

    if (!lineEditToNum(ui->serialNumberEdit, &eeprom_data.serial))
        return false;

    if (!lineEditToMac(ui->macAddressEdit, eeprom_data.mac))
        return false;

    eeprom_data.page_size = ui->eepromPageSizeEdit->text().toInt();
    eeprom_data.eeprom_size = ui->eepromSizeEdit->text().toInt();
    eeprom_data.eepromoops_offset = ui->eepromOopsOffsetEdit->text().toInt();
    eeprom_data.eepromoops_length = ui->eepromOopsSizeEdit->text().toInt();

    eeprom_data.features = 0;
    if (ui->es8328CB->checkState() == Qt::Checked)
        eeprom_data.features |= feature_es8328;
    if (ui->senokoCB->checkState() == Qt::Checked)
        eeprom_data.features |= feature_senoko;
    if (ui->pciExpressCB->checkState() == Qt::Checked)
        eeprom_data.features |= feature_pcie;
    if (ui->gigabitEthernetCB->checkState() == Qt::Checked)
        eeprom_data.features |= feature_gbit;
    if (ui->eepromOopsCB->checkState() == Qt::Checked)
        eeprom_data.features |= feature_eepromoops;
    if (ui->rootFsSATA->isChecked())
        eeprom_data.features |= feature_bootsrc_sata;

    eeprom_data.lvds1.flags = 0;
    eeprom_data.lvds2.flags = 0;
    eeprom_data.hdmi.flags = 0;

    eeprom_data.lvds1.frequency = ui->lvds0ClockEdit->text().toInt();
    eeprom_data.lvds1.hactive = ui->lvds0WidthEdit->text().toInt();
    eeprom_data.lvds1.vactive = ui->lvds0HeightEdit->text().toInt();
    eeprom_data.lvds1.hback_porch = ui->lvds0HBackEdit->text().toInt();
    eeprom_data.lvds1.hfront_porch = ui->lvds0HFrontEdit->text().toInt();
    eeprom_data.lvds1.hsync_len = ui->lvds0HSyncLenEdit->text().toInt();
    eeprom_data.lvds1.vback_porch = ui->lvds0VFrontEdit->text().toInt();
    eeprom_data.lvds1.vfront_porch = ui->lvds0VBackEdit->text().toInt();
    eeprom_data.lvds1.vsync_len = ui->lvds0VSyncLenEdit->text().toInt();
    if (ui->lvds0ChannelPresentCB->checkState() == Qt::Checked)
        eeprom_data.lvds1.flags |= channel_present;
    if (ui->lvds0VSyncPolarityCB->currentIndex())
        eeprom_data.lvds1.flags |= vsync_polarity;
    if (ui->lvds0HSyncPolarityCB->currentIndex())
        eeprom_data.lvds1.flags |= hsync_polarity;
    if (ui->lvds0DataMappingCB->currentIndex())
        eeprom_data.lvds1.flags |= mapping_jeida;
    if (ui->lvds0DataWidthCB->currentIndex())
        eeprom_data.lvds1.flags |= data_width_8bit;

    if (ui->lvdsDualLaneCB->checkState() == Qt::Checked) {
        eeprom_data.lvds1.flags |= dual_channel;
        eeprom_data.lvds2.flags |= channel_present;
        eeprom_data.features |= feature_retina;
    }

    eeprom_data.lvds2.frequency = ui->lvds1ClockEdit->text().toInt();
    eeprom_data.lvds2.hactive = ui->lvds1WidthEdit->text().toInt();
    eeprom_data.lvds2.vactive = ui->lvds1HeightEdit->text().toInt();
    eeprom_data.lvds2.hback_porch = ui->lvds1HBackEdit->text().toInt();
    eeprom_data.lvds2.hfront_porch = ui->lvds1HFrontEdit->text().toInt();
    eeprom_data.lvds2.hsync_len = ui->lvds1HSyncLenEdit->text().toInt();
    eeprom_data.lvds2.vback_porch = ui->lvds1VFrontEdit->text().toInt();
    eeprom_data.lvds2.vfront_porch = ui->lvds1VBackEdit->text().toInt();
    eeprom_data.lvds2.vsync_len = ui->lvds1VSyncLenEdit->text().toInt();
    if (ui->lvds1ChannelPresentCB->checkState() == Qt::Checked)
        eeprom_data.lvds2.flags |= channel_present;
    if (ui->lvds1VSyncPolarityCB->currentIndex())
        eeprom_data.lvds2.flags |= vsync_polarity;
    if (ui->lvds1HSyncPolarityCB->currentIndex())
        eeprom_data.lvds2.flags |= hsync_polarity;
    if (ui->lvds1DataMappingCB->currentIndex())
        eeprom_data.lvds2.flags |= mapping_jeida;
    if (ui->lvds1DataWidthCB->currentIndex())
        eeprom_data.lvds2.flags |= data_width_8bit;

    eeprom_data.hdmi.frequency = ui->hdmiClockEdit->text().toInt();
    eeprom_data.hdmi.hactive = ui->hdmiWidthEdit->text().toInt();
    eeprom_data.hdmi.vactive = ui->hdmiHeightEdit->text().toInt();
    eeprom_data.hdmi.hback_porch = ui->hdmiHBackEdit->text().toInt();
    eeprom_data.hdmi.hfront_porch = ui->hdmiHFrontEdit->text().toInt();
    eeprom_data.hdmi.hsync_len = ui->hdmiHSyncLenEdit->text().toInt();
    eeprom_data.hdmi.vback_porch = ui->hdmiVFrontEdit->text().toInt();
    eeprom_data.hdmi.vfront_porch = ui->hdmiVBackEdit->text().toInt();
    eeprom_data.hdmi.vsync_len = ui->hdmiVSyncLenEdit->text().toInt();
    if (ui->hdmiChannelPresentCB->checkState() == Qt::Checked) {
        eeprom_data.hdmi.flags |= channel_present;
        eeprom_data.features |= feature_hdmi;
    }
    if (ui->hdmiVSyncPolarityCB->currentIndex())
        eeprom_data.hdmi.flags |= vsync_polarity;
    if (ui->hdmiHSyncPolarityCB->currentIndex())
        eeprom_data.hdmi.flags |= hsync_polarity;
    if (ui->hdmiIgnoreSettingsCB->checkState() == Qt::Checked)
        eeprom_data.hdmi.flags |= ignore_settings;

    return true;
}

bool MainWindow::lineEditToNum(QLineEdit* edit, uint32_t* num)
{
    bool ok;
    *num = edit->text().toInt(&ok);
    return ok;
}

bool MainWindow::lineEditToMac(QLineEdit* edit, uint8_t* macp)
{
    std::string src_s = edit->text().toStdString();
    const char* str = src_s.c_str();

    int i;
    char *mac = (char*) macp;
    for (i=0; i<6; i++) {
        if (!isdigit(str[0]) &&
            (tolower(str[0]) < 'a' || tolower(str[0]) > 'f')) {
            std::ostringstream ostr;
            ostr << "Invalid character in MAC address: '" << str[0] << "'";
            return false;
        }
        if (!isdigit(str[1]) &&
            (tolower(str[1]) < 'a' || tolower(str[1]) > 'f')) {
            std::ostringstream ostr;
            ostr << "Invalid character in MAC address:'" << str[1] << "'";
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
        ostr << "Extra character in MAC address: '" << *str << "'";
        return false;
    }
    return true;
}

bool MainWindow::validateSerialNumber(const QString& serial_number, QString& err_str)
{
    bool ok;
    serial_number.toInt(&ok);

    if (!ok)
        err_str = "Serial number is not a valid number.";
    return ok;
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

void MainWindow::on_hdmiIgnoreSettingsCB_toggled(bool checked)
{
    setHdmiEnabled(ui->hdmiChannelPresentCB->isChecked(), checked);
}

void MainWindow::on_hdmiChannelPresentCB_toggled(bool checked)
{
    setHdmiEnabled(checked, ui->hdmiIgnoreSettingsCB->isChecked());
}

void MainWindow::on_lvds0ChannelPresentCB_toggled(bool checked)
{
    setLVDS0Enabled(checked);
}

void MainWindow::on_lvds1ChannelPresentCB_toggled(bool checked)
{
    setLVDS1Enabled(checked);
}

void MainWindow::on_actionSaveToEeprom_triggered(void)
{
    if (!guiToData(dev.data())) {
        showError("Invalid settings found");
        return;
    }
    if (dev.saveToDevice())
        showError("Error writing to EEPROM");
    ui->statusBar->showMessage("Written to EEPROM", 5 * 1000);
}

void MainWindow::on_actionExportToFile_triggered(void)
{
    novena_eeprom_data data;
    if (!guiToData(dev.data())) {
        showError("Invalid settings found");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export EEPROM"),
                           QString(),
                          tr("EEPROM File (*.novena-eeprom)"));
    // Not saving after all
    if (fileName == "")
        return;

    if (!fileName.endsWith(".novena-eeprom"))
        fileName += ".novena-eeprom";

    QFile output(fileName);
    if (!output.open(QIODevice::WriteOnly)) {
        QMessageBox msg_box;
        msg_box.setText("Unable to save file");
        msg_box.exec();
    }

    output.write((const char *)&data, sizeof(data));
    output.close();
    ui->statusBar->showMessage("Exported to " + fileName, 5 * 1000);
}

void MainWindow::on_actionImportFromFile_triggered(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import EEPROM"),
                                                    QString(),
                          tr("EEPROM File (*.novena-eeprom)"));
    if (fileName == "")
        return;

    novena_eeprom_data data;

    QFile input(fileName);
    if (!input.open(QIODevice::ReadOnly)) {
        QMessageBox msg_box;
        msg_box.setText("Unable to load file");
        msg_box.exec();
    }

    input.read((char *)&data, sizeof(data));
    input.close();

    dataToGui(data);
    ui->statusBar->showMessage("Imported from " + fileName, 5 * 1000);
}
