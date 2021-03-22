#include <QtGui>
#include <QtWidgets>
#include <QtPrintSupport>

#include <iostream>

#include "mainwindow.h"
#include "parser.h"
#include "filterwidget.h"
#include "planpromodel.h"
#include "graphicsscene.h"
#include "version.h"

MainWindow::MainWindow(const QString& dataFileName, QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("PlanPro Viewer"));
    setWindowIcon(QIcon(":/images/ppview.png"));

    readSettings();

    scene = new GraphicsScene();
    model = new PlanProModel();

    objectInfo = new QTextEdit();
    objectInfo->setReadOnly(true);
    filterWidget = new FilterWidget();
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText(tr("Object GUID"));
    searchButton = new QPushButton(tr("Search"));
    objectTreeView = new QTreeView();
    objectTreeView->setModel(model);
    objectTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    objectTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    favoriteList = new QListWidget();
    favoriteList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    referenceList = new QListWidget();
    referenceList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    filterWidget->readSettings();

    //view = new QGraphicsView(scene->getGraphicsScene(), this);
    view = new QGraphicsView(this);
    setCentralWidget(view);

    createActions();
    createMenus();
    createToolBars();
    createDockWindows();
    statusBar();

    setAcceptDrops(true);

    fileName = QString();
    selectionSource = SelectionSourceNotSelected;

    //connect(scene, SIGNAL(sendObjectInfo(QString)), this, SLOT(setTelegramInfo(QString)));
    //connect(scene->getGraphicsScene(), SIGNAL(selectionChanged()), this, SLOT(handleGraphicsSceneSelection()));
    //connect(filterWidget, SIGNAL(filterStateChanged(QString,bool)), scene, SLOT(changeFilterSettings(QString,bool)));
    connect(searchButton, SIGNAL(clicked()), this, SLOT(handleObjectSearchFromSearchWindow()));
    connect(objectTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(handleObjectListSelection(const QItemSelection &, const QItemSelection &)));
    connect(favoriteList, SIGNAL(itemSelectionChanged()), this, SLOT(handleFavoriteListSelection()));
    connect(referenceList, SIGNAL(itemSelectionChanged()), this, SLOT(handleReferenceListSelection()));

    addFileAct->setEnabled(false);
    saveFileAct->setEnabled(false);
    exportToPictureAct->setEnabled(false);
    exportToPdfAct->setEnabled(false);
    printFileAct->setEnabled(false);

    openNamedFile(dataFileName);
}

MainWindow::~MainWindow()
{

}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urllist(event->mimeData()->urls());
    if(urllist.count() > 0)
    {
        QUrl url = urllist.at(0); // only first entry
        QString s = url.toLocalFile();
        openNamedFile(s);
    }

    event->acceptProposedAction();
}

void MainWindow::closeEvent(QCloseEvent* ev)
{
    filterWidget->writeSettings();
    writeSettings();
    ev->accept();
}

void MainWindow::readSettings()
{
    QSettings settings;
    resize(settings.value("geometry/size", QSize(500, 450)).toSize());
    move(settings.value("geometry/pos", QPoint(200, 200)).toPoint());
    restoreState(settings.value("geometry/windowState").toByteArray());
    language = settings.value("preferences/language", "en").toString();
}


void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("geometry/size", size());
    settings.setValue("geometry/pos", pos());
    settings.setValue("geometry/windowState", saveState());
    settings.setValue("preferences/language", language);
}

void MainWindow::openFile()
{
    QString s = QFileDialog::getOpenFileName(
        this, tr("Open file"), QString(),
        tr("PlanPro XML files (*.ppxml);;All files (*.*)") );

    openNamedFile(s);
}

void MainWindow::addFile()
{
    QString s = QFileDialog::getOpenFileName(
        this, tr("Add file"), QString(),
        tr("PlanPro XML files (*.ppxml);;All files (*.*)") );

    if (!s.isEmpty())
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        bool success = model->addFile(s);
        Parser::createGraphicsScene(model,scene);
        if(!success)
        {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical(0, tr("File Reading Error"),
                            tr("File\n%1\ncould not be opened")
                            .arg(s));
        }
        QApplication::restoreOverrideCursor();
    }
}

void MainWindow::saveFile()
{
    QString defaultFileName = fileName;
    int index = defaultFileName.lastIndexOf(".");
    defaultFileName = defaultFileName.left(index);
    defaultFileName += ".ppxml";
    QString s = QFileDialog::getSaveFileName(
        this, tr("Save file"), defaultFileName,
        tr("PlanPro XML files (*.ppxml)"));

    if (!s.isEmpty())
    {
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
      bool success = model->saveFile(s);
      QApplication::restoreOverrideCursor();
      if(!success)
      {
        QMessageBox::critical(0, tr("File Saving Error"),
                              tr("File\n%1\ncould not be written")
                              .arg(s));
      }
    }
}

