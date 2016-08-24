//============================================================================
// Name        : EmployeeView.cpp
// Author      : Aapo Keskimolo, Elisa Kortela
// Description : Declarations for employee view class methods
//============================================================================

#include <QCoreApplication>
#include "EmployeeView.h"
#include "EmployeeControllerException.h"

const string EmployeeView::MODEL_STATE_FILEPATH = "model.state";


EmployeeView::EmployeeView(QWidget *parent): QMainWindow(parent) {

    qInfo() << ">>>>>>>>>>>>>>>>>>> EXECUTION STARTED <<<<<<<<<<<<<<<<<<<<<<<";

    loadLastSavedConfig();

    m_layoutContainer = new QWidget;
    m_layoutContainer->setWindowTitle(getQStringFromXml("main_window"));

    m_mainLayout = new QHBoxLayout;
    createMenuBar();
    createEmployeeInformationView();
    createTreeWidget();
    updateLabels();

    m_layoutContainer->setLayout(m_mainLayout);
    //window->resize(QDesktopWidget().availableGeometry().size() * 0.3);
    m_layoutContainer->resize(600,350);
    m_layoutContainer->setFixedSize(m_layoutContainer->size());
    m_layoutContainer->show();
}


EmployeeView::~EmployeeView() {
//    saveCurrentModelStateToFile();
//    saveCurrentConfig();
    qInfo() << ">>>>>>>>>>>>>>>>>>> EXECUTION STOPPED <<<<<<<<<<<<<<<<<<<<<<<";
}


QString EmployeeView::getQStringFromXml(string parameterName) {

    m_xmlFile = new QFile(xml_config_filename);

    QString value = "default";

    if (m_xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)) {

        m_xmlReader = new QXmlStreamReader(m_xmlFile);

        while(!m_xmlReader->atEnd() && !m_xmlReader->hasError()) {

            QXmlStreamReader::TokenType token = m_xmlReader->readNext();

            if(token == QXmlStreamReader::StartElement && token != QXmlStreamReader::StartDocument) {

                string tag = m_xmlReader->name().toString().toStdString();

                if( tag.compare(parameterName) == 0) {
                    value = m_xmlReader->readElementText();
                    qDebug() << "Read xml parameter: " + QString::fromStdString(tag) + ": " << value;
                    break;
                }
            }
        }

        if(m_xmlReader->hasError()) {
            popErrorBox("Parse error \"" + xml_config_filename.toStdString() + "\": " +
                        m_xmlReader->errorString().toStdString());
        }

    } else {
        popErrorBox("Could not open: \"" + xml_config_filename.toStdString() + "\"\nMake sure that your Qt build configuration target (Projects->Build Directory) has been set to the directory that has SalaryAdministrationGui.pro -file\n");
        QApplication::quit();
    }

    m_xmlReader->clear();
    m_xmlFile->close();

    return value;
}


void EmployeeView::createMenuBar() {
    m_menuBar = new QMenuBar(this);
        m_mainLayout->setMenuBar(m_menuBar);

        m_fileMenu = new QMenu(getQStringFromXml("menu_main"));

        m_newAction = new QAction(getQStringFromXml("menu_new"), this);
        m_saveAsAction = new QAction(getQStringFromXml("menu_save"), this);
        m_loadAction = new QAction(getQStringFromXml("menu_load"), this);
        m_aboutAction = new QAction(getQStringFromXml("menu_about"), this);
        m_quitAction = new QAction(getQStringFromXml("menu_quit"), this);

        m_menuBar->addMenu(m_fileMenu);
        m_fileMenu->addAction(m_newAction);
        m_fileMenu->addAction(m_saveAsAction);
        m_fileMenu->addAction(m_loadAction);
        m_fileMenu->addSeparator();
        m_fileMenu->addAction(m_aboutAction);
        m_fileMenu->addSeparator();
        m_fileMenu->addAction(m_quitAction);

        connect(m_newAction, &QAction::triggered, this, &EmployeeView::handleNewClick );
        connect(m_saveAsAction, &QAction::triggered, this, &EmployeeView::handleSaveAsClick );
        connect(m_loadAction, &QAction::triggered, this, &EmployeeView::handleLoadClick);
        connect(m_aboutAction, &QAction::triggered, this, &EmployeeView::handleAboutClick );
        connect(m_quitAction, &QAction::triggered, this, &EmployeeView::handleExitClick );
}


