/*
 * yeroth-erp-generic-dialog.cpp
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-generic-select-db-field-dialog.hpp"

#include "src/yeroth-erp-windows.hpp"

#include <QtWidgets/QDesktopWidget>


YerothERPGenericSelectDBFieldDialog::YerothERPGenericSelectDBFieldDialog(YerothERPWindows *allWindows, YerothWindowsCommons *associatedWindow):YerothPOSDialogCommons(""),
    _associatedWindow(associatedWindow),
    _currentPosition(new QPoint)
{
    QDesktopWidget &desktopWidget = _allWindows->desktopWidget();

    YerothUtils::getCenterPosition(desktopWidget.width(),
                                   desktopWidget.height(),
                                   *this, *_currentPosition);

    setFixedSize(width(), height());

    setModal(true);
}

YerothERPGenericSelectDBFieldDialog::~YerothERPGenericSelectDBFieldDialog()
{
    delete _currentPosition;
}


void YerothERPGenericSelectDBFieldDialog::show()
{
    move(*_currentPosition);

    YerothPOSDialogCommons::show();
}


void YerothERPGenericSelectDBFieldDialog::closeEvent(QCloseEvent *closeEvent)
{
    /*
     * Works fine only if this dialog is set as modal.
     */
    if (isVisible() && 0 != _associatedWindow)
    {
        _associatedWindow->set_PARAMETER_TABLE_COLUMN_ORDER();

        YerothERPWindows *allWindows = YerothUtils::getAllWindows();

        if (0 != allWindows)
        {
            YerothPOSUser *aUser = allWindows->getUser();

            if (0 != aUser)
            {
                aUser->save_user_personal_PRINTING_PARAMETER_settings();
            }
        }

        _associatedWindow->rendreVisible(_associatedWindow->
                                         getCurStocksTableModel());
    }

    YerothPOSDialogCommons::closeEvent(closeEvent);
}