void MainWindow::openNamedFile(const QString& filename)
{
    if (!filename.isEmpty())
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        selectionSource = SelectionSourceNotSelected;

        //disconnect(scene, SIGNAL(sendObjectInfo(QString)), this, SLOT(setTelegramInfo(QString)));
        //disconnect(scene->getGraphicsScene(), SIGNAL(selectionChanged()), this, SLOT(handleGraphicsSceneSelection()));
        //disconnect(filterWidget, SIGNAL(filterStateChanged(QString,bool)), scene, SLOT(changeFilterSettings(QString,bool)));
        disconnect(objectTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(handleObjectListSelection(const QItemSelection &, const QItemSelection &)));
        //disconnect(favoriteList, SIGNAL(itemSelectionChanged()), this, SLOT(handleFavoriteListSelection()));
        //disconnect(referenceList, SIGNAL(itemSelectionChanged()), this, SLOT(handleReferenceListSelection()));

        favoriteList->clear();
        referenceList->clear();

        delete scene;
        delete model;
        scene = new GraphicsScene();
        model = new PlanProModel();

        bool success = model->loadFile(filename);
        objectTreeView->setModel(model);
        if(!success)
        {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical(0, tr("File Reading Error"),
                            tr("File\n%1\ncould not be opened")
                            .arg(filename));

            setWindowTitle(tr("PlanPro Viewer"));
            addFileAct->setEnabled(false);
            saveFileAct->setEnabled(false);
            exportToPictureAct->setEnabled(false);
            exportToPdfAct->setEnabled(false);
            printFileAct->setEnabled(false);
            fileName = QString();
        }
        else
        {
            Parser::createGraphicsScene(model,scene);
            scene->changeFilterSettings(filterWidget->getFilterState());

            QFileInfo fi(filename);
            setWindowTitle(fi.fileName() + tr(" - PlanPro Viewer"));
            addFileAct->setEnabled(true);
            saveFileAct->setEnabled(true);
            exportToPictureAct->setEnabled(true);
            exportToPdfAct->setEnabled(true);
            printFileAct->setEnabled(true);
            fileName = fi.fileName();

            //objectTreeView->setModel(model);
            view->setScene(scene->getGraphicsScene());
            view->show();

            connect(scene, SIGNAL(sendObjectInfo(QString)), this, SLOT(setTelegramInfo(QString)));
            connect(scene->getGraphicsScene(), SIGNAL(selectionChanged()), this, SLOT(handleGraphicsSceneSelection()));
            connect(filterWidget, SIGNAL(filterStateChanged(QString,bool)), scene, SLOT(changeFilterSettings(QString,bool)));
            connect(objectTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(handleObjectListSelection(const QItemSelection &, const QItemSelection &)));
            //connect(favoriteList, SIGNAL(itemSelectionChanged()), this, SLOT(handleFavoriteListSelection()));
            //connect(referenceList, SIGNAL(itemSelectionChanged()), this, SLOT(handleReferenceListSelection()));

            QApplication::restoreOverrideCursor();
        }
    }
}

void MainWindow::exportToPicture()
{
    QString defaultFileName = fileName;
    int index = defaultFileName.lastIndexOf(".");
    defaultFileName = defaultFileName.left(index);
    defaultFileName += ".png";
    QString s = QFileDialog::getSaveFileName(
        this, tr("Export to PNG"), defaultFileName,
        tr("Portable Network Graphics (*.png)"));

    if (!s.isEmpty())
    {
      QImage image(view->width(), view->height(), QImage::Format_RGB32);
      image.fill(QColor(Qt::white));
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
      QPainter painter(&image);
      view->render(&painter);
      image.save(s, "PNG");
      QApplication::restoreOverrideCursor();
    }
}

void MainWindow::exportToPdf()
{
    QString defaultPdfFileName = fileName;
    int index = defaultPdfFileName.lastIndexOf(".");
    defaultPdfFileName = defaultPdfFileName.left(index);
    defaultPdfFileName += ".pdf";
    QString s = QFileDialog::getSaveFileName(
        this, tr("Export to PDF"), defaultPdfFileName,
        tr("PDF files (*.pdf)"));

    if (!s.isEmpty())
    {
      QPrinter printer;
      printer.setOutputFormat(QPrinter::PdfFormat);
      printer.setOutputFileName(s);
      printer.setDocName(fileName);
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
      QPainter painter(&printer);
      view->render(&painter);
      QApplication::restoreOverrideCursor();
    }
}

