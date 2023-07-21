/*
 * yeroth-erp-fournisseur-details-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_FOURNISSEUR_DETAILS_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_FOURNISSEUR_DETAILS_WINDOW_HPP_

#include "../../../ui_yeroth-erp-fournisseur-details-window.h"


#include "src/utils/yeroth-erp-logger.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"


#include "src/utils/yeroth-erp-LIST-map-COMPLEX-ITEM.hpp"


#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class YerothFournisseurDetailsWindow : public YerothWindowsCommons,
    								   private Ui_YerothFournisseurDetailsWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothFournisseurDetailsWindow();

    inline ~YerothFournisseurDetailsWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_fournisseurDetailsWindow;
    }

    inline virtual void definirCaissier()
    {
        definirPasDeRole();
    }

    void definirManager();

    inline virtual void definirVendeur()
    {
        definirPasDeRole();
    }

    inline virtual void definirGestionaireDesStocks()
    {
        definirPasDeRole();
    }

    inline virtual void definirMagasinier()
    {
        definirPasDeRole();
    }

    void definirPasDeRole();


    double GET_BEST_CURRENT_pay_group_MONEY_BENEFITS();


    double calculate_PAY_GROUP_MONEY_BENEFITS(const QString &PAY_GROUP);


    void CALCULATE_EMPLOYEE_SALARY(const QString  &a_nom_entreprise_client,
                                   bool           CALL_update_lineedits_and_labels = false);


    void rendreInvisible();

    void rendreVisible(YerothSqlTableModel *fournisseurTableModel,
                       YerothSqlTableModel *stocksTableModel,
					   bool 			   employe = false);

public slots:

    virtual bool imprimer_pdf_document();

    inline virtual void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    inline virtual void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr("Vous pouvez modifier les détails de ce fournisseur"
                                    		   	   " en cliquant sur 'Modifier' !"));
    }

protected:

    virtual void setupShortcuts();

private slots:

    void modifierFournisseur();

    void supprimerFournisseur();

private:

    void setupLineEdits();

    void showFournisseurDetail(bool employe = false);

    void checkCourrierAlerts();



    YerothLogger *_logger;

    bool _SHOW_EMPLOYEE;

    double _sommeTotal_HORS_TAXES;

    double _sommeTotal;

    YerothSqlTableModel *_curFournisseurTableModel;

    Yeroth_LIST_map_COMPLEX_ITEM _EMPLOYEE_group_program_TO_money_benefit;
};


#endif /* SRC_WINDOWS_YEROTH_FOURNISSEUR_DETAILS_WINDOW_HPP_ */
