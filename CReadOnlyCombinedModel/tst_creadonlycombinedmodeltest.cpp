#include <QString>
#include <QtTest>
#include "creadonlycombinedmodel.h"
#include <QStringListModel>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

class CReadOnlyCombinedModelTest : public QObject
{
    Q_OBJECT

public:
    CReadOnlyCombinedModelTest();

private:
    // submodels
    std::array<boost::shared_ptr<QAbstractListModel>, 4> submodels;

    /*
     * Class for mocking submodels
     */
    class abstractListModel: public QAbstractListModel
    {
    private:
        Qt::ItemFlags _flags;
        uint _size = 0;
        QList<void*> _pointers;
    public:
        abstractListModel(Qt::ItemFlags flags, uint size): QAbstractListModel(), _flags(flags), _size(size) {
            for(uint i = 0; i < size; i++)
                _pointers.append((void*) new uint(i));
        }

        Qt::ItemFlags flags(const QModelIndex &index) const override {
            Q_UNUSED(index);
            return _flags;
        }

        QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override {
            Q_UNUSED(parent);
            if(column != 0 || row < 0)
                return QModelIndex();
            return createIndex(row, 0, _pointers.at(row));
        }

        int rowCount(const QModelIndex& parent = QModelIndex()) const override {
            Q_UNUSED(parent);
            return _size;
        }

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
            Q_UNUSED(role);
            return QVariant(index);
        }
    };

private Q_SLOTS:
    void init();
    void cleanup();
    void rowCount();
    void flags();
    void columnCount();
    //void index();
    void childModelIndex();
    void data();
};

CReadOnlyCombinedModelTest::CReadOnlyCombinedModelTest()
{
}

void CReadOnlyCombinedModelTest::init()
{
    // initialization including itemFlags
    submodels[0] = boost::make_shared<abstractListModel>(Qt::NoItemFlags, 0);
    submodels[1] = boost::make_shared<abstractListModel>(Qt::ItemIsDragEnabled, 153);
    submodels[2] = boost::make_shared<abstractListModel>(Qt::ItemIsEnabled | Qt::ItemIsSelectable, 8);
    submodels[3] = boost::make_shared<abstractListModel>(Qt::ItemIsUserCheckable | Qt::ItemIsDropEnabled, 12365);
}

void CReadOnlyCombinedModelTest::cleanup()
{
    std::for_each(submodels.begin(), submodels.end(), [](auto i){ i.reset(); });
}

void CReadOnlyCombinedModelTest::rowCount()
{
    // mocking
    CReadOnlyCombinedModel no_model({});    // no model attached
    CReadOnlyCombinedModel empty_model({submodels[0]});    // one empty model
    CReadOnlyCombinedModel one_model({submodels[1]});   // one model
    CReadOnlyCombinedModel one_empty_model({submodels[0], submodels[1]});     // two models, one empty
    CReadOnlyCombinedModel two_models({submodels[1], submodels[2]});   // two models
    CReadOnlyCombinedModel three_models({submodels[1], submodels[2], submodels[3]});     // three models

    // testing
    QCOMPARE(no_model.rowCount(), 0);
    QCOMPARE(empty_model.rowCount(), 0);    
    QCOMPARE(one_model.rowCount(), submodels[1]->rowCount());   
    QCOMPARE(one_empty_model.rowCount(), submodels[1]->rowCount());    
    QCOMPARE(two_models.rowCount(), submodels[1]->rowCount() + submodels[2]->rowCount());    
    QCOMPARE(three_models.rowCount(), submodels[1]->rowCount() + submodels[2]->rowCount() + submodels[3]->rowCount());
}