void MainWindow::transformGraphicsView(int)
{
    QTransform transform;
    transform.scale(scaleSpinBox->value() / 100.0, scaleSpinBox->value() / 100.0);
    transform.rotate(rotateSpinBox->value());
    view->setTransform(transform);
}

void MainWindow::printFile()
{
    QPrinter printer;
    printer.setDocName(fileName);
    QPrintDialog dialog(&printer, this);
    if(dialog.exec() != QDialog::Accepted)
      return;

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QPainter painter(&printer);
    view->render(&painter);
    QApplication::restoreOverrideCursor();
}

void MainWindow::extractFile()
{
    QItemSelectionModel* selectionModel = objectTreeView->selectionModel();
    QModelIndexList selectedList = selectionModel->selectedIndexes();
    if(selectedList.count() != 1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Exactly 1 object must be selected"));
        return;
    }
    QModelIndex index = selectedList.at(0);
    QString filename = model->getBinaryFileName(index);
    if(filename.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Selected object contains no binary data"));
        return;
    }
    QString selectedFilename = QFileDialog::getSaveFileName(
        this, tr("Save file"), filename);

    if (!selectedFilename.isEmpty())
    {
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
      QByteArray data = model->extractBinaryFile(index);
      QFile file(selectedFilename);
      if (!file.open(QIODevice::WriteOnly))
      {
        QMessageBox::critical(0, tr("File Saving Error"),
                                tr("File\n%1\ncould not be written")
                                .arg(selectedFilename));
        QApplication::restoreOverrideCursor();
        return;
      }
      QDataStream out(&file);
      out << data;
      file.close();
      QApplication::restoreOverrideCursor();
    }
}

void MainWindow::measureDistance()
{
    QItemSelectionModel* selectionModel = objectTreeView->selectionModel();
    QModelIndexList selectedList = selectionModel->selectedIndexes();
    if(selectedList.count() != 2)
    {
        QMessageBox::critical(this, tr("Error"), tr("Exactly 2 objects must be selected"));
        return;
    }
    double result = model->calculateDistance(selectedList);
    if(result < -2)
    {
        QMessageBox::critical(this, tr("Error"), tr("At least one object is no Punkt_Objekt subtype"));
        return;
    }
    if(result < 0)
    {
        QMessageBox::information(this, tr("Distance"), tr("The selected objects have no direct connection"));
        return;
    }
    QMessageBox::information(this, tr("Distance"), tr("Distance: %1 m").arg(result));
}

void MainWindow::addToFavorites()
{
    QItemSelectionModel* selectionModel = objectTreeView->selectionModel();
    QModelIndexList selectedList = selectionModel->selectedIndexes();
    QModelIndex index;
    foreach (index, selectedList)
    {
        QModelIndex parentIndex = model->parent(index);
        while(parentIndex.isValid())
        {
            index = parentIndex;
            parentIndex = model->parent(index);
        }
        DomItem* domitem = static_cast<DomItem*>(index.internalPointer());
        QString name = domitem->node().nodeName();
        QString id = domitem->node().firstChildElement("Identitaet").firstChildElement("Wert").text();

        QListWidgetItem* newItem = new QListWidgetItem;
        newItem->setText(name + " [" + id + "]");
        favoriteList->addItem(newItem);
    }
}

void MainWindow::removeFromFavorites()
{
    QList<QListWidgetItem*> selectedList = favoriteList->selectedItems();
    for(int i = 0; i < selectedList.size(); i++)
    {
        QListWidgetItem* item = selectedList.at(i);
        int row = favoriteList->row(item);
        favoriteList->takeItem(row);
        delete item;
    }
}

void MainWindow::findReferencingObjects()
{
    QItemSelectionModel* selectionModel = objectTreeView->selectionModel();
    QModelIndexList selectedList = selectionModel->selectedIndexes();
    if(selectedList.count() != 1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Exactly 1 object must be selected"));
        return;
    }
    QModelIndex index = selectedList.at(0);
    QStringList resultlist = model->findReferencingObjects(index);
    referenceList->clear();
    referenceList->addItems(resultlist);
}

void MainWindow::setLanguage()
{
    QDir dir(QApplication::applicationDirPath());
    QStringList filters;
    filters << "ppview_*.qm";
    QStringList items = dir.entryList(filters, QDir::Files);
    items.replaceInStrings("ppview_", "");
    items.replaceInStrings(".qm", "");
    int index = items.indexOf(language);

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Set Language"),
                                         tr("Set Language (will be effective after restart):"),
                                         items, index, false, &ok);
    if (ok && !item.isEmpty())
      language = item;
}

