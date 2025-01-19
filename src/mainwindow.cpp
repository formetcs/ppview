/*
 * PlanPro Viewer
 *
 * Copyright (c) 2017-2025, The FormETCS Project. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

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
#include "textfiledialog.h"
#include "preferencesdialog.h"
#include "ui_finddialog.h"

MainWindow::MainWindow(const QString& fileName, QWidget* parent)
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

    bookmarkList = new QListWidget();
    bookmarkList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    searchResultList = new QTreeWidget();
    searchResultList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    searchResultList->setColumnCount(2);
    QStringList headerLabels;
    headerLabels << tr("Type") << tr("ID");
    searchResultList->setHeaderLabels(headerLabels);

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
    selectionManager->setBookmarkListWidget(bookmarkList);
    selectionManager->setSearchResultListWidget(searchResultList);

    setCentralWidget(view);

    createActions();
    createMenus();
    createToolBars();
    createDockWindows();
    statusBar();

    setAcceptDrops(true);

    connect(filterWidget, SIGNAL(filterStateChanged(QString,bool)), scene, SLOT(changeFilterSettings(QString,bool)));
    connect(categoryComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCategory()));
    connect(selectionManager, SIGNAL(selectionChanged(QList<DomItem*>)), objectInfo, SLOT(setInfoText(QList<DomItem*>)));
    connect(document, SIGNAL(dataChanged()), doctreemodel, SLOT(modelChanged()));
    connect(document, SIGNAL(dataChanged()), objectlistmodel, SLOT(modelChanged()));
    connect(objectInfo, SIGNAL(uuidClicked(QString)), selectionManager, SLOT(selectItem(QString)));

    enableActions();
    openNamedFile(fileName);
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
        QString s = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(),
                                                 tr("PlanPro XML Files (*.ppxml);;All Files (*.*)") );

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
        bookmarkList->clear();
        searchResultList->clear();
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
    QString s = QFileDialog::getSaveFileName(this, tr("Save File"), defaultFileName,
                                tr("PlanPro XML Files (*.ppxml)"));

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
        bookmarkList->clear();
        searchResultList->clear();
        document->clear();
        setWindowTitle(APPLICATION_NAME);
        doctreemodel->modelChanged();
        objectlistmodel->modelChanged();
        setWindowModified(false);
        enableActions();
    }
}

void MainWindow::saveAsPicture()
{
    QString defaultFileName = document->getFileName();
    int index = defaultFileName.lastIndexOf(".");
    defaultFileName = defaultFileName.left(index);
    defaultFileName += ".png";
    QString s = QFileDialog::getSaveFileName(this, tr("Save as PNG"), defaultFileName,
                                tr("Portable Network Graphics Files (*.png)"));

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

void MainWindow::saveAsPdf()
{
    QString defaultPdfFileName = document->getFileName();
    int index = defaultPdfFileName.lastIndexOf(".");
    defaultPdfFileName = defaultPdfFileName.left(index);
    defaultPdfFileName += ".pdf";
    QString s = QFileDialog::getSaveFileName(this, tr("Save as PDF"), defaultPdfFileName,
                                tr("Portable Document Format Files (*.pdf)"));

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

    QString selectedFilename = QFileDialog::getSaveFileName(this, tr("Save File"), filePathString);

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

void MainWindow::addToBookmarks()
{
    QList<DomItem*> selectedItemList = selectionManager->getSelectedItems();
    for(int i = 0; i < selectedItemList.count(); ++i)
    {
        DomItem* item = selectedItemList.at(i);
        QString name = item->getName();
        QString id = item->getFirstValueAtPath("Identitaet/Wert");
        QListWidgetItem* newItem = new QListWidgetItem;
        newItem->setText(name + " [" + id + "]");
        bookmarkList->addItem(newItem);
    }
}

void MainWindow::removeFromBookmarks()
{
    QList<QListWidgetItem*> selectedList = bookmarkList->selectedItems();
    for(int i = 0; i < selectedList.size(); ++i)
    {
        QListWidgetItem* item = selectedList.at(i);
        int row = bookmarkList->row(item);
        bookmarkList->takeItem(row);
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
    searchResultList->clear();
    DomItem* item = selectedItemList.at(0);
    QString name = item->getName();
    QString id = item->getFirstValueAtPath("Identitaet/Wert");
    QTreeWidgetItem* tlItem = new QTreeWidgetItem();
    tlItem->setText(0, name);
    tlItem->setText(1, id);
    searchResultList->addTopLevelItem(tlItem);
    createReferenceListRec(item, tlItem, 1);
    searchResultList->expandItem(tlItem);
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
        searchResultList->expandItem(newItem);
    }
}

void MainWindow::showPreferences()
{
    PreferencesDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted)
    {
        dialog.saveSettings();
        writeSettings();
        scene->updateColorSettings();
        objectlistmodel->modelChanged();
    }
}

void MainWindow::showDocumentInfo()
{
    QString text = tr("No file loaded");
    if(document->getDocumentType() != PlanProDocument::DocumentTypeInvalid)
    {
        QString doctype;
        QString objectcount;
        if(document->getDocumentType() == PlanProDocument::DocumentTypePlanning)
        {
            doctype = tr("Planning");
            int startcount = document->getObjectList(PlanProDocument::PlanningStateStart).count();
            int endcount = document->getObjectList(PlanProDocument::PlanningStateEnd).count();
            objectcount = tr("PlanPro Object Count (Start state): %1\n"
                             "PlanPro Object Count (End state): %2").arg(startcount).arg(endcount);
        }
        else
        {
            doctype = tr("State");
            int endcount = document->getObjectList(PlanProDocument::PlanningStateEnd).count();
            objectcount = tr("PlanPro Object Count: %1").arg(endcount);
        }
        QString timestamp = document->getTimestamp().toString();
        QString toolname = document->getToolName();
        QString toolversion = document->getToolVersion();
        QString remark = document->getRemark();
        text = tr("Document Type: %1\n\n%2\n\nTimestamp: %3\nTool: %4, Version %5\n\n%6").arg(doctype, objectcount,  timestamp, toolname, toolversion, remark);
    }
    QMessageBox::information(this, tr("Document Information"), text);
}

void MainWindow::editRemark()
{
    QString oldRemark = document->getRemark();
    bool ok;
    QString newRemark = QInputDialog::getMultiLineText(this, tr("Edit Remark"),
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
    QMessageBox::information(this, "Info", "The Online Help will be available in a future release");
}

void MainWindow::showReadme()
{
    TextFileDialog dialog(this);
    dialog.setWindowTitle(tr("Readme File"));
    dialog.resize(QSize(650, 300));
    dialog.loadFile("README.md", true);
    dialog.exec();
}

void MainWindow::showLicense()
{
    TextFileDialog dialog(this);
    dialog.setWindowTitle(tr("License"));
    dialog.resize(QSize(650, 300));
    dialog.loadFile("LICENSE");
    dialog.exec();
}

void MainWindow::show3rdPartyLicenses()
{
    TextFileDialog dialog(this);
    dialog.setWindowTitle(tr("Third Party Licenses"));
    dialog.resize(QSize(450, 300));
    dialog.loadFile("third-party-licenses.txt");
    dialog.exec();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About PlanPro Viewer"),
            tr("<h2>PlanPro Viewer %1.%2.%3</h2>"
            "Supports PlanPro Version %4.%5.%6<br><br>"
            "Copyright &copy; 2017-2025, The FormETCS Project.<br>All rights reserved.<br><br>"
            "This program is free software: you can redistribute it and/or modify "
            "it under the terms of the GNU General Public License as published by "
            "the Free Software Foundation, either version 3 of the License, or "
            "(at your option) any later version.<br><br>"
            "This program is distributed in the hope that it will be useful, "
            "but WITHOUT ANY WARRANTY; without even the implied warranty of "
            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
            "GNU General Public License for more details.")
            .arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH).arg(PLANPRO_MAJOR).arg(PLANPRO_MINOR).arg(PLANPRO_PATCH));
}

void MainWindow::goToObject()
{
    bool ok;
    QString searchedId = QInputDialog::getText(this, tr("Go To Object"),
                                tr("Enter the GUID of the object:"),
                                QLineEdit::Normal, QString(), &ok);
    if(ok && !searchedId.isEmpty())
    {
        selectionManager->selectItem(searchedId);
    }
}

void MainWindow::handleObjectSearch()
{
    Ui::FindDialog ui;
    QDialog dialog(this);
    ui.setupUi(&dialog);
    QStringList categoryList = document->getCategoryList();
    ui.comboBoxCategory->addItems(categoryList);
    if(dialog.exec() == QDialog::Accepted && !ui.lineEditFind->text().isEmpty())
    {
        searchResultList->clear();
        QString category = QString();
        if(ui.comboBoxCategory->currentIndex() > 0)
        {
            category = ui.comboBoxCategory->currentText();
        }

        QList<DomItem*> resultList = document->findObjects(ui.lineEditFind->text(),
                                                         (PlanProDocument::FindMatch) ui.comboBoxMatch->currentIndex(),
                                                         (PlanProDocument::FindState) ui.comboBoxState->currentIndex(),
                                                         category,
                                                         ui.checkBoxCaseSensitive->isChecked(),
                                                         ui.checkBoxAttrName->isChecked(),
                                                         ui.checkBoxAttrValue->isChecked());
        for(int i = 0; i < resultList.count(); ++i)
        {
            DomItem* currentItem = resultList.at(i);
            QString name = currentItem->getName();
            QString id = currentItem->getFirstValueAtPath("Identitaet/Wert");
            QTreeWidgetItem* newItem = new QTreeWidgetItem();
            newItem->setText(0, name);
            newItem->setText(1, id);
            searchResultList->addTopLevelItem(newItem);
        }
    }
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
    openFileAct = new QAction(tr("&Open File..."), this);
    openFileAct->setIcon(QIcon(":/images/document-open.svg"));
    openFileAct->setShortcut(QKeySequence::Open);
    openFileAct->setStatusTip(tr("Open a PlanPro XML file"));
    connect(openFileAct, SIGNAL(triggered()), this, SLOT(openFile()));

    saveFileAct = new QAction(tr("&Save File"), this);
    saveFileAct->setIcon(QIcon(":/images/document-save.svg"));
    saveFileAct->setShortcut(QKeySequence::Save);
    saveFileAct->setStatusTip(tr("Save the current PlanPro XML file"));
    connect(saveFileAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setIcon(QIcon(":/images/document-save-as.svg"));
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the current PlanPro data to a different file"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    closeFileAct = new QAction(tr("&Close File"), this);
    closeFileAct->setIcon(QIcon(":/images/document-close.svg"));
    closeFileAct->setShortcut(QKeySequence::Close);
    closeFileAct->setStatusTip(tr("Close the current PlanPro XML file"));
    connect(closeFileAct, SIGNAL(triggered()), this, SLOT(closeFile()));

    saveAsPictureAct = new QAction(tr("Save As P&NG..."), this);
    saveAsPictureAct->setStatusTip(tr("Save the visible layout area as PNG picture"));
    connect(saveAsPictureAct, SIGNAL(triggered()), this, SLOT(saveAsPicture()));

    saveAsPdfAct = new QAction(tr("Save As P&DF..."), this);
    saveAsPdfAct->setStatusTip(tr("Save the visible layout area as PDF file"));
    connect(saveAsPdfAct, SIGNAL(triggered()), this, SLOT(saveAsPdf()));

    docInfoAct = new QAction(tr("Document &Information..."), this);
    docInfoAct->setIcon(QIcon(":/images/help-about.svg"));
    docInfoAct->setShortcut(tr("Ctrl+I"));
    docInfoAct->setStatusTip(tr("Show the document information of the current PlanPro XML file"));
    connect(docInfoAct, SIGNAL(triggered()), this, SLOT(showDocumentInfo()));

    printFileAct = new QAction(tr("&Print..."), this);
    printFileAct->setIcon(QIcon(":/images/document-print.svg"));
    printFileAct->setShortcut(QKeySequence::Print);
    printFileAct->setStatusTip(tr("Print the visible layout area"));
    connect(printFileAct, SIGNAL(triggered()), this, SLOT(printFile()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setMenuRole(QAction::QuitRole);
    exitAct->setShortcut(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Close the program"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    searchAct = new QAction(tr("&Find..."), this);
    searchAct->setIcon(QIcon(":/images/edit-find.svg"));
    searchAct->setShortcut(QKeySequence::Find);
    searchAct->setStatusTip(tr("Search for objects based on given patterns"));
    connect(searchAct, SIGNAL(triggered()), this, SLOT(handleObjectSearch()));

    editRemarkAct = new QAction(tr("&Edit Remark..."), this);
    editRemarkAct->setIcon(QIcon(":/images/document-edit.svg"));
    editRemarkAct->setShortcut(tr("Ctrl+E"));
    editRemarkAct->setStatusTip(tr("Edit the remark section of the current PlanPro XML file"));
    connect(editRemarkAct, SIGNAL(triggered()), this, SLOT(editRemark()));

    centerAct = new QAction(tr("&Center Object"), this);
    centerAct->setIcon(QIcon(":/images/view-restore.svg"));
    centerAct->setShortcut(tr("Alt+C"));
    centerAct->setStatusTip(tr("Center the selected object in the graphics view"));
    connect(centerAct, SIGNAL(triggered()), this, SLOT(centerObject()));

    zoomInAct = new QAction(tr("Zoom &In"), this);
    zoomInAct->setIcon(QIcon(":/images/zoom-in.svg"));
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setStatusTip(tr("Zoom into the graphics view"));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out"), this);
    zoomOutAct->setIcon(QIcon(":/images/zoom-out.svg"));
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setStatusTip(tr("Zoom out of the graphics view"));
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    rotateLeftAct = new QAction(tr("Rotate &Left"), this);
    rotateLeftAct->setIcon(QIcon(":/images/object-rotate-left.svg"));
    rotateLeftAct->setShortcut(tr("Ctrl+Left"));
    rotateLeftAct->setStatusTip(tr("Rotate the graphics view counter-clockwise"));
    connect(rotateLeftAct, SIGNAL(triggered()), this, SLOT(rotateLeft()));

    rotateRightAct = new QAction(tr("Rotate &Right"), this);
    rotateRightAct->setIcon(QIcon(":/images/object-rotate-right.svg"));
    rotateRightAct->setShortcut(tr("Ctrl+Right"));
    rotateRightAct->setStatusTip(tr("Rotate the graphics view clockwise"));
    connect(rotateRightAct, SIGNAL(triggered()), this, SLOT(rotateRight()));

    resetViewAct = new QAction(tr("Reset &View"), this);
    resetViewAct->setIcon(QIcon(":/images/zoom-original.svg"));
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

    selectAllFiltersAct = new QAction(tr("&Select All Filter Settings"), this);
    selectAllFiltersAct->setShortcut(QKeySequence::SelectAll);
    selectAllFiltersAct->setStatusTip(tr("Select all types in the filter settings window"));
    connect(selectAllFiltersAct, SIGNAL(triggered()), filterWidget, SLOT(selectAllFilters()));

    deselectAllFiltersAct = new QAction(tr("D&eselect All Filter Settings"), this);
    deselectAllFiltersAct->setShortcut(QKeySequence::Deselect);
    deselectAllFiltersAct->setStatusTip(tr("Deselect all types in the filter settings window"));
    connect(deselectAllFiltersAct, SIGNAL(triggered()), filterWidget, SLOT(deselectAllFilters()));

    goToObjectAct = new QAction(tr("&Go To Object..."), this);
    goToObjectAct->setShortcut(tr("Alt+G"));
    goToObjectAct->setStatusTip(tr("Find and select the object with the given GUID"));
    connect(goToObjectAct, SIGNAL(triggered()), this, SLOT(goToObject()));

    extractFileAct = new QAction(tr("&Extract File..."), this);
    extractFileAct->setIcon(QIcon(":/images/archive-extract.svg"));
    extractFileAct->setShortcut(tr("Alt+E"));
    extractFileAct->setStatusTip(tr("Extract binary Base64 attachments from the selected object"));
    connect(extractFileAct, SIGNAL(triggered()), this, SLOT(extractFile()));

    measureDistanceAct = new QAction(tr("Measure &Distance..."), this);
    measureDistanceAct->setIcon(QIcon(":/images/measure.svg"));
    measureDistanceAct->setShortcut(tr("Alt+D"));
    measureDistanceAct->setStatusTip(tr("Calculate the distance between two selected Punkt_Objekt subtypes"));
    connect(measureDistanceAct, SIGNAL(triggered()), this, SLOT(measureDistance()));

    findReferencingObjectsAct = new QAction(tr("Find &Referencing Objects"), this);
    findReferencingObjectsAct->setIcon(QIcon(":/images/system-search.svg"));
    findReferencingObjectsAct->setShortcut(tr("Alt+R"));
    findReferencingObjectsAct->setStatusTip(tr("Find all objects referencing the selected object"));
    connect(findReferencingObjectsAct, SIGNAL(triggered()), this, SLOT(findReferencingObjects()));

    addBookmarkAct = new QAction(tr("&Add Bookmark"), this);
    addBookmarkAct->setIcon(QIcon(":/images/list-add.svg"));
    addBookmarkAct->setShortcut(tr("Shift+Alt+A"));
    addBookmarkAct->setStatusTip(tr("Add the selected object to the bookmark list"));
    connect(addBookmarkAct, SIGNAL(triggered()), this, SLOT(addToBookmarks()));

    removeBookmarkAct = new QAction(tr("&Remove Bookmark"), this);
    removeBookmarkAct->setIcon(QIcon(":/images/list-remove.svg"));
    removeBookmarkAct->setShortcut(tr("Shift+Alt+R"));
    removeBookmarkAct->setStatusTip(tr("Remove the selected object from the bookmark list"));
    connect(removeBookmarkAct, SIGNAL(triggered()), this, SLOT(removeFromBookmarks()));

    clearBookmarkListAct = new QAction(tr("&Clear Bookmark List"), this);
    clearBookmarkListAct->setIcon(QIcon(":/images/edit-clear-list.svg"));
    clearBookmarkListAct->setShortcut(tr("Shift+Alt+C"));
    clearBookmarkListAct->setStatusTip(tr("Clear the bookmark list"));
    connect(clearBookmarkListAct, SIGNAL(triggered()), bookmarkList, SLOT(clear()));

    preferencesAct = new QAction(tr("&Preferences..."), this);
    preferencesAct->setMenuRole(QAction::PreferencesRole);
    preferencesAct->setIcon(QIcon(":/images/configure.svg"));
    preferencesAct->setShortcut(QKeySequence::Preferences);
    preferencesAct->setStatusTip(tr("Set the default behavior of the program"));
    connect(preferencesAct, SIGNAL(triggered()), this, SLOT(showPreferences()));

    helpContentsAct = new QAction(tr("&Help..."), this);
    helpContentsAct->setIcon(QIcon(":/images/help-contents.svg"));
    helpContentsAct->setShortcut(QKeySequence::HelpContents);
    helpContentsAct->setStatusTip(tr("Show program help"));
    connect(helpContentsAct, SIGNAL(triggered()), this, SLOT(showHelp()));

    showReadmeAct = new QAction(tr("Show &Readme File..."), this);
    showReadmeAct->setStatusTip(tr("Show the content of the readme file"));
    connect(showReadmeAct, SIGNAL(triggered()), this, SLOT(showReadme()));

    showLicenseAct = new QAction(tr("Show &License..."), this);
    showLicenseAct->setStatusTip(tr("Show the license file"));
    connect(showLicenseAct, SIGNAL(triggered()), this, SLOT(showLicense()));

    show3rdPartyLicensesAct = new QAction(tr("Show &Third Party Licenses..."), this);
    show3rdPartyLicensesAct->setStatusTip(tr("Show the licenses of third party components"));
    connect(show3rdPartyLicensesAct, SIGNAL(triggered()), this, SLOT(show3rdPartyLicenses()));

    aboutAct = new QAction(tr("&About..."), this);
    aboutAct->setMenuRole(QAction::AboutRole);
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
        saveAsPictureAct->setEnabled(false);
        saveAsPdfAct->setEnabled(false);
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
        goToObjectAct->setEnabled(false);
        extractFileAct->setEnabled(false);
        measureDistanceAct->setEnabled(false);
        findReferencingObjectsAct->setEnabled(false);
        addBookmarkAct->setEnabled(false);
        removeBookmarkAct->setEnabled(false);
        clearBookmarkListAct->setEnabled(false);

        scaleSpinBox->setEnabled(false);
        rotateSpinBox->setEnabled(false);
    }
    else
    {
        saveFileAct->setEnabled(isWindowModified());
        saveAsAct->setEnabled(true);
        closeFileAct->setEnabled(true);
        saveAsPictureAct->setEnabled(true);
        saveAsPdfAct->setEnabled(true);
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
        goToObjectAct->setEnabled(true);
        extractFileAct->setEnabled(true);
        measureDistanceAct->setEnabled(true);
        findReferencingObjectsAct->setEnabled(true);
        addBookmarkAct->setEnabled(true);
        removeBookmarkAct->setEnabled(true);
        clearBookmarkListAct->setEnabled(true);

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
    fileMenu->addAction(saveAsPictureAct);
    fileMenu->addAction(saveAsPdfAct);
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
    editMenu->addAction(preferencesAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewDockSubmenu = viewMenu->addMenu(tr("Show &Dock Windows"));
    viewToolbarSubmenu = viewMenu->addMenu(tr("Show &Toolbars"));
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
    objectMenu->addAction(goToObjectAct);
    objectMenu->addSeparator();
    objectMenu->addAction(extractFileAct);
    objectMenu->addSeparator();
    objectMenu->addAction(measureDistanceAct);
    objectMenu->addAction(findReferencingObjectsAct);

    bookmarkMenu = menuBar()->addMenu(tr("&Bookmarks"));
    bookmarkMenu->addAction(addBookmarkAct);
    bookmarkMenu->addAction(removeBookmarkAct);
    bookmarkMenu->addSeparator();
    bookmarkMenu->addAction(clearBookmarkListAct);

    // Separator before help menu sets it in Motif style
    // at the right border (Convention)
    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpContentsAct);
    helpMenu->addSeparator();
    helpMenu->addAction(showReadmeAct);
    helpMenu->addAction(show3rdPartyLicensesAct);
    helpMenu->addSeparator();
    helpMenu->addAction(showLicenseAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->setObjectName("File");
    fileToolBar->addAction(openFileAct);
    fileToolBar->addAction(saveFileAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(printFileAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(docInfoAct);
    viewToolbarSubmenu->addAction(fileToolBar->toggleViewAction());

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->setObjectName("Edit");
    editToolBar->addAction(searchAct);
    editToolBar->addAction(editRemarkAct);
    viewToolbarSubmenu->addAction(editToolBar->toggleViewAction());

    viewToolBar = addToolBar(tr("View"));
    viewToolBar->setObjectName("View");
    QLabel* label1 = new QLabel(tr("Scale [%]"));
    viewToolBar->addWidget(label1);
    scaleSpinBox = new QSpinBox();
    scaleSpinBox->setMinimum(0);
    scaleSpinBox->setMaximum(1000);
    scaleSpinBox->setValue(100);
    viewToolBar->addWidget(scaleSpinBox);
    connect(scaleSpinBox, SIGNAL(valueChanged(int)), this, SLOT(transformGraphicsView()));
    viewToolBar->addAction(zoomInAct);
    viewToolBar->addAction(zoomOutAct);
    viewToolBar->addSeparator();
    QLabel* label2 = new QLabel(tr("Rotate [°]"));
    viewToolBar->addWidget(label2);
    rotateSpinBox = new QSpinBox();
    rotateSpinBox->setMinimum(-360);
    rotateSpinBox->setMaximum(360);
    rotateSpinBox->setValue(0);
    viewToolBar->addWidget(rotateSpinBox);
    connect(rotateSpinBox, SIGNAL(valueChanged(int)), this, SLOT(transformGraphicsView()));
    viewToolBar->addAction(rotateLeftAct);
    viewToolBar->addAction(rotateRightAct);
    viewToolbarSubmenu->addAction(viewToolBar->toggleViewAction());

    objectToolBar = addToolBar(tr("Object"));
    objectToolBar->setObjectName("Object");
    objectToolBar->addAction(extractFileAct);
    objectToolBar->addAction(measureDistanceAct);
    objectToolBar->addAction(findReferencingObjectsAct);
    viewToolbarSubmenu->addAction(objectToolBar->toggleViewAction());

    bookmarkToolBar = addToolBar(tr("Bookmarks"));
    bookmarkToolBar->setObjectName("Bookmarks");
    bookmarkToolBar->addAction(addBookmarkAct);
    bookmarkToolBar->addAction(removeBookmarkAct);
    bookmarkToolBar->addAction(clearBookmarkListAct);
    viewToolbarSubmenu->addAction(bookmarkToolBar->toggleViewAction());
}

void MainWindow::createDockWindows()
{
    QDockWidget *dockDocumentStructure = new QDockWidget(tr("Document Structure"), this);
    dockDocumentStructure->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockDocumentStructure->setObjectName("Document Structure");
    dockDocumentStructure->setWidget(documentTreeView);
    addDockWidget(Qt::LeftDockWidgetArea, dockDocumentStructure);
    viewDockSubmenu->addAction(dockDocumentStructure->toggleViewAction());

    QDockWidget *dockObjectList = new QDockWidget(tr("Object List"), this);
    dockObjectList->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockObjectList->setObjectName("Object List");
    QWidget* objectListWidget = new QWidget();
    QVBoxLayout* layoutObjectList = new QVBoxLayout;
    layoutObjectList->addWidget(categoryComboBox);
    layoutObjectList->addWidget(objectListView);
    objectListWidget->setLayout(layoutObjectList);
    dockObjectList->setWidget(objectListWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockObjectList);
    viewDockSubmenu->addAction(dockObjectList->toggleViewAction());

    QDockWidget *dockObjectInfo = new QDockWidget(tr("Object Properties"), this);
    dockObjectInfo->setObjectName("Object Properties");
    dockObjectInfo->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    dockObjectInfo->setWidget(objectInfo);
    addDockWidget(Qt::BottomDockWidgetArea, dockObjectInfo);
    viewDockSubmenu->addAction(dockObjectInfo->toggleViewAction());

    QDockWidget *dockFilterSettings = new QDockWidget(tr("Filter Settings"), this);
    dockFilterSettings->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockFilterSettings->setObjectName("Filter Settings");
    dockFilterSettings->setWidget(filterWidget);
    addDockWidget(Qt::RightDockWidgetArea, dockFilterSettings);
    viewDockSubmenu->addAction(dockFilterSettings->toggleViewAction());

    QDockWidget *dockBookmarks = new QDockWidget(tr("Bookmarks"), this);
    dockBookmarks->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockBookmarks->setObjectName("Bookmarks");
    dockBookmarks->setWidget(bookmarkList);
    addDockWidget(Qt::RightDockWidgetArea, dockBookmarks);
    viewDockSubmenu->addAction(dockBookmarks->toggleViewAction());

    QDockWidget *dockSearchResults = new QDockWidget(tr("Search Results"), this);
    dockSearchResults->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockSearchResults->setObjectName("Search Results");
    dockSearchResults->setWidget(searchResultList);
    addDockWidget(Qt::RightDockWidgetArea, dockSearchResults);
    viewDockSubmenu->addAction(dockSearchResults->toggleViewAction());
}
