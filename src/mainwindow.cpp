#include <QtGui>
#include <QtWidgets>
#include <QtPrintSupport>

//#include <iostream>

#include "mainwindow.h"
//#include "makro.h"
#include "graphicsscenebuilder.h"
#include "filterwidget.h"
#include "planpromodel.h"
#include "planproxmldocument.h"
#include "graphicsscene.h"
#include "preferences.h"
#include "version.h"
#include "objectinfowidget.h"
#include "selectionmanager.h"
#include "objectlistmodel.h"
#include "anhang.h"

MainWindow::MainWindow(const QString& dataFileName, QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("PlanPro Viewer"));
    setWindowIcon(QIcon(":/images/ppview.png"));

    readSettings();

    document = new PlanProXmlDocument();

    model = new PlanProModel();
    model->setDocument(document);

    objectlistmodel = new ObjectListModel();
    objectlistmodel->setDocument(document);
    objectlistmodel->changeCategory(QString());
    objectlistmodel->changeViewMode(ViewModeStateEnd);

    objectTreeView = new QTreeView();
    objectTreeView->setModel(model);
    objectTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    objectTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);

    categoryComboBox = new QComboBox();
    objectListView = new QTreeView();
    objectListView->setModel(objectlistmodel);
    objectListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    objectListView->setSelectionBehavior(QAbstractItemView::SelectRows);

    objectInfo = new ObjectInfoWidget();

    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText(tr("Object GUID"));
    searchButton = new QPushButton(tr("Search"));

    favoriteList = new QListWidget();
    favoriteList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    referenceList = new QTreeWidget();
    referenceList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    referenceList->setColumnCount(2);
    QStringList headerLabels;
    headerLabels << tr("Type") << tr("ID");
    referenceList->setHeaderLabels(headerLabels);

    filterWidget = new FilterWidget();
    filterWidget->readSettings();

    scene = new GraphicsScene();
    scene->changeViewMode(ViewModeStateEnd);

    view = new QGraphicsView(scene, this);
    view->setAcceptDrops(false);

    selectionManager = new SelectionManager();
    selectionManager->setDocument(document);
    selectionManager->setScene(scene);
    selectionManager->setDocumentTreeView(objectTreeView);
    selectionManager->setObjectListView(objectListView);
    selectionManager->setFavoriteListWidget(favoriteList);
    selectionManager->setReferenceListWidget(referenceList);

    setCentralWidget(view);

    createActions();
    createMenus();
    createToolBars();
    createDockWindows();
    statusBar();

    setAcceptDrops(true);

    connect(filterWidget, SIGNAL(filterStateChanged(QString,bool)), scene, SLOT(changeFilterSettings(QString,bool)));
    connect(searchButton, SIGNAL(clicked()), this, SLOT(handleObjectSearchFromSearchWindow()));
    connect(categoryComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCategory()));
    connect(selectionManager, SIGNAL(selectionChanged(QList<DomItem*>)), objectInfo, SLOT(setInfoText(QList<DomItem*>)));
    connect(document, SIGNAL(dataChanged()), model, SLOT(modelChanged()));
    connect(document, SIGNAL(dataChanged()), objectlistmodel, SLOT(modelChanged()));
    connect(objectInfo, SIGNAL(uuidClicked(QString)), selectionManager, SLOT(selectItem(QString)));

    saveFileAct->setEnabled(false);
    exportToPictureAct->setEnabled(false);
    exportToPdfAct->setEnabled(false);
    printFileAct->setEnabled(false);

    openNamedFile(dataFileName);
}

MainWindow::~MainWindow()
{
}

void MainWindow::dragEnterEvent(QDragEnterEvent* ev)
{
    if (ev->mimeData()->hasUrls())
        ev->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent* ev)
{
    QList<QUrl> urllist(ev->mimeData()->urls());
    if(urllist.count() > 0)
    {
        QUrl url = urllist.at(0); // only first entry
        QString s = url.toLocalFile();
        openNamedFile(s);
    }
    ev->acceptProposedAction();
}

void MainWindow::closeEvent(QCloseEvent* ev)
{
    filterWidget->writeSettings();
    writeSettings();
    ev->accept();
}