void MainWindow::showHelp()
{
    QMessageBox::information(this, "Info", "TODO");
}

void MainWindow::about()
{
    QMessageBox::about(
        this, tr("About PlanPro Viewer"),
        tr("<h2>PlanPro Viewer</h2>"
           "Version: %1.%2.%3<br><br>"
           "<b>Copyright &copy; 2018-2019 Stefan Dillmann<br>"
           "Technische Universit&auml;t Darmstadt<br>"
           "Department of Computer Science<br>"
           "<a href=https://www.informatik.tu-darmstadt.de/se/homepage/index.en.jsp>Software Engineering Group</a><br><br>"
           "This program is released under the terms of the<br>"
           "<a href=https://www.gnu.org/licenses/gpl.html>GNU General Public License</a><br><br></b>"
           "<b>The work is supported by DB Netz AG in project<br>"
           "FormETCS, part of the Innovationsallianz<br>"
           "of TU Darmstadt and Deutsche Bahn AG</b>").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH));
}

void MainWindow::setTelegramInfo(const QString& text)
{
    objectInfo->setHtml(text);
}

void MainWindow::handleObjectSearchFromSearchWindow()
{
    QString searchedId = searchEdit->text();
    handleObjectSearch(searchedId);
}

void MainWindow::handleObjectSearchFromMenu()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Search"),
                                         tr("Enter the GUID of the searched object:"), QLineEdit::Normal,
                                         QString(), &ok);
    if (ok && !text.isEmpty())
        handleObjectSearch(text);
}

void MainWindow::centerObject()
{
    QList<QGraphicsItem*> itemlist = scene->getGraphicsScene()->selectedItems();
    for(int i = 0; i < itemlist.count(); i++)
    {
        QGraphicsItem* item = itemlist[i];
        view->centerOn(item);
    }
}

void MainWindow::handleObjectSearch(QString id)
{
    if(selectionSource != SelectionSourceNotSelected)
    {
        return;
    }
    if(selectionSource == SelectionSourceNotSelected)
    {
        selectionSource = SelectionSourceExternal;
    }

    QModelIndex index = model->getModelIndexById(id);
    if(!index.isValid())
    {
        QMessageBox::critical(0, tr("Search Error"),
                        tr("Object with GUID\n%1\ncould not be found")
                        .arg(id));

        selectionSource = SelectionSourceNotSelected;
        return;
    }
    QItemSelectionModel* selectionModel = objectTreeView->selectionModel();
    selectionModel->select(index, QItemSelectionModel::ClearAndSelect);
    objectTreeView->scrollTo(index, QAbstractItemView::EnsureVisible);

    QList<QGraphicsItem*> graphicsItemList = scene->getGraphicsScene()->items();
    for(int i = 0; i < graphicsItemList.count(); i++)
    {
        QGraphicsItem* item = graphicsItemList[i];
        if((item->data(GRAPHICSITEM_ID)).toString() == id)
        {
            item->setSelected(true);
            view->ensureVisible(item);
        }
        else
        {
            item->setSelected(false);
        }
    }

    for(int i = 0; i < favoriteList->count(); i++)
    {
        QListWidgetItem* item = favoriteList->item(i);
        if(item->text().contains(id))
        {
            item->setSelected(true);
            favoriteList->scrollToItem(item);
        }
        else
        {
            item->setSelected(false);
        }
    }

    for(int i = 0; i < referenceList->count(); i++)
    {
        QListWidgetItem* item = referenceList->item(i);
        if(item->text().contains(id))
        {
            item->setSelected(true);
            referenceList->scrollToItem(item);
        }
        else
        {
            item->setSelected(false);
        }
    }

    if(selectionSource == SelectionSourceExternal)
    {
        selectionSource = SelectionSourceNotSelected;
    }
}

void MainWindow::handleGraphicsSceneSelection()
{
    if(selectionSource != SelectionSourceNotSelected)
    {
        return;
    }
    if(selectionSource == SelectionSourceNotSelected)
    {
        selectionSource = SelectionSourceGraphicsView;
    }

    QList<QGraphicsItem*> itemlist = scene->getGraphicsScene()->selectedItems();
    QItemSelectionModel* selectionModel = objectTreeView->selectionModel();
    QItemSelection selection;
    favoriteList->clearSelection();
    referenceList->clearSelection();
    for(int i = 0; i < itemlist.count(); i++)
    {
        QString id = itemlist[i]->data(GRAPHICSITEM_ID).toString();
        QModelIndex index = model->getModelIndexById(id);
        if(index.isValid())
        {
            selection.select(index, index);
            objectTreeView->scrollTo(index, QAbstractItemView::EnsureVisible);
        }
        //std::cout << "handleGraphicsSceneSelection: " << qPrintable(id) << std::endl;

        for(int i = 0; i < favoriteList->count(); i++)
        {
            QListWidgetItem* item = favoriteList->item(i);
            if(item->text().contains(id))
            {
                item->setSelected(true);
                favoriteList->scrollToItem(item);
            }
        }

        for(int i = 0; i < referenceList->count(); i++)
        {
            QListWidgetItem* item = referenceList->item(i);
            if(item->text().contains(id))
            {
                item->setSelected(true);
                referenceList->scrollToItem(item);
            }
        }
    }
    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

    if(selectionSource == SelectionSourceGraphicsView)
    {
        selectionSource = SelectionSourceNotSelected;
    }
}

