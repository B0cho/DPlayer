#include <QString>
#include <QtTest>
#include "creadonlycombinedmodel.h"
#include <QStringListModel>
#include <boost/shared_ptr.hpp>

class CReadOnlyCombinedModelTest : public QObject
{
    Q_OBJECT

public:
    CReadOnlyCombinedModelTest();

private Q_SLOTS:
    void rowCount();

};

CReadOnlyCombinedModelTest::CReadOnlyCombinedModelTest()
{
}

void CReadOnlyCombinedModelTest::rowCount()
{
    // submodels
    boost::shared_ptr<QStringListModel>
            model1(new QStringListModel),
            model2(new QStringListModel),
            model3(new QStringListModel),
            model4(new QStringListModel);
    model1->insertRows(0, 0);
    model2->insertRows(0, 2);
    model3->insertRows(0, 31);
    model4->insertRows(0, 561);

    // no model attached
    CReadOnlyCombinedModel no_model({});
    QCOMPARE(no_model.rowCount(), 0);

    // one empty model
    CReadOnlyCombinedModel empty_model({model1});
    QCOMPARE(empty_model.rowCount(), 0);

    // one model
    CReadOnlyCombinedModel one_model({model2});
    QCOMPARE(one_model.rowCount(), 2);

    // two models, one empty
    CReadOnlyCombinedModel one_empty_model({model1, model2});
    QCOMPARE(one_empty_model.rowCount(), 2);

    // two models
    CReadOnlyCombinedModel two_models({model2, model4});
    QCOMPARE(two_models.rowCount(), 563);

    // three models
    CReadOnlyCombinedModel three_models({model2, model3, model4});
    QCOMPARE(three_models.rowCount(), 594);
}

QTEST_APPLESS_MAIN(CReadOnlyCombinedModelTest)

#include "tst_creadonlycombinedmodeltest.moc"
