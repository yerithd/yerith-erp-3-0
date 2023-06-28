/*
 * yeroth-erp-comptabilite-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-comptabilite-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-user-administrateur.hpp"

#include "src/users/yeroth-erp-user-caissier.hpp"

#include "src/users/yeroth-erp-user-magasinier.hpp"

#include "src/users/yeroth-erp-user-manager.hpp"

#include "src/utils/yeroth-erp-info-entreprise.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/dialogs/yeroth-erp-changer-utilisateur-dialog.hpp"

#include "src/dialogs/yeroth-erp-generic-select-db-field-dialog.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>


YerothComptabiliteWindow::YerothComptabiliteWindow()
:YerothWindowsCommons("yeroth-erp-listing-comptabilite"),
 YerothAbstractClassYerothSearchWindow(YerothDatabase::COMPTES_DOPERATIONS_COMPTABLES),
 _comptesDoperationsComptablesTransactionFilter(YerothUtils::EMPTY_STRING),
 _logger(new YerothLogger("YerothComptabiliteWindow")),
 _curCompte_DOPERATIONS_COMPTABLES_SqlTableModel
 	 (&_allWindows->getSqlTableModel_comptes_doperations_comptables()),
 _pushButton_comptabilite_filtrer_font(0)
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("comptabilité"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

#ifdef YEROTH_FEATURES_COMPTABILITE_yes

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_comptes_doperations_comptables);


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_comptes_doperations_comptables);

//    _list_yeroth_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
//              << pushButton_afficher;

    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
            << actionExporter_au_format_csv
            << action_parametrer_les_impressions << actionAfficherPDF;

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_GREEN_2_160_170,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


    setup_select_configure_dbcolumn(YerothDatabase::COMPTES_DOPERATIONS_COMPTABLES);

    YEROTH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerothDatabase::COMPTES_DOPERATIONS_COMPTABLES);

    reinitialiser_colones_db_visibles();

    setupLineEdits();

    setupDateTimeEdits();

    populateComboBoxes();

    _pushButton_comptabilite_filtrer_font =
                    new QFont(pushButton_comptabilite_filtrer->font());

    tableView_comptes_doperations_comptables
    	->setSqlTableName(&YerothDatabase::COMPTES_DOPERATIONS_COMPTABLES);


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    pushButton_creer_le_compte->disable(this);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    connect(tabWidget_creer, SIGNAL(currentChanged(int)), this, SLOT(handle_change_tab(int)));

    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this, SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(print_PDF_PREVIOUSLY_SETUP()));
    connect(actionReinitialiserChampsDBVisible,
    		SIGNAL(triggered()),
			this,
			SLOT(slot_reinitialiser_colones_db_visibles()));
    connect(actionChampsDBVisible, SIGNAL(triggered()), this,
            SLOT(selectionner_champs_db_visibles()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this,
            SLOT(export_csv_file()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));

    setupShortcuts();

#endif //#ifdef YEROTH_FEATURES_COMPTABILITE_yes
}


YerothComptabiliteWindow::~YerothComptabiliteWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR delete _logger;
}


void YerothComptabiliteWindow::afficher_comptes_bancaires(YerothSqlTableModel &
                                                          aYerothSqlTableModel)
{
    tableView_comptes_doperations_comptables->
    queryYerothTableViewCurrentPageContentRow(aYerothSqlTableModel);

    tableView_show_or_hide_columns(*tableView_comptes_doperations_comptables);

    int rowCount =
                    tableView_comptes_doperations_comptables->
                    getYerothSqlTableModelTotalRowCount();

    lineEdit_nombre_de_comptes_bancaires->setText(GET_NUM_STRING(rowCount));
}


void YerothComptabiliteWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerothDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE
            << YerothDatabaseTableColumn::NUMERO_DU_COMPTE_DOPERATION_COMPTABLE
            << YerothDatabaseTableColumn::RAISON_DOPERATION_COMPTABLE
            << YerothDatabaseTableColumn::DESCRIPTION_DU_COMPTE_DOPERATION_COMPTABLE;
}


void YerothComptabiliteWindow::hideEvent(QHideEvent *hideEvent)
{
    _selectExportDBQDialog->close();
}


void YerothComptabiliteWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF
    (*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerothComptabiliteWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();
    afficher_comptes_bancaires();
}


void YerothComptabiliteWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_comptabilite_element_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_comptabilite_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
            //qDebug() << "++ searchTermList: " << partSearchTerm;

            _searchFilter.append(QString("%1").arg(GENERATE_SQL_LIKE_STMT
                                                   (YerothDatabaseTableColumn::
                                                    DESCRIPTION_DU_COMPTE_DOPERATION_COMPTABLE,
                                                    partSearchTerm)));

            if (k != lastIdx)
            {
                _searchFilter.append(" AND ");
            }
        }
    }


    YerothWindowsCommons::setYerothLineEditQCompleterSearchFilter(_searchFilter);


    YerothLineEdit *aYerothLineEdit = 0;

    QString correspondingDBFieldKeyValue;

    QString aTableColumnFieldContentForANDSearch;

    QMapIterator<YerothLineEdit **, QString> it(_lineEditsToANDContentForSearch);

    while (it.hasNext())
    {
        it.next();

        aYerothLineEdit = *it.key();

        correspondingDBFieldKeyValue = it.value();

        if (0 != aYerothLineEdit)
        {
            aTableColumnFieldContentForANDSearch = aYerothLineEdit->text();

            if (!correspondingDBFieldKeyValue.isEmpty() &&
                    !aTableColumnFieldContentForANDSearch.isEmpty())
            {
                if (!_searchFilter.isEmpty())
                {
                    _searchFilter.append(" AND ");
                }

                _searchFilter.append(GENERATE_SQL_IS_STMT
                                     (correspondingDBFieldKeyValue,
                                      aTableColumnFieldContentForANDSearch));
            }
        }
    }

    QString finalSearchFilter(_comptesDoperationsComptablesTransactionFilter);

    if (!_searchFilter.isEmpty())
    {
        QString searchFilterWithDate(QString("%1 AND (%2)").arg
                                     (_comptesDoperationsComptablesTransactionFilter,
                                      _searchFilter));

        finalSearchFilter = searchFilterWithDate;
    }

    _yerothSqlTableModel->yerothSetFilter_WITH_where_clause(finalSearchFilter);

//    QDEBUG_STRING_OUTPUT_2("finalSearchFilter", finalSearchFilter);

    if (_yerothSqlTableModel->select())
    {
        afficher_comptes_bancaires(*_yerothSqlTableModel);
    }
    else
    {
        qDebug() <<
                 QString
                 ("++ YerothStocksWindow::textChangedSearchLineEditsQCompleters(): %1").
                 arg(_yerothSqlTableModel->lastError().text());
    }
}


void YerothComptabiliteWindow::handle_change_tab(int current_tab_index)
{
    switch (current_tab_index)
    {
    case 1:
        updateComboBoxes();
        break;

    case 2:
        creer_COMPTE_DOPERATION_CHECK_FIELDS();
        break;

    default:
        break;
    }
}


void YerothComptabiliteWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
        _pushButton_comptabilite_filtrer_font->setUnderline(true);
    }
    else
    {
        _pushButton_comptabilite_filtrer_font->setUnderline(false);
    }

    pushButton_comptabilite_filtrer->setFont(*_pushButton_comptabilite_filtrer_font);
}


bool YerothComptabiliteWindow::creer_le_compte_doperation_comptable()
{
    if (creer_COMPTE_DOPERATION_CHECK_FIELDS())
    {
        YerothSqlTableModel &comptes_doperations_comptables_TableModel =
                        _allWindows->getSqlTableModel_comptes_doperations_comptables();

        QSqlRecord record = comptes_doperations_comptables_TableModel.record();

        record.setValue(YerothDatabaseTableColumn::ID,
                        YerothERPWindows::getNextIdSqlTableModel_comptes_doperations_comptables());

        record.setValue(YerothDatabaseTableColumn::NUMERO_DU_COMPTE_DOPERATION_COMPTABLE,
        				lineEdit_comptabilite_numero_de_compte->text());

        record.setValue(YerothDatabaseTableColumn::RAISON_DOPERATION_COMPTABLE,
                        lineEdit_comptabilite_RAISON_de_loperation->text());

        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_DU_COMPTE_DOPERATION_COMPTABLE,
        				textEdit_comptabilite_DESCRIPTION->toPlainText());

        bool success = comptes_doperations_comptables_TableModel
        					.insertNewRecord(record,
        								     this,
											 "src/windows/yeroth-erp-comptabilite-window.cpp",
											 355,
											 lineEdit_comptabilite_RAISON_de_loperation->text());

        QString retMsg(QObject::tr("Le compte d'opération comptable numéro '%1' ")
        				 .arg(lineEdit_comptabilite_numero_de_compte->text()));

        if (success)
        {
            retMsg.append(QObject::tr(" a été créer avec succès !"));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("créer 1 compte d'opération comptable"),
                                           retMsg);

            clear_creer_le_compte_doperation_comptable();

            afficher_comptes_bancaires();

            tabWidget_creer->setCurrentIndex(0);

            return true;
        }
        else
        {
            retMsg.append(QObject::tr(" n'a pas pu être créer !"));

            YerothQMessageBox::warning(this,
                                       QObject::tr
                                       ("créer 1 compte client"), retMsg);
            return false;
        }
    }

    return false;
}


void YerothComptabiliteWindow::clear_creer_le_compte_doperation_comptable()
{
    lineEdit_comptabilite_numero_de_compte->clear();
    lineEdit_comptabilite_RAISON_de_loperation->clear();
    textEdit_comptabilite_DESCRIPTION->clear();
}


void YerothComptabiliteWindow::refineYerothLineEdits()
{
    _comptesDoperationsComptablesTransactionFilter.clear();

//      _comptesDoperationsComptablesTransactionFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
//                                                                                                      .arg(YerothDatabaseTableColumn::DATE_PAIEMENT,
//                                                                                                               DATE_TO_DB_FORMAT_STRING(dateEdit_comptabilite_debut->date()),
//                                                                                                                       YerothDatabaseTableColumn::DATE_PAIEMENT,
//                                                                                                                       DATE_TO_DB_FORMAT_STRING(dateEdit_comptabilite_fin->date())));
//
//      setupLineEditsQCompleters((QObject *)this);

    afficher_comptes_bancaires();
}


bool YerothComptabiliteWindow::creer_COMPTE_DOPERATION_CHECK_FIELDS()
{
    bool result = false;

    bool numero_de_compte_doperation_comptable =
                    lineEdit_comptabilite_numero_de_compte->checkField();

    bool raison_de_loperation_comptable =
                    lineEdit_comptabilite_RAISON_de_loperation->checkField();

    result = numero_de_compte_doperation_comptable &&
             raison_de_loperation_comptable;

    return result;
}


void YerothComptabiliteWindow::updateComboBoxes()
{
    comboBox_comptes_doperations_comptables->clear();

    comboBox_types_doperations_comptables->clear();

    comboBox_comptes_doperations_comptables->populateComboBoxMissingRawString
    (YerothDatabaseTableColumn::NUMERO_DU_COMPTE_DOPERATION_COMPTABLE,
     YerothDatabase::COMPTES_DOPERATIONS_COMPTABLES,
     YerothUtils::EMPTY_STRING);

    comboBox_types_doperations_comptables->populateComboBox();
}


void YerothComptabiliteWindow::populateComboBoxes()
{
    comboBox_comptes_doperations_comptables->populateComboBoxMissingRawString
    (YerothDatabaseTableColumn::NUMERO_DU_COMPTE_DOPERATION_COMPTABLE,
     YerothDatabase::COMPTES_DOPERATIONS_COMPTABLES,
     YerothUtils::EMPTY_STRING);


    comboBox_types_doperations_comptables->setupPopulateNORawString
    (YerothDatabase::TYPE_DOPERATIONS_FINANCIERES,
     YerothDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE,
     &YerothUtils::_typedoperationfinancieresToUserViewString);

    comboBox_types_doperations_comptables->populateComboBox();
}


void YerothComptabiliteWindow::setupLineEdits()
{
    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_comptabilite_nombre_de_lignes_par_page;

    lineEdit_comptabilite_terme_recherche->enableForSearch(QObject::tr
                                                           ("terme à rechercher (description du compte bancaire)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr
                                                    ("valeur à rechercher"));

    lineEdit_comptabilite_element_resultat->
    setValidator(&YerothUtils::POSITIVE_DoubleValidator);

    lineEdit_nombre_de_comptes_bancaires->setYerothEnabled(false);
    lineEdit_comptabilite_encaisse->setYerothEnabled(false);
    lineEdit_comptabilite_debourse->setYerothEnabled(false);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT
    (lineEdit_comptabilite_nombre_de_lignes_par_page,
     tableView_comptes_doperations_comptables);
}


void YerothComptabiliteWindow::setupDateTimeEdits()
{
    dateEdit_comptabilite_debut->
    setStartDate(YerothERPConfig::GET_YEROTH_PAGING_DEFAULT_START_DATE());

    dateEdit_comptabilite_fin->setStartDate(GET_CURRENT_DATE);

    _comptesDoperationsComptablesTransactionFilter.clear();

//      _comptesDoperationsComptablesTransactionFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
//                                                                                                      .arg(YerothDatabaseTableColumn::DATE_PAIEMENT,
//                                                                                                               DATE_TO_DB_FORMAT_STRING(dateEdit_comptabilite_debut->date()),
//                                                                                                                       YerothDatabaseTableColumn::DATE_PAIEMENT,
//                                                                                                                       DATE_TO_DB_FORMAT_STRING(dateEdit_comptabilite_fin->date())));
//
//    connect(dateEdit_comptabilite_debut,
//              SIGNAL(dateChanged(const QDate &)),
//                      this,
//                      SLOT(refineYerothLineEdits()));
//
//    connect(dateEdit_comptabilite_fin,
//              SIGNAL(dateChanged(const QDate &)),
//                      this,
//                      SLOT(refineYerothLineEdits()));
}



void YerothComptabiliteWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curCompte_DOPERATIONS_COMPTABLES_SqlTableModel)
    pushButton_creer_le_compte->enable(this,
                                       SLOT
                                       (creer_le_compte_doperation_comptable
                                        ()));
}


void YerothComptabiliteWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_creer_le_compte->disable(this);
}


void YerothComptabiliteWindow::rendreVisible(YerothSqlTableModel *
                                             stocksTableModel)
{
    retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    _curCompte_DOPERATIONS_COMPTABLES_SqlTableModel =
                    &_allWindows->getSqlTableModel_comptes_doperations_comptables();

    setYerothSqlTableModel(_curCompte_DOPERATIONS_COMPTABLES_SqlTableModel);

    updateComboBoxes();

    setVisible(true);

    afficher_comptes_bancaires();

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("comptabilité"),
    								 _curCompte_DOPERATIONS_COMPTABLES_SqlTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();

    lineEdit_comptabilite_terme_recherche->setFocus();
}


bool YerothComptabiliteWindow::export_csv_file()
{
    bool success = false;

    QList < int >tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        success = YerothUtils::export_csv_file(*this,
                                               *tableView_comptes_doperations_comptables,
                                               tableColumnsToIgnore,
                                               "yeroth-erp-financial-accounting-operation-listing-csv-format",
                                               "financial accounting operation listing");
    }
    else
    {
        success = YerothUtils::export_csv_file(*this,
                                               *tableView_comptes_doperations_comptables,
                                               tableColumnsToIgnore,
                                               "yeroth-erp-fichier-comptes-doperations-comptables-format-csv",
                                               "liste des comptes opérations comptables");
    }

    return success;
}


QString YerothComptabiliteWindow::get_latex_template_print_pdf_content()
{
    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerothUtils::EN_template_comptabilite_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerothUtils::FR_template_comptabilite_tex;
    }

    return _latex_template_print_pdf_content;
}


void YerothComptabiliteWindow::deconnecter_utilisateur()
{
    clear_creer_le_compte_doperation_comptable();
    YerothWindowsCommons::deconnecter_utilisateur();
}