void MainWindow::handleFavoriteListSelection()
{
    if(selectionSource != SelectionSourceNotSelected)
    {
        return;
    }
    if(selectionSource == SelectionSourceNotSelected)
    {
        selectionSource = SelectionSourceFavoriteList;
    }

    QItemSelectionModel* selectionModel = objectTreeView->selectionModel();
    QItemSelection selection;
    referenceList->clearSelection();
    scene->getGraphicsScene()->clearSelection();

    QList<QListWidgetItem*> selectedList = favoriteList->selectedItems();
    for(int i = 0; i < selectedList.count(); i++)
    {
        QString itemtext = selectedList.at(i)->text();
        int charindex = itemtext.indexOf("[");
        QString id = itemtext.mid(charindex + 1, 36);

        QModelIndex index = model->getModelIndexById(id);
        if(index.isValid())
        {
            selection.select(index, index);
            objectTreeView->scrollTo(index, QAbstractItemView::EnsureVisible);
        }

        QGraphicsItem* graphicsitem = scene->getItemById(id);
        if(graphicsitem != NULL)
        {
            graphicsitem->setSelected(true);
            view->ensureVisible(graphicsitem);
        }

        for(int j = 0; j < referenceList->count(); j++)
        {
            QListWidgetItem* item = referenceList->item(j);
            if(item->text().contains(id))
            {
                item->setSelected(true);
                referenceList->scrollToItem(item);
            }
        }
    }
    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

    if(selectionSource == SelectionSourceFavoriteList)
    {
        selectionSource = SelectionSourceNotSelected;
    }
}

void MainWindow::handleReferenceListSelection()
{
    if(selectionSource != SelectionSourceNotSelected)
    {
        return;
    }
    if(selectionSource == SelectionSourceNotSelected)
    {
        selectionSource = SelectionSourceReferenceList;
    }

    QItemSelectionModel* selectionModel = objectTreeView->selectionModel();
    QItemSelection selection;
    favoriteList->clearSelection();
    scene->getGraphicsScene()->clearSelection();

    QList<QListWidgetItem*> selectedList = referenceList->selectedItems();
    for(int i = 0; i < selectedList.count(); i++)
    {
        QString itemtext = selectedList.at(i)->text();
        int charindex = itemtext.indexOf("[");
        QString id = itemtext.mid(charindex + 1, 36);

        QModelIndex index = model->getModelIndexById(id);
        if(index.isValid())
        {
            selection.select(index, index);
            objectTreeView->scrollTo(index, QAbstractItemView::EnsureVisible);
        }

        QGraphicsItem* graphicsitem = scene->getItemById(id);
        if(graphicsitem != NULL)
        {
            graphicsitem->setSelected(true);
            view->ensureVisible(graphicsitem);
        }

        for(int j = 0; j < favoriteList->count(); j++)
        {
            QListWidgetItem* item = favoriteList->item(j);
            if(item->text().contains(id))
            {
                item->setSelected(true);
                favoriteList->scrollToItem(item);
            }
        }
    }
    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

    if(selectionSource == SelectionSourceReferenceList)
    {
        selectionSource = SelectionSourceNotSelected;
    }
}

