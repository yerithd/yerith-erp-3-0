/*
 * yeroth-erp-site-table-view.hpp
 *
 * Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ADMIN_SITE_TABLE_VIEW_HPP_
#define SRC_YEROTH_ADMIN_SITE_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPSiteTableView:public YerothTableView
{
Q_OBJECT public:

    YEROTH_CLASS_OPERATORS inline YerothERPSiteTableView():YerothTableView()
    {
        _stdItemModel->_curTableView = this;
    }

    inline YerothERPSiteTableView(QWidget *parent):YerothTableView(parent)
    {
        _stdItemModel->_curTableView = this;
    }

    inline virtual ~YerothERPSiteTableView()
    {
    }
};


#endif /* SRC_YEROTH_ADMIN_SITE_TABLE_VIEW_HPP_ */