void MainWindow::readSettings()
{
    Preferences* prefs = Preferences::getInstance();
    resize(prefs->getSize());
    move(prefs->getPos());
    restoreState(prefs->getWindowState());
}


void MainWindow::writeSettings()
{
    Preferences* prefs = Preferences::getInstance();
    prefs->setSize(size());
    prefs->setPos(pos());
    prefs->setWindowState(saveState());

    prefs->writeSettings();
}

void MainWindow::openFile()
{
    QString s = QFileDialog::getOpenFileName(
        this, tr("Open file"), QDir::homePath(),
        tr("PlanPro XML files (*.ppxml);;All files (*.*)") );

    openNamedFile(s);
}

void MainWindow::saveFile()
{
    QString defaultFileName = document->getFileName();
    int index = defaultFileName.lastIndexOf(".");
    defaultFileName = defaultFileName.left(index);
    defaultFileName += ".ppxml";
    QString s = QFileDialog::getSaveFileName(
        this, tr("Save file"), defaultFileName,
        tr("PlanPro XML files (*.ppxml)"));

    if (!s.isEmpty())
    {
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
      document->updateHeader("PlanPro Viewer", QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH));
      model->modelChanged(); // because of updated header
      bool success = document->saveFile(s);
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
        bool success = document->loadFile(filename);
        if(!success)
        {
            scene->clear();
            categoryComboBox->clear();
            setWindowTitle(tr("PlanPro Viewer"));
            saveFileAct->setEnabled(false);
            exportToPictureAct->setEnabled(false);
            exportToPdfAct->setEnabled(false);
            printFileAct->setEnabled(false);

            QApplication::restoreOverrideCursor();
            QMessageBox::critical(0, tr("File Reading Error"),
                            tr("File\n%1\ncould not be opened")
                            .arg(filename));
        }
        else
        {
            GraphicsSceneBuilder builder(document, scene);
            builder.createGraphicsScene();
            scene->changeFilterSettings(filterWidget->getFilterState());
            categoryComboBox->clear();
            QStringList categoryList = document->getCategoryList();
            if(!categoryList.isEmpty())
            {
                categoryComboBox->addItem(tr("(all)"));
                categoryComboBox->addItems(categoryList);
            }

            QFileInfo fi(filename);
            setWindowTitle(fi.fileName() + tr(" - PlanPro Viewer"));
            saveFileAct->setEnabled(true);
            exportToPictureAct->setEnabled(true);
            exportToPdfAct->setEnabled(true);
            printFileAct->setEnabled(true);

            //view->show();

            QApplication::restoreOverrideCursor();
        }
        favoriteList->clear();
        referenceList->clear();
        selectionManager->clearSelection();
    }
}