void MainWindow::handleObjectListSelection(const QItemSelection &selected, const QItemSelection &deselected)
{
    if(selectionSource != SelectionSourceNotSelected)
    {
        return;
    }
    if(selectionSource == SelectionSourceNotSelected)
    {
        selectionSource = SelectionSourceObjectList;
    }

    QModelIndex index;
    QModelIndexList items = selected.indexes();

    foreach (index, items)
    {
        QModelIndex parentIndex = model->parent(index);
        while(parentIndex.isValid())
        {
            index = parentIndex;
            parentIndex = model->parent(index);
        }
        DomItem* domitem = static_cast<DomItem*>(index.internalPointer());
        QString id = domitem->node().firstChildElement("Identitaet").firstChildElement("Wert").text();
        QGraphicsItem* graphicsitem = scene->getItemById(id);
        if(graphicsitem != NULL)
        {
            graphicsitem->setSelected(true);
            view->ensureVisible(graphicsitem);
        }
        //QString text = QString("Selected: (%1,%2)").arg(index.row()).arg(index.column());
        //std::cout << qPrintable(text) << std::endl;

        for(int i = 0; i < favoriteList->count(); i++)
        {
            QListWidgetItem* item = favoriteList->item(i);
            if(item->text().contains(id))
            {
                item->setSelected(true);
                favoriteList->scrollToItem(item);
            }
        }

        for(int i = 0; i < referenceList->count(); i++)
        {
            QListWidgetItem* item = referenceList->item(i);
            if(item->text().contains(id))
            {
                item->setSelected(true);
                referenceList->scrollToItem(item);
            }
        }
    }

    items = deselected.indexes();

    foreach (index, items)
    {
        QModelIndex parentIndex = model->parent(index);
        while(parentIndex.isValid())
        {
            index = parentIndex;
            parentIndex = model->parent(index);
        }
        DomItem* domitem = static_cast<DomItem*>(index.internalPointer());
        QString id = domitem->node().firstChildElement("Identitaet").firstChildElement("Wert").text();
        QGraphicsItem* graphicsitem = scene->getItemById(id);
        if(graphicsitem != NULL)
        {
            graphicsitem->setSelected(false);
        }

        //QString text = QString("Deselected: (%1,%2)").arg(index.row()).arg(index.column());
        //std::cout << qPrintable(text) << std::endl;

        for(int i = 0; i < favoriteList->count(); i++)
        {
            QListWidgetItem* item = favoriteList->item(i);
            if(item->text().contains(id))
            {
                item->setSelected(false);
            }
        }

        for(int i = 0; i < referenceList->count(); i++)
        {
            QListWidgetItem* item = referenceList->item(i);
            if(item->text().contains(id))
            {
                item->setSelected(false);
            }
        }
    }

    if(selectionSource == SelectionSourceObjectList)
    {
        selectionSource = SelectionSourceNotSelected;
    }
}

