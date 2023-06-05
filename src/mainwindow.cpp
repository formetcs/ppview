#include <QtGui>
#include <QtWidgets>
#include <QtPrintSupport>

#include "mainwindow.h"
#include "global.h"
#include "graphicsscenebuilder.h"
#include "filterwidget.h"
#include "documenttreemodel.h"
#include "planproxmldocument.h"
#include "graphicsscene.h"
#include "preferences.h"
#include "version.h"
#include "objectinfowidget.h"
#include "selectionmanager.h"
#include "objectlistmodel.h"
#include "anhang.h"
#include "planprograph.h"

MainWindow::MainWindow(const QString& dataFileName, QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(APPLICATION_NAME);
    setWindowIcon(QIcon(":/images/ppview.png"));

    readSettings();

    document = new PlanProXmlDocument();
    
    doctreemodel = new DocumentTreeModel();
    doctreemodel->setDocument(document);

    objectlistmodel = new ObjectListModel();
    objectlistmodel->setDocument(document);
    objectlistmodel->changeCategory(QString());
    objectlistmodel->changeViewMode(ViewModeStateEnd);

    documentTreeView = new QTreeView();
    documentTreeView->setModel(doctreemodel);
    documentTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    documentTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);

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
    selectionManager->setDocumentTreeView(documentTreeView);
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
    connect(document, SIGNAL(dataChanged()), doctreemodel, SLOT(modelChanged()));
    connect(document, SIGNAL(dataChanged()), objectlistmodel, SLOT(modelChanged()));
    connect(objectInfo, SIGNAL(uuidClicked(QString)), selectionManager, SLOT(selectItem(QString)));

    enableActions();
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
        if(okToContinue())
        {
            openNamedFile(s);
        }
    }
    ev->acceptProposedAction();
}