void MainWindow::exportToPicture()
{
    QString defaultFileName = document->getFileName();
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
    QString defaultPdfFileName = document->getFileName();
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
      QFileInfo fi(document->getFileName());
      printer.setDocName(fi.fileName());
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
    QFileInfo fi(document->getFileName());
    printer.setDocName(fi.fileName());
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
    DomItem* domItem = static_cast<DomItem*>(index.internalPointer());
    Anhang anhang(domItem);
    if(!anhang.isAnhang())
    {
        QMessageBox::critical(this, tr("Error"), tr("Selected object contains no binary data"));
        return;
    }
    QString filename = anhang.getDateiname() + "." + anhang.getDateityp();
    QFileInfo fi(document->getFileName());
    QDir path = fi.dir();
    QString filePathString = path.filePath(filename);

    QString selectedFilename = QFileDialog::getSaveFileName(
        this, tr("Save file"), filePathString);

    if (!selectedFilename.isEmpty())
    {
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
      QByteArray data = anhang.getDaten();
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
//    QItemSelectionModel* selectionModel = objectTreeView->selectionModel();
//    QModelIndexList selectedList = selectionModel->selectedIndexes();
//    if(selectedList.count() != 2)
//    {
//        QMessageBox::critical(this, tr("Error"), tr("Exactly 2 objects must be selected"));
//        return;
//    }
//    double result = model->calculateDistance(selectedList);
//    if(result < -2)
//    {
//        QMessageBox::critical(this, tr("Error"), tr("At least one object is no Punkt_Objekt subtype"));
//        return;
//    }
//    if(result < 0)
//    {
//        QMessageBox::information(this, tr("Distance"), tr("The selected objects have no direct connection"));
//        return;
//    }
//    QMessageBox::information(this, tr("Distance"), tr("Distance: %1 m").arg(result));
}

void MainWindow::addToFavorites()
{
    QList<DomItem*> selectedItemList = selectionManager->getSelectedItems();
    for(int i = 0; i < selectedItemList.count(); ++i)
    {
      DomItem* item = selectedItemList.at(i);
      QString name = item->getName();
      QString id = item->getFirstValueAtPath("Identitaet/Wert");
      QListWidgetItem* newItem = new QListWidgetItem;
      newItem->setText(name + " [" + id + "]");
      favoriteList->addItem(newItem);
    }
}

void MainWindow::removeFromFavorites()
{
    QList<QListWidgetItem*> selectedList = favoriteList->selectedItems();
    for(int i = 0; i < selectedList.size(); ++i)
    {
        QListWidgetItem* item = selectedList.at(i);
        int row = favoriteList->row(item);
        favoriteList->takeItem(row);
        delete item;
    }
}

void MainWindow::findReferencingObjects()
{
    QList<DomItem*> selectedItemList = selectionManager->getSelectedItems();
    if(selectedItemList.count() != 1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Exactly 1 object must be selected"));
        return;
    }
    referenceList->clear();
    DomItem* item = selectedItemList.at(0);
    QString name = item->getName();
    QString id = item->getFirstValueAtPath("Identitaet/Wert");
    QTreeWidgetItem* tlItem = new QTreeWidgetItem();
    tlItem->setText(0, name);
    tlItem->setText(1, id);
    referenceList->addTopLevelItem(tlItem);
    createReferenceListRec(item, tlItem, 1);
    referenceList->expandItem(tlItem);
}

void MainWindow::createReferenceListRec(DomItem *item, QTreeWidgetItem* parent, int depth)
{
    if(depth > 10)
    {
        return;
    }
    QList<DomItem*> refList = document->findDependentObjects(item);
    for(int i = 0; i < refList.count(); ++i)
    {
        DomItem* currentItem = refList.at(i);
        QString name = currentItem->getName();
        QString id = currentItem->getFirstValueAtPath("Identitaet/Wert");
        QTreeWidgetItem* newItem = new QTreeWidgetItem();
        newItem->setText(0, name);
        newItem->setText(1, id);
        parent->addChild(newItem);
        createReferenceListRec(currentItem, newItem, depth + 1);
        referenceList->expandItem(newItem);
    }
}

void MainWindow::setLanguage()
{
    QDir dir(QApplication::applicationDirPath());
    QStringList filters;
    filters << "ppview_*.qm";
    QStringList items = dir.entryList(filters, QDir::Files);
    items.replaceInStrings("ppview_", "");
    items.replaceInStrings(".qm", "");
    QString language = Preferences::getInstance()->getLanguage();
    int index = items.indexOf(language);

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Set Language"),
                                         tr("Set Language (will be effective after restart):"),
                                         items, index, false, &ok);
    if (ok && !item.isEmpty())
    {
        Preferences::getInstance()->setLanguage(item);
    }
}

void MainWindow::showDocumentInfo()
{
    QString text = "No file loaded";
    QString timestamp = document->getTimestamp().toString();
    QString toolname = document->getToolName();
    QString toolversion = document->getToolVersion();
    QString remark = document->getRemark();
    text = "Timestamp: " + timestamp + "\nTool: " + toolname + ", Version " + toolversion + "\nRemark: " + remark;
    QMessageBox::information(this, "Info", text);
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
           "<b>Copyright &copy; 2017-2021<br>"
           "Technische Universit&auml;t Darmstadt<br><br>"
           "This program is released under the terms of the<br>"
           "BSD 3-Clause License<br><br></b>"
           "<b>The work was supported by DB Netz AG in<br>"
           "Project FormETCS</b>").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH));
}

void MainWindow::handleObjectSearchFromSearchWindow()
{
    QString searchedId = searchEdit->text();
    selectionManager->selectItem(searchedId);
}

