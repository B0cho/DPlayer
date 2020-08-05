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
            Q_UNUSED(index);
            Q_UNUSED(role);
            return QVariant();
        }
    };

private Q_SLOTS:
    void init();
    void cleanup();
    void rowCount();
    void flags();
    void columnCount();
    void index();
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

    QCOMPARE(no_model.flags(QModelIndex()), Qt::NoItemFlags);

    QCOMPARE(empty_model.flags(QModelIndex()), Qt::NoItemFlags);
    /// To be finished
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

QTEST_APPLESS_MAIN(CReadOnlyCombinedModelTest)

#include "tst_creadonlycombinedmodeltest.moc"