void EmployeeView::createTreeWidget()
{
    m_treeWidget = new QTreeWidget;
    m_treeWidget->setColumnCount(3);
    m_treeWidget->setSortingEnabled(true);

    QStringList labels;
    labels
        << getQStringFromXml("editor_lastname")
        << getQStringFromXml("editor_firstname")
        << getQStringFromXml("editor_ssn");

    m_treeWidget->setHeaderLabels(labels);

    m_treeWidget->resizeColumnToContents(1);
    m_treeWidget->resizeColumnToContents(2);
    m_treeWidget->resizeColumnToContents(3);

    connect(m_treeWidget, SIGNAL (itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT (handleTreeWidgetDoubleClick()) );

    m_mainLayout->addWidget(m_treeWidget);
}


void EmployeeView::createEmployeeInformationView()
{
    // Create structure
    m_leftLayout = new QGridLayout;
    m_scrollArea = new QScrollArea;

    QWidget *empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    m_leftLayout->addWidget(empty,7,0);

    // Employee information feed
    m_lastNameLabel = new QLabel;
    m_lastNameEdit = new QLineEdit();
    m_firstNameLabel = new QLabel;
    m_firstNameEdit = new QLineEdit;
    m_SSNLabel = new QLabel;
    m_SSNEdit = new QLineEdit;

    m_payTypeLabel = new QLabel;
    m_payTypeMenu = new QComboBox;

    QStringList payTypes;
    payTypes << ""
             << employeeTypetoString(employee_types::MONTHLY_PAID_EMPLOYEE)
             << employeeTypetoString(employee_types::HOURLY_PAID_EMPLOYEE)
             << employeeTypetoString(employee_types::SALESMAN_EMPLOYEE);
    m_payTypeMenu->insertItems(0, payTypes);

    m_monthlySalaryLabel = new QLabel;
    m_monthlySalaryEdit = new QLineEdit;
    QDoubleValidator *m_mDouble = new QDoubleValidator(0,9999.99,2,this);
    m_mDouble->setNotation(QDoubleValidator::StandardNotation);
    m_monthlySalaryEdit->setValidator(m_mDouble);

    m_hourlySalaryLabel = new QLabel;
    m_hourlySalaryEdit = new QLineEdit;
    QDoubleValidator *m_hDouble = new QDoubleValidator(0,999.99,2,this);
    m_hDouble->setNotation(QDoubleValidator::StandardNotation);
    m_hourlySalaryEdit->setValidator(m_hDouble);

    m_hoursDoneLabel = new QLabel;
    m_hoursDoneEdit = new QLineEdit;
    QDoubleValidator *m_hdDouble = new QDoubleValidator(0,999.99,2,this);
    m_hdDouble->setNotation(QDoubleValidator::StandardNotation);
    m_hoursDoneEdit->setValidator(m_hdDouble);

    m_outcomeClaimLabel = new QLabel;
    m_outcomeClaimCheckBox = new QCheckBox;

    m_bonusLabel = new QLabel;
    m_bonusEdit = new QLineEdit;
    m_bonusEdit->setValidator(new QIntValidator(0,100,this));

    // Line edit size limitations
    m_lastNameEdit->setMaxLength(30);
    m_firstNameEdit->setMaxLength(30);
    m_SSNEdit->setMaxLength(11);

    // Grid construction
    m_leftLayout->addWidget(m_lastNameLabel,0,0);
    m_leftLayout->addWidget(m_lastNameEdit,0,1);

    m_leftLayout->addWidget(m_firstNameLabel,1,0);
    m_leftLayout->addWidget(m_firstNameEdit,1,1);

    m_leftLayout->addWidget(m_SSNLabel,2,0);
    m_leftLayout->addWidget(m_SSNEdit,2,1);

    m_leftLayout->addWidget(m_payTypeLabel,3,0);
    m_leftLayout->addWidget(m_payTypeMenu,3,1);

    // Monthly
    m_leftLayout->addWidget(m_monthlySalaryLabel,4,0);
    m_leftLayout->addWidget(m_monthlySalaryEdit,4,1);

    // Hourly
    m_leftLayout->addWidget(m_hoursDoneLabel,4,0);
    m_leftLayout->addWidget(m_hoursDoneEdit,4,1);
    m_leftLayout->addWidget(m_hourlySalaryLabel,5,0);
    m_leftLayout->addWidget(m_hourlySalaryEdit,5,1);

    // Salesman
    m_leftLayout->addWidget(m_outcomeClaimLabel,5,0);
    m_leftLayout->addWidget(m_outcomeClaimCheckBox,5,1);
    m_leftLayout->addWidget(m_bonusLabel,6,0);
    m_leftLayout->addWidget(m_bonusEdit,6,1);

    // Hiding widgets
    setInformationFormWidgetVisibility(false,false,false,false,false);

    m_leftLayout->setColumnStretch(0,1);

    // Buttons / Signals
    m_saveButton = new QPushButton;
    connect(m_saveButton, SIGNAL (released()), this, SLOT (handleSaveButtonClick()) );
    m_deleteButton = new QPushButton;
    connect(m_deleteButton, SIGNAL (released()), this, SLOT (handleDeleteButtonClick()) );
    connect(m_payTypeMenu, SIGNAL (currentIndexChanged(int)), this, SLOT (handlePayTypeChange()) );
    m_clearFormButton = new QPushButton;
    connect(m_clearFormButton, SIGNAL (released()), this, SLOT (handleClearFormButton()) );

    m_leftLayout->addWidget(m_saveButton,8,0);
    m_leftLayout->addWidget(m_deleteButton,8,1);
    m_leftLayout->addWidget(m_clearFormButton,9,0,1,2);

    // Set layouts
    m_scrollArea->setLayout(m_leftLayout);
    m_mainLayout->addWidget(m_scrollArea);
}

void EmployeeView::updateLabels() {
    m_lastNameLabel->setText(getQStringFromXml("editor_lastname"));
    m_firstNameLabel->setText(getQStringFromXml("editor_firstname"));
    m_SSNLabel->setText(getQStringFromXml("editor_ssn"));
    m_payTypeLabel->setText(getQStringFromXml("editor_pay_type"));
    m_monthlySalaryLabel->setText(getQStringFromXml("editor_mon_sal"));
    m_hourlySalaryLabel->setText(getQStringFromXml("editor_hour_sal"));
    m_hoursDoneLabel->setText(getQStringFromXml("editor_hours_done"));
    m_outcomeClaimLabel->setText(getQStringFromXml("editor_claim"));
    m_bonusLabel->setText(getQStringFromXml("editor_bonus"));

    m_saveButton->setText(getQStringFromXml("button_save_emp"));
    m_deleteButton->setText(getQStringFromXml("button_delete_emp"));
    m_clearFormButton->setText(getQStringFromXml("button_clear_form"));
}


void EmployeeView::registerObserver(IController* observer) {
    this->m_observer = observer;
}


void EmployeeView::updateEmployeeList(vector<Employee *> model) {
    employeeList.clear();
    m_treeWidget->clear();
    for (vector<Employee*>::iterator it = model.begin(); it != model.end(); ++it) {
        QTreeWidgetItem *row = new QTreeWidgetItem(m_treeWidget);
        row->setText(0, QString::fromStdString( (*it)->getFirstName() ));
        row->setText(1, QString::fromStdString( (*it)->getLastName() ));
        row->setText(2, QString::fromStdString( (*it)->getSocialSecurityNumber() ));
        employeeList.append(row);
    }
}


void EmployeeView::popInfoBox(string message) {
    qDebug() << "QmessageBox printing message: \"" << QString::fromStdString(message) << "\"";
    QMessageBox::information(
        this,
        tr( QCoreApplication::applicationName().toStdString().c_str() ),
        tr(message.c_str()) );
}


void EmployeeView::popErrorBox(string message) {
    qDebug() << "QmessageBox printing message: \"" << QString::fromStdString(message) << "\"";
    QMessageBox::critical(
        this,
        tr( QCoreApplication::applicationName().toStdString().c_str() ),
        tr(message.c_str()) );
}


bool EmployeeView::popQuestionBox(string title, string message) {
    QMessageBox::StandardButton confirm;
      confirm = QMessageBox::question(this, QString::fromStdString(title), QString::fromStdString(message), QMessageBox::Yes|QMessageBox::Cancel);
      if (confirm == QMessageBox::Yes) {
        qDebug() << "Yes was clicked";
        return true;
      } else {
        qDebug() << "Yes was *not* clicked";
        return false;
      }
}

bool EmployeeView::checkIfSSNExists(QString ssn) {
    qDebug() << "SSN:" << ssn;

    for (int i = 0; i < employeeList.size(); i++) {
        if (employeeList[i]->text(2) == ssn) {
            return true;
        }
    }
    return false;
}


void EmployeeView::handleSaveButtonClick() {
    qDebug() << "Save button was clicked!";

    if (checkIfSSNExists(m_SSNEdit->text())) {
        if (!popQuestionBox("Rewrite employee", "Are you sure you want to rewrite the employee data?")) {
            return;
        }
        m_observer->handleEventRemoveEmployee(m_SSNEdit->text().toStdString());
    }

    employee_types::type typ = static_cast<employee_types::type>(m_payTypeMenu->currentIndex());
    string firstName = m_firstNameEdit->text().toStdString();
    string lastName = m_lastNameEdit->text().toStdString();
    string ssn = m_SSNEdit->text().toStdString();
    double monthlySalary = m_monthlySalaryEdit->text().replace(",",".").toDouble();
    double hourlySalary = m_hourlySalaryEdit->text().replace(",",".").toDouble();
    double hoursDone = m_hoursDoneEdit->text().replace(",",".").toDouble();
    double bonus = this->m_bonusEdit->text().replace(",",".").toDouble();
    bool outcomeClaim = true ? static_cast<int>(m_outcomeClaimCheckBox->checkState()): false;

    if (typ != employee_types::UNKNOWN)
    {

        try {

            switch(typ) {

            case employee_types::MONTHLY_PAID_EMPLOYEE:
                m_observer->handleEventCreateEmployee(typ, firstName, lastName, ssn, monthlySalary, 0.0, 0.0, 0.0, false);
                break;

            case employee_types::HOURLY_PAID_EMPLOYEE:
                m_observer->handleEventCreateEmployee(typ, firstName, lastName, ssn, 0.0, hourlySalary, hoursDone, 0.0, false);
                break;

            case employee_types::SALESMAN_EMPLOYEE:
                m_observer->handleEventCreateEmployee(typ, firstName, lastName, ssn, monthlySalary, 0.0, 0.0, bonus, outcomeClaim);
                break;

            default:
                popErrorBox("Invalid employee type: " + string(employeeTypetoString(typ)) );
                break;
            }

        } catch (ec::EmployeeAlreadyExistsException &e1) {
            popErrorBox("Employee already exists: " + string(e1.what()) );

        } catch (ec::EmployeeTypeInvalidException &e2) {
            popErrorBox("Unexpected caused by invalid employee type: " + string(e2.what()) );

        } catch (exception &e) {
            popErrorBox("Unhandled error: " + string(e.what()) );
        }
    }
    else {
        popInfoBox("Please, select \"" + getQStringFromXml("editor_pay_type").toStdString() + "\"");
    }
}


void EmployeeView::handleDeleteButtonClick() {
    qDebug() << "Delete button was clicked!";

    if (m_treeWidget->currentItem() != NULL) {

        string lname = m_treeWidget->currentItem()->text(0).toStdString();
        string fname = m_treeWidget->currentItem()->text(1).toStdString();
        string ssnStr = m_treeWidget->currentItem()->text(2).toStdString();

        if(popQuestionBox("Employee deletion", "Are you sure you want to delete the employee:\n   " + lname + ", " + fname + " (SSN: " + ssnStr + ") ?") ) {

            for (int i = 0; i < employeeList.size(); i++) {

                QString ssn = employeeList[i]->text(2);
                QString curSsn = m_treeWidget->currentItem()->text(2);

                if ( ssn.compare(ssn, curSsn) == 0) {

                    try {

                        m_observer->handleEventRemoveEmployee(curSsn.toStdString() );
                        popInfoBox("Employee (SSN: " + ssn.toStdString() + ") deleted");
                        break;

                    } catch( ec::SsnDoesNotExistException &e1) {
                        popErrorBox("Unable to delete employee! " + string(e1.what()) );

                    } catch(exception &e) {
                        popErrorBox("Unknown exception occurred: " + string(e.what()) );
                    }
                }
            }
        }

    } else {
        popErrorBox("Unable to delete! No employee selected.");
    }
}


void EmployeeView::handleClearFormButton() {
    qDebug() << "Add new employee button clicked!";
    clearForm();
    m_payTypeMenu->setCurrentIndex(0);
}


void EmployeeView::clearForm() {
    m_firstNameEdit->clear();
    m_lastNameEdit->clear();
    m_SSNEdit->clear();
    m_monthlySalaryEdit->clear();
    m_hoursDoneEdit->clear();
    m_hourlySalaryEdit->clear();
    m_outcomeClaimCheckBox->setChecked(false);
    m_bonusEdit->clear();
}


void EmployeeView::handleTreeWidgetDoubleClick() {
    unsigned int rowIdx = m_treeWidget->currentIndex().row();
    qDebug() << "Item double clicked: " << QString::number(rowIdx);

    clearForm();

    Employee *m_emp = m_observer->handleEventGetEmployee(m_treeWidget->currentItem()->text(2).toStdString());

    m_firstNameEdit->setText(QString::fromStdString(m_emp->getFirstName()));
    m_lastNameEdit->setText(QString::fromStdString(m_emp->getLastName()));
    m_SSNEdit->setText(m_treeWidget->currentItem()->text(2));

    if (m_emp->getType() == employee_types::MONTHLY_PAID_EMPLOYEE)
    {
        m_payTypeMenu->setCurrentIndex(1);
        MonthlyPaidEmployee *m_monthlyEmp = dynamic_cast <MonthlyPaidEmployee*> (m_emp);

        m_monthlySalaryEdit->setText(QString::number(m_monthlyEmp->getSalary()));

    } else if (m_emp->getType() == employee_types::HOURLY_PAID_EMPLOYEE)
    {
        m_payTypeMenu->setCurrentIndex(2);
        HourlyPaidEmployee *m_hourlyEmp = dynamic_cast <HourlyPaidEmployee*> (m_emp);

        m_hoursDoneEdit->setText(QString::number(m_hourlyEmp->getDoneHours()));
        m_hourlySalaryEdit->setText(QString::number(m_hourlyEmp->getHourlySalary()));

    } else
    {
        m_payTypeMenu->setCurrentIndex(3);
        SalesmanEmployee* m_salesEmp = dynamic_cast <SalesmanEmployee*> (m_emp);

        m_monthlySalaryEdit->setText(QString::number(m_salesEmp->getMonthlySalary()));
        m_outcomeClaimCheckBox->setChecked(m_salesEmp->getOutcomeClaim());
        m_bonusEdit->setText(QString::number(m_salesEmp->getBonus()));
    }
}


void EmployeeView::handlePayTypeChange() {
    qDebug() << "Pay type menu active index changed!";

    if (m_payTypeMenu->currentText().toStdString() == employeeTypetoString(employee_types::MONTHLY_PAID_EMPLOYEE)) {
        setInformationFormWidgetVisibility(true,false,false,false,false);

    } else if (m_payTypeMenu->currentText().toStdString() == employeeTypetoString(employee_types::HOURLY_PAID_EMPLOYEE)) {
        setInformationFormWidgetVisibility(false,true,true,false,false);

    } else if (m_payTypeMenu->currentText().toStdString() == employeeTypetoString(employee_types::SALESMAN_EMPLOYEE)) {
        setInformationFormWidgetVisibility(true,false,false,true,true);

    } else {
        setInformationFormWidgetVisibility(false,false,false,false,false);
    }
}


void EmployeeView::handleExitClick() {
    qDebug() << "Exit click detected!!";

    if(popQuestionBox("Exit", "Are you sure to exit the program?")) {
        saveCurrentModelStateToFile();
        saveCurrentConfig();
        close();
        qApp->exit(0);
    }
}


void EmployeeView::setInformationFormWidgetVisibility(bool mSal, bool hDone, bool hSal, bool oClaim, bool bonus) {
    m_monthlySalaryLabel->setVisible(mSal);
    m_monthlySalaryEdit->setVisible(mSal);
    m_hoursDoneLabel->setVisible(hDone);
    m_hoursDoneEdit->setVisible(hDone);
    m_hourlySalaryLabel->setVisible(hSal);
    m_hourlySalaryEdit->setVisible(hSal);
    m_outcomeClaimLabel->setVisible(oClaim);
    m_outcomeClaimCheckBox->setVisible(oClaim);
    m_bonusLabel->setVisible(bonus);
    m_bonusEdit->setVisible(bonus);
}


void EmployeeView::saveCurrentConfig() {
    ofstream ofs(CONFIGURATION_FILEPATH, ios::out | ios::binary);
    ofs << (*this);
    ofs.close();
    qInfo() << "Current configuration saved in" << QString::fromStdString(CONFIGURATION_FILEPATH) << "";
}


void EmployeeView::loadLastSavedConfig() {

    ifstream ifs(CONFIGURATION_FILEPATH, ios::in | ios::binary);

    if (ifs.good())
    {
        if( !(ifs >> (*this) )) {
            popErrorBox("Unable to load previously saved configuration from " + CONFIGURATION_FILEPATH + "");
        } else {
            qInfo() << "Previously saved configuration loaded from" << QString::fromStdString(CONFIGURATION_FILEPATH);
        }

    } else {
        popInfoBox("No existing configuration file " + CONFIGURATION_FILEPATH + " found");
    }

   ifs.close();
}


void EmployeeView::saveCurrentModelStateToFile(const string filepath) {

    if (m_observer != nullptr) {
        m_observer->handleEventSaveModelStateToFile(filepath);
        configLastModelStateFilepath = filepath;

    } else {
        popErrorBox("Unable to save model state: Observer is null!");
        qCritical() << "Unable to save model state: Observer is null!";
    }
}


void EmployeeView::loadLastModelStateFromFile(const string filepath) {
    if (m_observer != nullptr) {
        m_observer->handleEventLoadModelStateFromFile(filepath);

    } else{
        popErrorBox("Unable to load model state: Observer is null!");
        qCritical() << "Unable to load model state: Observer is null!";
    }
}

void EmployeeView::handleSaveAsClick() {
    qDebug() << "Menu item '/Save'/ click detected!!";

    QString filepath = QFileDialog::getSaveFileName(this,
            getQStringFromXml("dialog_save_title"), "",
            tr("(*.cfg)"));

    if (filepath.isEmpty())
            return;
    else {
        m_observer->handleEventSaveModelStateToFile(filepath.toStdString());
    }
}

void EmployeeView::handleLoadClick() {
    qDebug() << "Menu item '/Load'/  click detected!!";

    QString filepath = QFileDialog::getOpenFileName(this,
            getQStringFromXml("dialog_load_title"), "",
            tr("(*.cfg)"));

    if (filepath.isEmpty())
            return;
    else {
        m_observer->handleEventRequestViewUpdate();
        m_observer->handleEventLoadModelStateFromFile(filepath.toStdString());
    }
}

void EmployeeView::handleNewClick() {
    qDebug() << "Menu item '/New'/  click detected!!";
}

void EmployeeView::handleAboutClick() {
    qDebug() << "Menu item '/About'/  click detected!!";
}