void MainWindow::handleObjectSearchFromMenu()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Search"),
                                         tr("Enter the GUID of the searched object:"), QLineEdit::Normal,
                                         QString(), &ok);
    if (ok && !text.isEmpty())
        selectionManager->selectItem(text);
}

void MainWindow::centerObject()
{
    QList<QGraphicsItem*> itemlist = scene->selectedItems();
    for(int i = 0; i < itemlist.count(); ++i)
    {
        QGraphicsItem* item = itemlist.at(i);
        view->centerOn(item);
    }
}

void MainWindow::switchViewMode()
{
    ViewMode viewMode = ViewModeStateEnd;
    if(startViewModeAct->isChecked())
    {
        viewMode = ViewModeStateStart;
        qDebug("start state switch");
    }
    else if(endViewModeAct->isChecked())
    {
        viewMode = ViewModeStateEnd;
        qDebug("end state switch");
    }
    else if(comparisonViewModeAct->isChecked())
    {
        viewMode = ViewModeStateComparison;
        qDebug("combined state switch");
    }
    objectlistmodel->changeViewMode(viewMode);
    scene->changeViewMode(viewMode);
    selectionManager->changeViewMode(viewMode);
}

void MainWindow::changeCategory()
{
    QString category = QString();
    if(categoryComboBox->currentIndex() > 0)
    {
        category = categoryComboBox->currentText();
    }
    objectlistmodel->changeCategory(category);
    selectionManager->clearSelection();
}

void MainWindow::createActions()
{
    openFileAct = new QAction(QIcon(":/images/fileopen.png"), tr("&Open file..."), this);
    openFileAct->setShortcut(tr("Ctrl+O"));
    openFileAct->setStatusTip(tr("Open a PlanPro XML file"));
    connect(openFileAct, SIGNAL(triggered()), this, SLOT(openFile()));

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

    startViewModeAct = new QAction(/*QIcon(":/images/contents.png"),*/ tr("&Start State"), this);
    startViewModeAct->setShortcut(tr("F5"));
    startViewModeAct->setStatusTip(tr("Show the start state of the planning"));
    startViewModeAct->setCheckable(true);
    startViewModeAct->setChecked(false);
    connect(startViewModeAct, SIGNAL(triggered()), this, SLOT(switchViewMode()));

    endViewModeAct = new QAction(/*QIcon(":/images/contents.png"),*/ tr("&End State"), this);
    endViewModeAct->setShortcut(tr("F6"));
    endViewModeAct->setStatusTip(tr("Show the end state of the planning"));
    endViewModeAct->setCheckable(true);
    endViewModeAct->setChecked(true);
    connect(endViewModeAct, SIGNAL(triggered()), this, SLOT(switchViewMode()));

    comparisonViewModeAct = new QAction(/*QIcon(":/images/contents.png"),*/ tr("Start/End &Comparison"), this);
    comparisonViewModeAct->setShortcut(tr("F7"));
    comparisonViewModeAct->setStatusTip(tr("Show the comparison of the start and end state of the planning"));
    comparisonViewModeAct->setCheckable(true);
    comparisonViewModeAct->setChecked(false);
    connect(comparisonViewModeAct, SIGNAL(triggered()), this, SLOT(switchViewMode()));

    viewModeActGroup = new QActionGroup(this);
    viewModeActGroup->addAction(startViewModeAct);
    viewModeActGroup->addAction(endViewModeAct);
    viewModeActGroup->addAction(comparisonViewModeAct);

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
    viewMenu->addAction(startViewModeAct);
    viewMenu->addAction(endViewModeAct);
    viewMenu->addAction(comparisonViewModeAct);
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

    QDockWidget *dock4 = new QDockWidget(tr("Document Structure"), this);
    dock4->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock4->setObjectName("Document Structure");
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

    QDockWidget *dock7 = new QDockWidget(tr("Object List"), this);
    dock7->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock7->setObjectName("Object List");
    QWidget* objectListWidget = new QWidget();
    QVBoxLayout* layout2 = new QVBoxLayout;
    layout2->addWidget(categoryComboBox);
    layout2->addWidget(objectListView);
    objectListWidget->setLayout(layout2);
    dock7->setWidget(objectListWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock7);
    viewDockSubmenu->addAction(dock7->toggleViewAction());
}