void MainWindow::closeEvent(QCloseEvent* ev)
{
    if(okToContinue())
    {
        filterWidget->writeSettings();
        writeSettings();
        ev->accept();
    }
    else
    {
        ev->ignore();
    }
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

bool MainWindow::okToContinue()
{
    if(isWindowModified())
    {
        int r = QMessageBox::warning(this, APPLICATION_NAME,
                                    tr("The document has been modified.\nDo you want to save your changes?"),
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                    QMessageBox::Yes);
        if(r == QMessageBox::Yes)
        {
            return saveFile();
        }
        else if(r == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

void MainWindow::openFile()
{
    if(okToContinue())
    {
        QString s = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath(),
                                                 tr("PlanPro XML files (*.ppxml);;All files (*.*)") );

        openNamedFile(s);
    }
}

void MainWindow::openNamedFile(const QString& filename)
{
    if (!filename.isEmpty())
    {
        selectionManager->clearSelection();
        scene->clear();
        categoryComboBox->clear();
        favoriteList->clear();
        referenceList->clear();
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        doctreemodel->modelAboutToBeChanged();
        objectlistmodel->modelAboutToBeChanged();
        bool success = document->loadFile(filename);
        QApplication::restoreOverrideCursor();
        if(!success)
        {
            setWindowTitle(APPLICATION_NAME);
            QMessageBox::critical(0, tr("File Reading Error"),
                                tr("File\n%1\ncould not be opened").arg(filename));
        }
        else
        {
            GraphicsSceneBuilder builder(document, scene);
            if(!builder.createGraphicsScene())
            {
                document->clear();
                setWindowTitle(APPLICATION_NAME);
            }
            else
            {
                scene->changeFilterSettings(filterWidget->getFilterState());
                QStringList categoryList = document->getCategoryList();
                if(!categoryList.isEmpty())
                {
                    categoryComboBox->addItem(tr("(all)"));
                    categoryComboBox->addItems(categoryList);
                }

                QFileInfo fi(filename);
                setWindowTitle(QString("%1[*] - %2").arg(fi.fileName(), APPLICATION_NAME));
            }
        }
        doctreemodel->modelChanged();
        objectlistmodel->modelChanged();
        setWindowModified(false);
        enableActions();
    }
}

bool MainWindow::saveFile()
{
    return saveNamedFile(document->getFileName());
}

bool MainWindow::saveAs()
{
    QString defaultFileName = document->getFileName();
    int index = defaultFileName.lastIndexOf(".");
    defaultFileName = defaultFileName.left(index);
    defaultFileName += ".ppxml";
    QString s = QFileDialog::getSaveFileName(this, tr("Save file"), defaultFileName,
                                tr("PlanPro XML files (*.ppxml)"));

    return saveNamedFile(s);
}

bool MainWindow::saveNamedFile(const QString& filename)
{
    if (filename.isEmpty())
    {
        return false;
    }
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    document->updateHeader(APPLICATION_NAME, QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH));
    doctreemodel->modelChanged(); // because of updated header
    bool success = document->saveFile(filename);
    QApplication::restoreOverrideCursor();
    if(!success)
    {
        QMessageBox::critical(0, tr("File Saving Error"),
                              tr("File\n%1\ncould not be written").arg(filename));
        return false;
    }
    QFileInfo fi(filename);
    setWindowTitle(QString("%1[*] - %2").arg(fi.fileName(), APPLICATION_NAME));
    setWindowModified(false);
    enableActions();
    return true;
}

void MainWindow::closeFile()
{
    if(okToContinue())
    {
        selectionManager->clearSelection();
        scene->clear();
        categoryComboBox->clear();
        favoriteList->clear();
        referenceList->clear();
        document->clear();
        setWindowTitle(APPLICATION_NAME);
        doctreemodel->modelChanged();
        objectlistmodel->modelChanged();
        setWindowModified(false);
        enableActions();
    }
}

void MainWindow::exportToPicture()
{
    QString defaultFileName = document->getFileName();
    int index = defaultFileName.lastIndexOf(".");
    defaultFileName = defaultFileName.left(index);
    defaultFileName += ".png";
    QString s = QFileDialog::getSaveFileName(this, tr("Export to PNG"), defaultFileName,
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
    QString s = QFileDialog::getSaveFileName(this, tr("Export to PDF"), defaultPdfFileName,
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

void MainWindow::transformGraphicsView()
{
    QTransform transform;
    transform.scale(scaleSpinBox->value() / 100.0, scaleSpinBox->value() / 100.0);
    transform.rotate(rotateSpinBox->value());
    view->setTransform(transform);
}

void MainWindow::zoomIn()
{
    int zoomStep = Preferences::getInstance()->getZoomStep();
    int val = scaleSpinBox->value() + zoomStep;
    scaleSpinBox->setValue(val);
}

void MainWindow::zoomOut()
{
    int zoomStep = Preferences::getInstance()->getZoomStep();
    int minZoom = Preferences::getInstance()->getMinZoom();
    int val = scaleSpinBox->value() - zoomStep;
    if(val < minZoom)
    {
        val = minZoom;
    }
    scaleSpinBox->setValue(val);
}

void MainWindow::rotateLeft()
{
    int rotateStep = Preferences::getInstance()->getRotateStep();
    int val = rotateSpinBox->value() - rotateStep;
    rotateSpinBox->setValue(val);
}

void MainWindow::rotateRight()
{
    int rotateStep = Preferences::getInstance()->getRotateStep();
    int val = rotateSpinBox->value() + rotateStep;
    rotateSpinBox->setValue(val);
}

void MainWindow::resetView()
{
    scaleSpinBox->setValue(100);
    rotateSpinBox->setValue(0);
}

void MainWindow::extractFile()
{
    QItemSelectionModel* selectionModel = documentTreeView->selectionModel();
    QModelIndexList selectedList = selectionModel->selectedIndexes();
    if(selectedList.count() != 1)
    {
        QMessageBox::information(this, tr("Error"), tr("Exactly 1 object must be selected"));
        return;
    }
    QModelIndex index = selectedList.at(0);
    DomItem* domItem = static_cast<DomItem*>(index.internalPointer());
    Anhang anhang(domItem);
    if(!anhang.isAnhang())
    {
        QMessageBox::information(this, tr("Error"), tr("Selected object contains no binary data"));
        return;
    }
    QString filename = anhang.getDateiname() + "." + anhang.getDateityp();
    QFileInfo fi(document->getFileName());
    QDir path = fi.dir();
    QString filePathString = path.filePath(filename);

    QString selectedFilename = QFileDialog::getSaveFileName(this, tr("Save file"), filePathString);

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
    QList<DomItem*> selectedItemList = selectionManager->getSelectedItems();
    if(selectedItemList.count() != 2)
    {
        QMessageBox::information(this, tr("Error"), tr("Exactly 2 objects must be selected"));
        return;
    }
    PlanProGraph graph(document);
    DomItem* item1 = selectedItemList.at(0);
    DomItem* item2 = selectedItemList.at(1);
    int result = graph.calculateDistance(item1, item2);
    if(result < -2)
    {
        QMessageBox::information(this, tr("Error"), tr("At least one object is no Punkt_Objekt subtype"));
        return;
    }
    if(result < 0)
    {
        QMessageBox::information(this, tr("Distance"), tr("The selected objects have no direct connection"));
        return;
    }
    double doubleDistance = result / 1000.0;
    QMessageBox::information(this, tr("Distance"), tr("Distance: %1 m").arg(doubleDistance, 0, 'f', 3));
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
        QMessageBox::information(this, tr("Error"), tr("Exactly 1 object must be selected"));
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
    int maxDepth = Preferences::getInstance()->getMaxRecursionDepth();
    if(depth > maxDepth)
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
    QString text = tr("No file loaded");
    if(document->getDocumentType() != PlanProDocument::DocumentTypeInvalid)
    {
        QString doctype = tr("State");
        if(document->getDocumentType() == PlanProDocument::DocumentTypePlanning)
        {
            doctype = tr("Planning");
        }
        QString timestamp = document->getTimestamp().toString();
        QString toolname = document->getToolName();
        QString toolversion = document->getToolVersion();
        QString remark = document->getRemark();
        text = tr("Document Type: %1\n\nTimestamp: %2\nTool: %3, Version %4\n\n%5").arg(doctype, timestamp, toolname, toolversion, remark);
    }
    QMessageBox::information(this, tr("Document Information"), text);
}

void MainWindow::editRemark()
{
    QString oldRemark = document->getRemark();
    bool ok;
    QString newRemark = QInputDialog::getMultiLineText(this, tr("Edit remark"),
                                            tr("Enter the remark for the PlanPro file:"), oldRemark, &ok);
    if(ok && newRemark != oldRemark)
    {
        document->setRemark(newRemark);
        doctreemodel->modelChanged(); // because of updated header
        setWindowModified(true);
        enableActions();
    }
}

void MainWindow::showHelp()
{
    QMessageBox::information(this, "Info", "TODO");
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About PlanPro Viewer"),
            tr("<h2>PlanPro Viewer %1.%2.%3</h2>"
            "Supports PlanPro Version %4.%5.%6<br><br>"
            "Copyright &copy; 2017-2023 The FormETCS Project.<br>All rights reserved.<br><br>"
            "This program is released under the terms of the<br>"
            "GNU General Public License.")
            .arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH).arg(PLANPRO_MAJOR).arg(PLANPRO_MINOR).arg(PLANPRO_PATCH));
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
    }
    else if(endViewModeAct->isChecked())
    {
        viewMode = ViewModeStateEnd;
    }
    else if(comparisonViewModeAct->isChecked())
    {
        viewMode = ViewModeStateComparison;
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
    openFileAct = new QAction(tr("&Open file..."), this);
    openFileAct->setIcon(QIcon(":/images/fileopen.png"));
    openFileAct->setShortcut(QKeySequence::Open);
    openFileAct->setStatusTip(tr("Open a PlanPro XML file"));
    connect(openFileAct, SIGNAL(triggered()), this, SLOT(openFile()));

    saveFileAct = new QAction(tr("&Save file"), this);
    saveFileAct->setIcon(QIcon(":/images/save.png"));
    saveFileAct->setShortcut(QKeySequence::Save);
    saveFileAct->setStatusTip(tr("Save the current PlanPro XML file"));
    connect(saveFileAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAct = new QAction(tr("Save &as..."), this);
    saveAsAct->setIcon(QIcon(":/images/save.png"));
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the current PlanPro data to a different file"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    closeFileAct = new QAction(tr("&Close file"), this);
    closeFileAct->setShortcut(QKeySequence::Close);
    closeFileAct->setStatusTip(tr("Close the current PlanPro XML file"));
    connect(closeFileAct, SIGNAL(triggered()), this, SLOT(closeFile()));

    exportToPictureAct = new QAction(tr("P&NG..."), this);
    exportToPictureAct->setIcon(QIcon(":/images/pdf.png"));
    exportToPictureAct->setStatusTip(tr("Export the visible area as PNG picture"));
    connect(exportToPictureAct, SIGNAL(triggered()), this, SLOT(exportToPicture()));

    exportToPdfAct = new QAction(tr("P&DF..."), this);
    exportToPdfAct->setIcon(QIcon(":/images/pdf.png"));
    exportToPdfAct->setStatusTip(tr("Export the visible area as PDF file"));
    connect(exportToPdfAct, SIGNAL(triggered()), this, SLOT(exportToPdf()));

    docInfoAct = new QAction(tr("Show document &information..."), this);
    docInfoAct->setShortcut(tr("Ctrl+I"));
    docInfoAct->setStatusTip(tr("Show the document information of the current PlanPro XML file"));
    connect(docInfoAct, SIGNAL(triggered()), this, SLOT(showDocumentInfo()));

    printFileAct = new QAction(tr("&Print..."), this);
    printFileAct->setIcon(QIcon(":/images/fileprint.png"));
    printFileAct->setShortcut(QKeySequence::Print);
    printFileAct->setStatusTip(tr("Print the visible area"));
    connect(printFileAct, SIGNAL(triggered()), this, SLOT(printFile()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Close the program"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    searchAct = new QAction(tr("&Search..."), this);
    searchAct->setShortcut(tr("Ctrl+F"));
    searchAct->setStatusTip(tr("Search an object based on its GUID"));
    connect(searchAct, SIGNAL(triggered()), this, SLOT(handleObjectSearchFromMenu()));

    editRemarkAct = new QAction(tr("&Edit remark..."), this);
    editRemarkAct->setShortcut(tr("Ctrl+E"));
    editRemarkAct->setStatusTip(tr("Edit the remark section of the current PlanPro XML file"));
    connect(editRemarkAct, SIGNAL(triggered()), this, SLOT(editRemark()));

    centerAct = new QAction(tr("&Center object"), this);
    centerAct->setShortcut(tr("Alt+C"));
    centerAct->setStatusTip(tr("Center the selected object in the graphics view"));
    connect(centerAct, SIGNAL(triggered()), this, SLOT(centerObject()));

    zoomInAct = new QAction(tr("Zoom &in"), this);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setStatusTip(tr("Zoom into the graphics view"));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &out"), this);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setStatusTip(tr("Zoom out of the graphics view"));
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    rotateLeftAct = new QAction(tr("Rotate &left"), this);
    rotateLeftAct->setShortcut(tr("Ctrl+Left"));
    rotateLeftAct->setStatusTip(tr("Rotate the graphics view counter-clockwise"));
    connect(rotateLeftAct, SIGNAL(triggered()), this, SLOT(rotateLeft()));

    rotateRightAct = new QAction(tr("Rotate &right"), this);
    rotateRightAct->setShortcut(tr("Ctrl+Right"));
    rotateRightAct->setStatusTip(tr("Rotate the graphics view clockwise"));
    connect(rotateRightAct, SIGNAL(triggered()), this, SLOT(rotateRight()));

    resetViewAct = new QAction(tr("Reset &view"), this);
    resetViewAct->setShortcut(tr("Alt+V"));
    resetViewAct->setStatusTip(tr("Reset the zoom and rotation of the view to the original value"));
    connect(resetViewAct, SIGNAL(triggered()), this, SLOT(resetView()));

    startViewModeAct = new QAction(tr("&Start State"), this);
    startViewModeAct->setShortcut(tr("F5"));
    startViewModeAct->setStatusTip(tr("Show the start state of the planning"));
    startViewModeAct->setCheckable(true);
    startViewModeAct->setChecked(false);
    connect(startViewModeAct, SIGNAL(triggered()), this, SLOT(switchViewMode()));

    endViewModeAct = new QAction(tr("&End State"), this);
    endViewModeAct->setShortcut(tr("F6"));
    endViewModeAct->setStatusTip(tr("Show the end state of the planning"));
    endViewModeAct->setCheckable(true);
    endViewModeAct->setChecked(true);
    connect(endViewModeAct, SIGNAL(triggered()), this, SLOT(switchViewMode()));

    comparisonViewModeAct = new QAction(tr("Start/End &Comparison"), this);
    comparisonViewModeAct->setShortcut(tr("F7"));
    comparisonViewModeAct->setStatusTip(tr("Show the comparison of the start and end state of the planning"));
    comparisonViewModeAct->setCheckable(true);
    comparisonViewModeAct->setChecked(false);
    connect(comparisonViewModeAct, SIGNAL(triggered()), this, SLOT(switchViewMode()));

    viewModeActGroup = new QActionGroup(this);
    viewModeActGroup->addAction(startViewModeAct);
    viewModeActGroup->addAction(endViewModeAct);
    viewModeActGroup->addAction(comparisonViewModeAct);

    selectAllFiltersAct = new QAction(tr("&Select all filter settings"), this);
    selectAllFiltersAct->setShortcut(tr("Shift+Alt+S"));
    selectAllFiltersAct->setStatusTip(tr("Select all objects in the object filter window"));
    connect(selectAllFiltersAct, SIGNAL(triggered()), filterWidget, SLOT(selectAllFilters()));

    deselectAllFiltersAct = new QAction(tr("D&eselect all filter settings"), this);
    deselectAllFiltersAct->setShortcut(tr("Shift+Alt+D"));
    deselectAllFiltersAct->setStatusTip(tr("Deselect all objects in the object filter window"));
    connect(deselectAllFiltersAct, SIGNAL(triggered()), filterWidget, SLOT(deselectAllFilters()));

    extractFileAct = new QAction(tr("&Extract file..."), this);
    extractFileAct->setShortcut(tr("Alt+E"));
    extractFileAct->setStatusTip(tr("Extract binary data from the selected object"));
    connect(extractFileAct, SIGNAL(triggered()), this, SLOT(extractFile()));

    measureDistanceAct = new QAction(tr("Measure &distance..."), this);
    measureDistanceAct->setShortcut(tr("Alt+D"));
    measureDistanceAct->setStatusTip(tr("Calculate the distance between two selected Punkt_Objekt subtypes"));
    connect(measureDistanceAct, SIGNAL(triggered()), this, SLOT(measureDistance()));

    findReferencingObjectsAct = new QAction(tr("Find &referencing objects"), this);
    findReferencingObjectsAct->setShortcut(tr("Alt+R"));
    findReferencingObjectsAct->setStatusTip(tr("Find all objects referencing the selected object"));
    connect(findReferencingObjectsAct, SIGNAL(triggered()), this, SLOT(findReferencingObjects()));

    addFavoriteAct = new QAction(tr("&Add Favorite"), this);
    addFavoriteAct->setShortcut(tr("Shift+Alt+A"));
    addFavoriteAct->setStatusTip(tr("Add the selected object to the favorite list"));
    connect(addFavoriteAct, SIGNAL(triggered()), this, SLOT(addToFavorites()));

    removeFavoriteAct = new QAction(tr("&Remove Favorite"), this);
    removeFavoriteAct->setShortcut(tr("Shift+Alt+R"));
    removeFavoriteAct->setStatusTip(tr("Remove the selected object from the favorite list"));
    connect(removeFavoriteAct, SIGNAL(triggered()), this, SLOT(removeFromFavorites()));

    clearFavoriteListAct = new QAction(tr("&Clear Favorite List"), this);
    clearFavoriteListAct->setShortcut(tr("Shift+Alt+C"));
    clearFavoriteListAct->setStatusTip(tr("Clear the favorite list"));
    connect(clearFavoriteListAct, SIGNAL(triggered()), favoriteList, SLOT(clear()));

    setLanguageAct = new QAction(tr("Preferences..."), this);
    setLanguageAct->setShortcut(QKeySequence::Preferences);
    setLanguageAct->setStatusTip(tr("Set the language of the program"));
    connect(setLanguageAct, SIGNAL(triggered()), this, SLOT(setLanguage()));

    helpContentsAct = new QAction(tr("&Help"), this);
    helpContentsAct->setIcon(QIcon(":/images/contents.png"));
    helpContentsAct->setShortcut(QKeySequence::HelpContents);
    helpContentsAct->setStatusTip(tr("Show program help"));
    connect(helpContentsAct, SIGNAL(triggered()), this, SLOT(showHelp()));

    aboutAct = new QAction(tr("&About..."), this);
    aboutAct->setStatusTip(tr("Show program and version info"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::enableActions()
{
    if(document->getDocumentType() == PlanProDocument::DocumentTypeInvalid)
    {
        saveFileAct->setEnabled(false);
        saveAsAct->setEnabled(false);
        closeFileAct->setEnabled(false);
        exportToPictureAct->setEnabled(false);
        exportToPdfAct->setEnabled(false);
        docInfoAct->setEnabled(false);
        printFileAct->setEnabled(false);
        searchAct->setEnabled(false);
        editRemarkAct->setEnabled(false);
        startViewModeAct->setEnabled(false);
        endViewModeAct->setEnabled(false);
        comparisonViewModeAct->setEnabled(false);
        centerAct->setEnabled(false);
        zoomInAct->setEnabled(false);
        zoomOutAct->setEnabled(false);
        rotateLeftAct->setEnabled(false);
        rotateRightAct->setEnabled(false);
        resetViewAct->setEnabled(false);
        extractFileAct->setEnabled(false);
        measureDistanceAct->setEnabled(false);
        findReferencingObjectsAct->setEnabled(false);
        addFavoriteAct->setEnabled(false);
        removeFavoriteAct->setEnabled(false);
        clearFavoriteListAct->setEnabled(false);

        scaleSpinBox->setEnabled(false);
        rotateSpinBox->setEnabled(false);
    }
    else
    {
        saveFileAct->setEnabled(isWindowModified());
        saveAsAct->setEnabled(true);
        closeFileAct->setEnabled(true);
        exportToPictureAct->setEnabled(true);
        exportToPdfAct->setEnabled(true);
        docInfoAct->setEnabled(true);
        printFileAct->setEnabled(true);
        searchAct->setEnabled(true);
        editRemarkAct->setEnabled(true);
        startViewModeAct->setEnabled(true);
        endViewModeAct->setEnabled(true);
        comparisonViewModeAct->setEnabled(true);
        centerAct->setEnabled(true);
        zoomInAct->setEnabled(true);
        zoomOutAct->setEnabled(true);
        rotateLeftAct->setEnabled(true);
        rotateRightAct->setEnabled(true);
        resetViewAct->setEnabled(true);
        extractFileAct->setEnabled(true);
        measureDistanceAct->setEnabled(true);
        findReferencingObjectsAct->setEnabled(true);
        addFavoriteAct->setEnabled(true);
        removeFavoriteAct->setEnabled(true);
        clearFavoriteListAct->setEnabled(true);

        scaleSpinBox->setEnabled(true);
        rotateSpinBox->setEnabled(true);
    }
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveFileAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeFileAct);
    fileMenu->addSeparator();
    exportSubmenu = fileMenu->addMenu(tr("&Export"));
    exportSubmenu->addAction(exportToPictureAct);
    exportSubmenu->addAction(exportToPdfAct);
    fileMenu->addSeparator();
    fileMenu->addAction(printFileAct);
    fileMenu->addSeparator();
    fileMenu->addAction(docInfoAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(searchAct);
    editMenu->addAction(editRemarkAct);
    editMenu->addSeparator();
    editMenu->addAction(setLanguageAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewDockSubmenu = viewMenu->addMenu(tr("Show &Dock Windows"));
    viewMenu->addSeparator();
    viewMenu->addAction(startViewModeAct);
    viewMenu->addAction(endViewModeAct);
    viewMenu->addAction(comparisonViewModeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(centerAct);
    viewMenu->addSeparator();
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(rotateLeftAct);
    viewMenu->addAction(rotateRightAct);
    viewMenu->addAction(resetViewAct);
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
    toolBar->addAction(printFileAct);
    toolBar->addSeparator();
    QLabel* label1 = new QLabel(tr("Scale [%]"));
    toolBar->addWidget(label1);
    scaleSpinBox = new QSpinBox();
    scaleSpinBox->setMinimum(0);
    scaleSpinBox->setMaximum(1000);
    scaleSpinBox->setValue(100);
    toolBar->addWidget(scaleSpinBox);
    connect(scaleSpinBox, SIGNAL(valueChanged(int)), this, SLOT(transformGraphicsView()));
    QLabel* label2 = new QLabel(tr("Rotate [°]"));
    toolBar->addWidget(label2);
    rotateSpinBox = new QSpinBox();
    rotateSpinBox->setMinimum(-360);
    rotateSpinBox->setMaximum(360);
    rotateSpinBox->setValue(0);
    toolBar->addWidget(rotateSpinBox);
    connect(rotateSpinBox, SIGNAL(valueChanged(int)), this, SLOT(transformGraphicsView()));
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
    dock4->setWidget(documentTreeView);
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