void CReadOnlyCombinedModelTest::flags()
{
    // mocking
    CReadOnlyCombinedModel no_model({});    // no model attached
    CReadOnlyCombinedModel empty_model({submodels[0]});    // one empty model
    CReadOnlyCombinedModel one_model({submodels[1]});   // one model
    CReadOnlyCombinedModel one_empty_model({submodels[0], submodels[1]});     // two models, one empty
    CReadOnlyCombinedModel two_models({submodels[1], submodels[2]});   // two models
    CReadOnlyCombinedModel three_models({submodels[1], submodels[2], submodels[3]});     // three models


    /// testing
    // NO MODEL
    QCOMPARE(no_model.flags(QModelIndex()), Qt::NoItemFlags);
    QCOMPARE(no_model.flags(no_model.index(-1)), Qt::NoItemFlags);
    QCOMPARE(no_model.flags(no_model.index(0)), Qt::NoItemFlags);
    QCOMPARE(no_model.flags(no_model.index(8)), Qt::NoItemFlags);
    QCOMPARE(no_model.flags(no_model.index(200)), Qt::NoItemFlags);

    // EMPTY MODEL
    QCOMPARE(empty_model.flags(QModelIndex()), Qt::NoItemFlags);
    QCOMPARE(empty_model.flags(empty_model.index(-1)), Qt::NoItemFlags);
    QCOMPARE(empty_model.flags(empty_model.index(0)), Qt::NoItemFlags);
    QCOMPARE(empty_model.flags(empty_model.index(8)), Qt::NoItemFlags);
    QCOMPARE(empty_model.flags(empty_model.index(200)), Qt::NoItemFlags);

    // ONE MODEL
    QCOMPARE(one_model.flags(QModelIndex()), Qt::NoItemFlags);
    QCOMPARE(one_model.flags(one_model.index(-1)), Qt::NoItemFlags);
    QCOMPARE(one_model.flags(one_model.index(0)), Qt::ItemIsDragEnabled);
    QCOMPARE(one_model.flags(one_model.index(8)), Qt::ItemIsDragEnabled);
    QCOMPARE(one_model.flags(one_model.index(150)), Qt::ItemIsDragEnabled);
    QCOMPARE(one_model.flags(one_model.index(160)), Qt::NoItemFlags);

    // ONE EMPTY MODEL
    QCOMPARE(one_empty_model.flags(QModelIndex()), Qt::NoItemFlags);
    QCOMPARE(one_empty_model.flags(one_empty_model.index(-1)), Qt::NoItemFlags);
    QCOMPARE(one_empty_model.flags(one_empty_model.index(0)), Qt::ItemIsDragEnabled);
    QCOMPARE(one_empty_model.flags(one_empty_model.index(8)), Qt::ItemIsDragEnabled);
    QCOMPARE(one_empty_model.flags(one_empty_model.index(150)), Qt::ItemIsDragEnabled);
    QCOMPARE(one_empty_model.flags(one_empty_model.index(160)), Qt::NoItemFlags);

    // TWO MODELS
    QCOMPARE(two_models.flags(QModelIndex()), Qt::NoItemFlags);
    QCOMPARE(two_models.flags(two_models.index(-1)), Qt::NoItemFlags);
    QCOMPARE(two_models.flags(two_models.index(0)), Qt::ItemIsDragEnabled);
    QCOMPARE(two_models.flags(two_models.index(8)), Qt::ItemIsDragEnabled);
    QCOMPARE(two_models.flags(two_models.index(150)), Qt::ItemIsDragEnabled);
    QCOMPARE(two_models.flags(two_models.index(155)), Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QCOMPARE(two_models.flags(two_models.index(160)), Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QCOMPARE(two_models.flags(two_models.index(180)), Qt::NoItemFlags);

    // THREE MODELS
    QCOMPARE(three_models.flags(QModelIndex()), Qt::NoItemFlags);
    QCOMPARE(three_models.flags(three_models.index(-1)), Qt::NoItemFlags);
    QCOMPARE(three_models.flags(three_models.index(0)), Qt::ItemIsDragEnabled);
    QCOMPARE(three_models.flags(three_models.index(8)), Qt::ItemIsDragEnabled);
    QCOMPARE(three_models.flags(three_models.index(150)), Qt::ItemIsDragEnabled);
    QCOMPARE(three_models.flags(three_models.index(155)), Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QCOMPARE(three_models.flags(three_models.index(160)), Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QCOMPARE(three_models.flags(three_models.index(168)), Qt::ItemIsUserCheckable | Qt::ItemIsDropEnabled);
    QCOMPARE(three_models.flags(three_models.index(1200)), Qt::ItemIsUserCheckable | Qt::ItemIsDropEnabled);
    QCOMPARE(three_models.flags(three_models.index(12500)), Qt::ItemIsUserCheckable | Qt::ItemIsDropEnabled);
    QCOMPARE(three_models.flags(three_models.index(12530)), Qt::NoItemFlags);
}

void CReadOnlyCombinedModelTest::columnCount()
{
    // mocking
    CReadOnlyCombinedModel no_model({});    // no model attached
    CReadOnlyCombinedModel empty_model({submodels[0]});    // one empty model
    CReadOnlyCombinedModel one_model({submodels[1]});   // one model
    CReadOnlyCombinedModel one_empty_model({submodels[0], submodels[1]});     // two models, one empty
    CReadOnlyCombinedModel two_models({submodels[1], submodels[2]});   // two models
    CReadOnlyCombinedModel three_models({submodels[1], submodels[2], submodels[3]});     // three models
    const QModelIndex parent; // parent for argument

    // testing
    QCOMPARE(no_model.columnCount(parent), 1);
    QCOMPARE(empty_model.columnCount(parent), 1);
    QCOMPARE(one_model.columnCount(parent), 1);
    QCOMPARE(one_empty_model.columnCount(parent), 1);
    QCOMPARE(two_models.columnCount(parent), 1);
    QCOMPARE(three_models.columnCount(parent), 1);
}

void CReadOnlyCombinedModelTest::childModelIndex()
{
    /// mocking
    CReadOnlyCombinedModel no_model({});    // no model attached
    CReadOnlyCombinedModel empty_model({submodels[0]});    // one empty model
    CReadOnlyCombinedModel one_model({submodels[1]});   // one model
    CReadOnlyCombinedModel one_empty_model({submodels[0], submodels[1]});     // two models, one empty
    CReadOnlyCombinedModel two_models({submodels[1], submodels[2]});   // two models
    CReadOnlyCombinedModel three_models({submodels[1], submodels[2], submodels[3]});     // three models

    /// testing
    // NO MODEL
    QCOMPARE(no_model.childModelIndex(no_model.index(-1)), QModelIndex());
    QCOMPARE(no_model.childModelIndex(no_model.index(0)), QModelIndex());
    QCOMPARE(no_model.childModelIndex(no_model.index(8)), QModelIndex());
    QCOMPARE(no_model.childModelIndex(no_model.index(200)), QModelIndex());

    // EMPTY MODEL
    QCOMPARE(empty_model.childModelIndex(empty_model.index(-1)), QModelIndex());
    QCOMPARE(empty_model.childModelIndex(empty_model.index(0)), QModelIndex());
    QCOMPARE(empty_model.childModelIndex(empty_model.index(8)), QModelIndex());
    QCOMPARE(empty_model.childModelIndex(empty_model.index(200)), QModelIndex());

    // ONE MODEL
    QCOMPARE(one_model.childModelIndex(one_model.index(-1)), QModelIndex());
    QCOMPARE(one_model.childModelIndex(one_model.index(0)), submodels[1]->index(0));
    QCOMPARE(one_model.childModelIndex(one_model.index(8)), submodels[1]->index(8));
    QCOMPARE(one_model.childModelIndex(one_model.index(150)), submodels[1]->index(150));
    QCOMPARE(one_model.childModelIndex(one_model.index(160)), QModelIndex());

    // ONE EMPTY MODEL
    QCOMPARE(one_empty_model.childModelIndex(one_empty_model.index(-1)), QModelIndex());
    QCOMPARE(one_empty_model.childModelIndex(one_empty_model.index(0)), submodels[1]->index(0));
    QCOMPARE(one_empty_model.childModelIndex(one_empty_model.index(8)), submodels[1]->index(8));
    QCOMPARE(one_empty_model.childModelIndex(one_empty_model.index(150)), submodels[1]->index(150));
    QCOMPARE(one_empty_model.childModelIndex(one_empty_model.index(160)), QModelIndex());

    // TWO MODELS
    QCOMPARE(two_models.childModelIndex(two_models.index(-1)), QModelIndex());
    QCOMPARE(two_models.childModelIndex(two_models.index(0)), submodels[1]->index(0));
    QCOMPARE(two_models.childModelIndex(two_models.index(8)), submodels[1]->index(8));
    QCOMPARE(two_models.childModelIndex(two_models.index(150)), submodels[1]->index(150));
    QCOMPARE(two_models.childModelIndex(two_models.index(155)), submodels[2]->index(2));
    QCOMPARE(two_models.childModelIndex(two_models.index(160)), submodels[2]->index(7));
    QCOMPARE(two_models.childModelIndex(two_models.index(180)), QModelIndex());

    // THREE MODELS
    QCOMPARE(three_models.childModelIndex(three_models.index(-1)), QModelIndex());
    QCOMPARE(three_models.childModelIndex(three_models.index(0)), submodels[1]->index(0));
    QCOMPARE(three_models.childModelIndex(three_models.index(8)), submodels[1]->index(8));
    QCOMPARE(three_models.childModelIndex(three_models.index(150)), submodels[1]->index(150));
    QCOMPARE(three_models.childModelIndex(three_models.index(155)), submodels[2]->index(2));
    QCOMPARE(three_models.childModelIndex(three_models.index(160)), submodels[2]->index(7));
    QCOMPARE(three_models.childModelIndex(three_models.index(168)), submodels[3]->index(7));
    QCOMPARE(three_models.childModelIndex(three_models.index(1200)), submodels[3]->index(1039));
    QCOMPARE(three_models.childModelIndex(three_models.index(12500)), submodels[3]->index(12339));
    QCOMPARE(three_models.childModelIndex(three_models.index(12530)), QModelIndex());
}

void CReadOnlyCombinedModelTest::data()
{
    /// mocking
    CReadOnlyCombinedModel no_model({});    // no model attached
    CReadOnlyCombinedModel empty_model({submodels[0]});    // one empty model
    CReadOnlyCombinedModel one_model({submodels[1]});   // one model
    CReadOnlyCombinedModel one_empty_model({submodels[0], submodels[1]});     // two models, one empty
    CReadOnlyCombinedModel two_models({submodels[1], submodels[2]});   // two models
    CReadOnlyCombinedModel three_models({submodels[1], submodels[2], submodels[3]});     // three models

    /// testing
    // NO MODEL
    QCOMPARE(no_model.data(no_model.index(-1)), QVariant());
    QCOMPARE(no_model.data(no_model.index(0)), QVariant());
    QCOMPARE(no_model.data(no_model.index(8)), QVariant());
    QCOMPARE(no_model.data(no_model.index(200)), QVariant());

    // EMPTY MODEL
    QCOMPARE(empty_model.data(empty_model.index(-1)), QVariant());
    QCOMPARE(empty_model.data(empty_model.index(0)), QVariant());
    QCOMPARE(empty_model.data(empty_model.index(8)), QVariant());
    QCOMPARE(empty_model.data(empty_model.index(200)), QVariant());

    // ONE MODEL
    QCOMPARE(one_model.data(one_model.index(-1)), QVariant());
    QCOMPARE(one_model.data(one_model.index(0)), QVariant(submodels[1]->index(0)));
    QCOMPARE(one_model.data(one_model.index(8)), QVariant(submodels[1]->index(8)));
    QCOMPARE(one_model.data(one_model.index(150)), QVariant(submodels[1]->index(150)));
    QCOMPARE(one_model.data(one_model.index(160)), QVariant());

    // ONE EMPTY MODEL
    QCOMPARE(one_empty_model.data(one_empty_model.index(-1)), QVariant());
    QCOMPARE(one_empty_model.data(one_empty_model.index(0)), QVariant(submodels[1]->index(0)));
    QCOMPARE(one_empty_model.data(one_empty_model.index(8)), QVariant(submodels[1]->index(8)));
    QCOMPARE(one_empty_model.data(one_empty_model.index(150)), QVariant(submodels[1]->index(150)));
    QCOMPARE(one_empty_model.data(one_empty_model.index(160)), QVariant());

    // TWO MODELS
    QCOMPARE(two_models.data(two_models.index(-1)), QVariant());
    QCOMPARE(two_models.data(two_models.index(0)), QVariant(submodels[1]->index(0)));
    QCOMPARE(two_models.data(two_models.index(8)), QVariant(submodels[1]->index(8)));
    QCOMPARE(two_models.data(two_models.index(150)), QVariant(submodels[1]->index(150)));
    QCOMPARE(two_models.data(two_models.index(155)), QVariant(submodels[2]->index(2)));
    QCOMPARE(two_models.data(two_models.index(160)), QVariant(submodels[2]->index(7)));
    QCOMPARE(two_models.data(two_models.index(180)), QVariant());

    // THREE MODELS
    QCOMPARE(three_models.data(three_models.index(-1)), QVariant());
    QCOMPARE(three_models.data(three_models.index(0)), QVariant(submodels[1]->index(0)));
    QCOMPARE(three_models.data(three_models.index(8)), QVariant(submodels[1]->index(8)));
    QCOMPARE(three_models.data(three_models.index(150)), QVariant(submodels[1]->index(150)));
    QCOMPARE(three_models.data(three_models.index(155)), QVariant(submodels[2]->index(2)));
    QCOMPARE(three_models.data(three_models.index(160)), QVariant(submodels[2]->index(7)));
    QCOMPARE(three_models.data(three_models.index(168)), QVariant(submodels[3]->index(7)));
    QCOMPARE(three_models.data(three_models.index(1200)), QVariant(submodels[3]->index(1039)));
    QCOMPARE(three_models.data(three_models.index(12500)), QVariant(submodels[3]->index(12339)));
    QCOMPARE(three_models.data(three_models.index(12530)), QVariant());
}

/*
void CReadOnlyCombinedModelTest::index()
{
    /// mocking
    CReadOnlyCombinedModel no_model({});    // no model attached
    CReadOnlyCombinedModel empty_model({submodels[0]});    // one empty model
    CReadOnlyCombinedModel one_model({submodels[1]});   // one model
    CReadOnlyCombinedModel one_empty_model({submodels[0], submodels[1]});     // two models, one empty
    CReadOnlyCombinedModel two_models({submodels[1], submodels[2]});   // two models
    CReadOnlyCombinedModel three_models({submodels[1], submodels[2], submodels[3]});     // three models

    /// testing
    // NO MODEL
    QCOMPARE(no_model.index(-1), QModelIndex());
    QCOMPARE(no_model.index(0), QModelIndex());
    QCOMPARE(no_model.index(8), QModelIndex());
    QCOMPARE(no_model.index(200), QModelIndex());

    // EMPTY MODEL
    QCOMPARE(empty_model.index(-1), QModelIndex());
    QCOMPARE(empty_model.index(0), QModelIndex());
    QCOMPARE(empty_model.index(8), QModelIndex());
    QCOMPARE(empty_model.index(200), QModelIndex());

    // ONE MODEL
    QCOMPARE(one_model.index(-1), QModelIndex());
    QCOMPARE(one_model.index(0).internalPointer(), submodels[1]->index(0).internalPointer());
    QCOMPARE(one_model.index(8).internalPointer(), submodels[1]->index(8).internalPointer());
    QCOMPARE(one_model.index(150).internalPointer(), submodels[1]->index(150).internalPointer());
    QCOMPARE(one_model.index(160), QModelIndex());

    // ONE EMPTY MODEL
    QCOMPARE(one_empty_model.index(-1), QModelIndex());
    QCOMPARE(one_empty_model.index(0).internalPointer(), submodels[1]->index(0).internalPointer());
    QCOMPARE(one_empty_model.index(8).internalPointer(), submodels[1]->index(8).internalPointer());
    QCOMPARE(one_empty_model.index(150).internalPointer(), submodels[1]->index(150).internalPointer());
    QCOMPARE(one_empty_model.index(160), QModelIndex());

    // TWO MODELS
    QCOMPARE(two_models.index(-1), QModelIndex());
    QCOMPARE(two_models.index(0).internalPointer(), submodels[1]->index(0).internalPointer());
    QCOMPARE(two_models.index(8).internalPointer(), submodels[1]->index(8).internalPointer());
    QCOMPARE(two_models.index(150).internalPointer(), submodels[1]->index(150).internalPointer());
    QCOMPARE(two_models.index(155).internalPointer(), submodels[2]->index(2).internalPointer());
    QCOMPARE(two_models.index(160).internalPointer(), submodels[2]->index(7).internalPointer());
    QCOMPARE(two_models.index(180), QModelIndex());

    // THREE MODELS
    QCOMPARE(three_models.index(-1), QModelIndex());
    QCOMPARE(three_models.index(0).internalPointer(), submodels[1]->index(0).internalPointer());
    QCOMPARE(three_models.index(8).internalPointer(), submodels[1]->index(8).internalPointer());
    QCOMPARE(three_models.index(150).internalPointer(), submodels[1]->index(150).internalPointer());
    QCOMPARE(three_models.index(155).internalPointer(), submodels[2]->index(2).internalPointer());
    QCOMPARE(three_models.index(160).internalPointer(), submodels[2]->index(7).internalPointer());
    QCOMPARE(three_models.index(168).internalPointer(), submodels[3]->index(7).internalPointer());
    QCOMPARE(three_models.index(1200).internalPointer(), submodels[3]->index(1039).internalPointer());
    QCOMPARE(three_models.index(12500).internalPointer(), submodels[3]->index(12339).internalPointer());
    QCOMPARE(three_models.index(12530), QModelIndex());
}
*/

QTEST_APPLESS_MAIN(CReadOnlyCombinedModelTest)

#include "tst_creadonlycombinedmodeltest.moc"
