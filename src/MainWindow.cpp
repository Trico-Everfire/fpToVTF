#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <VTFLib.h>
#include "MainWindow.h"


MainWindow::MainWindow() : QDialog()
{

    setWindowTitle("floats to hdr");

    resize(300, 400);

    auto pMainWindowLayout = new QGridLayout(this);

    m_pFileButton = new QPushButton("Browse...",this);
    pMainWindowLayout->addWidget(m_pFileButton, 0, 0, 1, 4);

    m_pFileEdit = new QTextEdit(this);
    pMainWindowLayout->addWidget(m_pFileEdit, 1, 0, 3, 0);

    auto pWidthLabel = new QLabel("Width:",this);
    pMainWindowLayout->addWidget(pWidthLabel, 4, 0);

    m_pImageWidthSpinbox = new QSpinBox(this);
    m_pImageWidthSpinbox->setMinimum(0);
    m_pImageWidthSpinbox->setMaximum(99990);
    pMainWindowLayout->addWidget(m_pImageWidthSpinbox, 4, 1);

    auto pHeightLabel = new QLabel("Height:",this);
    pMainWindowLayout->addWidget(pHeightLabel, 4, 2);

    m_pImageHeightSpinbox = new QSpinBox(this);
    m_pImageHeightSpinbox->setMinimum(0);
    m_pImageHeightSpinbox->setMaximum(99990);
    pMainWindowLayout->addWidget(m_pImageHeightSpinbox, 4, 3);

    auto pRotationLabel = new QLabel("Rotate by:", this);
    pMainWindowLayout->addWidget(pRotationLabel, 5, 0, 1, 2);

    m_pImageRotateSpinbox = new QComboBox(this);
    m_pImageRotateSpinbox->addItem("0°deg", 0);
    m_pImageRotateSpinbox->addItem("90°deg", 1);
    m_pImageRotateSpinbox->addItem("180°deg", 2);
    m_pImageRotateSpinbox->addItem("270°deg", 3);

    pMainWindowLayout->addWidget(m_pImageRotateSpinbox, 5, 2, 1, 2);

    m_pImageTypeSpinbox = new QComboBox(this);
    m_pImageTypeSpinbox->addItem("RGBAFP32",true);
    m_pImageTypeSpinbox->addItem("RFP32",false);

    pMainWindowLayout->addWidget(m_pImageTypeSpinbox, 6, 0, 1, 4);

    m_pSaveFileButton = new QPushButton("Save as...",this);
    pMainWindowLayout->addWidget(m_pSaveFileButton, 7, 0, 1, 4);

    connect(m_pFileButton, &QPushButton::pressed, this, [&]{
        auto filePath = QFileDialog::getOpenFileName(this, "Open Float File","./","Float file (*.txt)" );

        if(filePath.isEmpty())
            return;

        QFile file(filePath);
        file.open(QFile::ReadOnly);

        QString data;
        data.append(file.readAll());
        data.replace(",", ".");
        auto list = data.split('\n');
        for(int i = 0; i < list.count() ; i++)
        {
            if(list[i].isEmpty())
                continue;
           bool isOk;
            list[i].toFloat(&isOk);
            if(!isOk) {
                QMessageBox::critical(this, "Invalid Float File", "The file you're trying to process does not contain only floats.");
                return;
            }
        }

        m_pFileEdit->setText(data);

    });

    connect(m_pSaveFileButton, &QPushButton::pressed, this, [&]{

        auto list = m_pFileEdit->toPlainText().split('\n');

        uint width = m_pImageWidthSpinbox->value();
        uint height = m_pImageHeightSpinbox->value();

        int multiplication = m_pImageTypeSpinbox->currentData().toBool() ? 4 : 1;

        int multisize = (width * height * multiplication);

        if(width <= 0 || height <= 0 || multisize > list.count())
        {
            QMessageBox::critical(this, "Invalid Size", "The width and height specified are invalid.");
            return;
        }

        auto filePath = QFileDialog::getSaveFileName(this, "Save HDR File","./","VTF File (*.vtf)" );

        if(filePath.isEmpty())
            return;

        auto redData = QVector<float>(list.count() * 4, 0);
        bool isOk = true;

        if(m_pImageTypeSpinbox->currentData().toBool())
        {
            for (int i = 0; i < list.count() && isOk; i+=4) {
                redData[i] = list[i].toFloat(&isOk);
                if(!isOk) break;
                redData[i + 1] = list[i + 1].toFloat(&isOk);
                if(!isOk) break;
                redData[i + 2] = list[i + 2].toFloat(&isOk);
                redData[i + 3] = 1.0f;
            }

        }
        else
        {
            for (int i = 0; i < list.count() && isOk; i+=4) {
                redData[i] = list[i].toFloat(&isOk);
                redData[i + 1] = 0.0f;
                redData[i + 2] = 0.0f;
                redData[i + 3] = 1.0f;
            }
        }

        if(!isOk)
        {
            QMessageBox::critical(this, "Invalid values", "The values are not floats.");
            return;
        }

        QVector<float*> flarray {redData.data()};

        auto vFile = new VTFLib::CVTFFile;

        SVTFCreateOptions VTFCreateOptions {};
        VTFCreateOptions.ImageFormat = VTFImageFormat::IMAGE_FORMAT_RGBA16161616F;
        VTFCreateOptions.uiVersion[0] = 7;
        VTFCreateOptions.uiVersion[1] = 4;
        VTFCreateOptions.uiStartFrame = 0;
        VTFCreateOptions.uiFlags = TEXTUREFLAGS_EIGHTBITALPHA;
        VTFCreateOptions.bSRGB = false;
        VTFCreateOptions.bResize = false;
        VTFCreateOptions.bMipmaps = false;

        vFile->Create(width, height, 1, 1, 1, reinterpret_cast<vlByte**>(flarray.data()), VTFCreateOptions, IMAGE_FORMAT_RGBA32323232F);

        vFile->Save(filePath.toUtf8().constData());

    });


}