void MainWindow::createActions()
{
    openFileAct = new QAction(QIcon(":/images/fileopen.png"), tr("&Open file..."), this);
    openFileAct->setShortcut(tr("Ctrl+O"));
    openFileAct->setStatusTip(tr("Open a PlanPro XML file"));
    connect(openFileAct, SIGNAL(triggered()), this, SLOT(openFile()));

    addFileAct = new QAction(QIcon(":/images/fileopen.png"), tr("&Add file..."), this);
    addFileAct->setShortcut(tr("Ctrl+A"));
    addFileAct->setStatusTip(tr("Add the contents of a file to the existing data"));
    connect(addFileAct, SIGNAL(triggered()), this, SLOT(addFile()));

    saveFileAct = new QAction(QIcon(":/images/save.png"), tr("&Save file..."), this);
    saveFileAct->setShortcut(tr("Ctrl+S"));
    saveFileAct->setStatusTip(tr("Save the current view data to a PlanPro XML file"));
    connect(saveFileAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    exportToPictureAct = new QAction(QIcon(":/images/pdf.png"), tr("P&NG..."), this);
    exportToPictureAct->setStatusTip(tr("Export the visible area as PNG picture"));
    connect(exportToPictureAct, SIGNAL(triggered()), this, SLOT(exportToPicture()));

    exportToPdfAct = new QAction(QIcon(":/images/pdf.png"), tr("P&DF..."), this);
    exportToPdfAct->setStatusTip(tr("Export the visible area as PDF file"));
    connect(exportToPdfAct, SIGNAL(triggered()), this, SLOT(exportToPdf()));

    printFileAct = new QAction(QIcon(":/images/fileprint.png"), tr("&Print..."), this);
    printFileAct->setShortcut(tr("Ctrl+P"));
    printFileAct->setStatusTip(tr("Print the visible area"));
    connect(printFileAct, SIGNAL(triggered()), this, SLOT(printFile()));

    exitAct = new QAction(QIcon(":/images/exit.png"), tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Close the program"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    searchAct = new QAction(/*QIcon(":/images/contents.png"),*/ tr("&Search..."), this);
    searchAct->setShortcut(tr("Ctrl+F"));
    searchAct->setStatusTip(tr("Search an object based on its GUID"));
    connect(searchAct, SIGNAL(triggered()), this, SLOT(handleObjectSearchFromMenu()));

    centerAct = new QAction(/*QIcon(":/images/contents.png"),*/ tr("&Center object"), this);
    centerAct->setShortcut(tr("Alt+C"));
    centerAct->setStatusTip(tr("Center the selected object in the graphics view"));
    connect(centerAct, SIGNAL(triggered()), this, SLOT(centerObject()));

    selectAllFiltersAct = new QAction(/*QIcon(":/images/contents.png"),*/ tr("&Select all filter settings"), this);
    selectAllFiltersAct->setShortcut(tr("Shift+Alt+S"));
    selectAllFiltersAct->setStatusTip(tr("Select all objects in the object filter window"));
    connect(selectAllFiltersAct, SIGNAL(triggered()), filterWidget, SLOT(selectAllFilters()));

    deselectAllFiltersAct = new QAction(/*QIcon(":/images/contents.png"),*/ tr("D&eselect all filter settings"), this);
    deselectAllFiltersAct->setShortcut(tr("Shift+Alt+D"));
    deselectAllFiltersAct->setStatusTip(tr("Deselect all objects in the object filter window"));
    connect(deselectAllFiltersAct, SIGNAL(triggered()), filterWidget, SLOT(deselectAllFilters()));

    extractFileAct = new QAction(/*QIcon(":/images/contents.png"),*/ tr("&Extract file..."), this);
    extractFileAct->setShortcut(tr("Ctrl+E"));
    extractFileAct->setStatusTip(tr("Extract binary data from the selected object"));
    connect(extractFileAct, SIGNAL(triggered()), this, SLOT(extractFile()));

    measureDistanceAct = new QAction(/*QIcon(":/images/contents.png"),*/ tr("Measure &distance..."), this);
    measureDistanceAct->setShortcut(tr("Ctrl+D"));
    measureDistanceAct->setStatusTip(tr("Calculate the distance between two selected Punkt_Objekt subtypes"));
    connect(measureDistanceAct, SIGNAL(triggered()), this, SLOT(measureDistance()));

    findReferencingObjectsAct = new QAction(/*QIcon(":/images/contents.png"),*/ tr("Find &referencing objects"), this);
    findReferencingObjectsAct->setShortcut(tr("Ctrl+R"));
    findReferencingObjectsAct->setStatusTip(tr("Find all objects referencing the selected object"));
    connect(findReferencingObjectsAct, SIGNAL(triggered()), this, SLOT(findReferencingObjects()));

    addFavoriteAct = new QAction(/*QIcon(":/images/contents.png"),*/ tr("&Add Favorite"), this);
    addFavoriteAct->setShortcut(tr("Shift+Alt+A"));
    addFavoriteAct->setStatusTip(tr("Add the selected object to the favorite list"));
    connect(addFavoriteAct, SIGNAL(triggered()), this, SLOT(addToFavorites()));

    removeFavoriteAct = new QAction(/*QIcon(":/images/contents.png"),*/ tr("&Remove Favorite"), this);
    removeFavoriteAct->setShortcut(tr("Shift+Alt+R"));
    removeFavoriteAct->setStatusTip(tr("Remove the selected object from the favorite list"));
    connect(removeFavoriteAct, SIGNAL(triggered()), this, SLOT(removeFromFavorites()));

    clearFavoriteListAct = new QAction(/*QIcon(":/images/contents.png"),*/ tr("&Clear Favorite List"), this);
    clearFavoriteListAct->setShortcut(tr("Shift+Alt+C"));
    clearFavoriteListAct->setStatusTip(tr("Clear the favorite list"));
    connect(clearFavoriteListAct, SIGNAL(triggered()), favoriteList, SLOT(clear()));

    setLanguageAct = new QAction(/*QIcon(":/images/contents.png"),*/ tr("Set &Language..."), this);
    setLanguageAct->setShortcut(tr("Ctrl+L"));
    setLanguageAct->setStatusTip(tr("Set the language of the program"));
    connect(setLanguageAct, SIGNAL(triggered()), this, SLOT(setLanguage()));

    helpContentsAct = new QAction(QIcon(":/images/contents.png"), tr("&Help"), this);
    helpContentsAct->setShortcut(tr("F1"));
    helpContentsAct->setStatusTip(tr("Show program help"));
    connect(helpContentsAct, SIGNAL(triggered()), this, SLOT(showHelp()));

    aboutAct = new QAction(tr("&Info..."), this);
    aboutAct->setStatusTip(tr("Show program and version info"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAct);
    //fileMenu->addAction(addFileAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveFileAct);
    fileMenu->addSeparator();
    exportSubmenu = fileMenu->addMenu(tr("&Export"));
    exportSubmenu->addAction(exportToPictureAct);
    exportSubmenu->addAction(exportToPdfAct);
    fileMenu->addSeparator();
    fileMenu->addAction(printFileAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(searchAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewDockSubmenu = viewMenu->addMenu(tr("Show &Dock Windows"));
    viewMenu->addSeparator();
    viewMenu->addAction(centerAct);
    viewMenu->addSeparator();
    viewMenu->addAction(selectAllFiltersAct);
    viewMenu->addAction(deselectAllFiltersAct);

    objectMenu = menuBar()->addMenu(tr("&Object"));
    objectMenu->addAction(extractFileAct);
    objectMenu->addSeparator();
    objectMenu->addAction(measureDistanceAct);
    objectMenu->addAction(findReferencingObjectsAct);

    favoriteMenu = menuBar()->addMenu(tr("F&avorites"));
    favoriteMenu->addAction(addFavoriteAct);
    favoriteMenu->addAction(removeFavoriteAct);
    favoriteMenu->addSeparator();
    favoriteMenu->addAction(clearFavoriteListAct);

    extrasMenu = menuBar()->addMenu(tr("E&xtras"));
    extrasMenu->addAction(setLanguageAct);

    // Separator before help menu sets it in Motif style
    // at the right border (Convention)
    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpContentsAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
    toolBar = addToolBar(tr("ToolBar"));
    toolBar->setObjectName("ToolBar");
    toolBar->addAction(openFileAct);
    //toolBar->addAction(addFileAct);
    toolBar->addAction(saveFileAct);
    toolBar->addSeparator();
    //toolBar->addAction(exportToPictureAct);
    //toolBar->addAction(exportToPdfAct);
    toolBar->addAction(printFileAct);
    toolBar->addSeparator();
    QLabel* label1 = new QLabel(tr("Scale [%]"));
    toolBar->addWidget(label1);
    scaleSpinBox = new QSpinBox();
    scaleSpinBox->setMinimum(0);
    scaleSpinBox->setMaximum(1000);
    scaleSpinBox->setValue(100);
    toolBar->addWidget(scaleSpinBox);
    connect(scaleSpinBox, SIGNAL(valueChanged(int)), this, SLOT(transformGraphicsView(int)));
    QLabel* label2 = new QLabel(tr("Rotate [°]"));
    toolBar->addWidget(label2);
    rotateSpinBox = new QSpinBox();
    rotateSpinBox->setMinimum(-360);
    rotateSpinBox->setMaximum(360);
    rotateSpinBox->setValue(0);
    toolBar->addWidget(rotateSpinBox);
    connect(rotateSpinBox, SIGNAL(valueChanged(int)), this, SLOT(transformGraphicsView(int)));
}

void MainWindow::createDockWindows()
{
    QDockWidget *dock1 = new QDockWidget(tr("Object Info"), this);
    dock1->setObjectName("Object Info");
    dock1->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    dock1->setWidget(objectInfo);
    addDockWidget(Qt::BottomDockWidgetArea, dock1);
    viewDockSubmenu->addAction(dock1->toggleViewAction());

    QDockWidget *dock2 = new QDockWidget(tr("Object Filter"), this);
    dock2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock2->setObjectName("Object Filter");
    dock2->setWidget(filterWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock2);
    viewDockSubmenu->addAction(dock2->toggleViewAction());

    QDockWidget *dock3 = new QDockWidget(tr("Object Search"), this);
    dock3->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock3->setObjectName("Object Search");
    QWidget* searchWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(searchEdit);
    layout->addWidget(searchButton);
    layout->addStretch();
    searchWidget->setLayout(layout);
    dock3->setWidget(searchWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock3);
    viewDockSubmenu->addAction(dock3->toggleViewAction());

    QDockWidget *dock4 = new QDockWidget(tr("Object List"), this);
    dock4->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock4->setObjectName("Object List");
    dock4->setWidget(objectTreeView);
    addDockWidget(Qt::LeftDockWidgetArea, dock4);
    viewDockSubmenu->addAction(dock4->toggleViewAction());

    QDockWidget *dock5 = new QDockWidget(tr("Favorite List"), this);
    dock5->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock5->setObjectName("Favorite List");
    dock5->setWidget(favoriteList);
    addDockWidget(Qt::RightDockWidgetArea, dock5);
    viewDockSubmenu->addAction(dock5->toggleViewAction());

    QDockWidget *dock6 = new QDockWidget(tr("Reference List"), this);
    dock6->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock6->setObjectName("Reference List");
    dock6->setWidget(referenceList);
    addDockWidget(Qt::RightDockWidgetArea, dock6);
    viewDockSubmenu->addAction(dock6->toggleViewAction());
}
