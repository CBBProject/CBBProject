#include <MainWindow.h>
#include <MainWindow_p.h>

#include <Composer/cbbComposerFactory.h>

#include <dtkComposer/dtkComposer.h>
#include <dtkComposer/dtkComposerCompass.h>
#include <dtkComposer/dtkComposerEvaluator.h>
#include <dtkComposer/dtkComposerFactoryView.h>
#include <dtkComposer/dtkComposerScene.h>
#include <dtkComposer/dtkComposerSceneModel.h>
#include <dtkComposer/dtkComposerSceneNodeEditor.h>
#include <dtkComposer/dtkComposerSceneView.h>
#include <dtkComposer/dtkComposerStack.h>
#include <dtkComposer/dtkComposerStackView.h>
#include <dtkComposer/dtkComposerView.h>

#include <dtkGui/dtkRecentFilesMenu.h>
#include <dtkGui/dtkSpacer.h>
#include <dtkGui/dtkSplitter.h>
#include <dtkGui/dtkViewManager.h>

#include <dtkCore/dtkGlobal.h>

#include <dtkLog/dtkLog.h>

#include <QtCore>
#include <QtGui>

// /////////////////////////////////////////////////////////////////
// MainWindowPrivate
// /////////////////////////////////////////////////////////////////

bool MainWindowPrivate::maySave() {
    if(this->closing)
        return true;

    if (q->isWindowModified()) {
        QMessageBox::StandardButton ret = QMessageBox::warning(q,
            q->tr("Creator"),
            q->tr("The composition has been modified.\n Do you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Save);

        if (ret == QMessageBox::Save)
            return q->compositionSave();
        else
            if(ret == QMessageBox::Cancel)
                return false;
    }

    return true;
}

void MainWindowPrivate::setCurrentFile(const QString &file) {
     this->current_composition = file;

     q->setWindowModified(false);

     QString shownName = this->current_composition;
     if (shownName.isEmpty())
         shownName = "untitled.dtk";

     q->setWindowFilePath(shownName);
}

void MainWindowPrivate::setModified(bool modified) {
    q->setWindowModified(modified);
}

// /////////////////////////////////////////////////////////////////
// MainWindow
// /////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), d(new MainWindowPrivate) {
    d->q = this;

    d->composer = new dtkComposer;
    d->composer->setFactory(new cbbComposerFactory);
    d->composer->view()->setBackgroundBrush(QColor("#ffffff"));
    d->composer->view()->setCacheMode(QGraphicsView::CacheBackground);

    d->editor = new dtkComposerSceneNodeEditor(this);
    d->editor->setScene(d->composer->scene());
    d->editor->setStack(d->composer->stack());
    d->editor->setGraph(d->composer->graph());

    d->model = new dtkComposerSceneModel(this);
    d->model->setScene(d->composer->scene());

    d->scene = new dtkComposerSceneView(this);
    d->scene->setScene(d->composer->scene());
    d->scene->setModel(d->model);

    d->stack = new dtkComposerStackView(this);
    d->stack->setStack(d->composer->stack());

    d->nodes = new dtkComposerFactoryView(this);
    d->nodes->setFactory(d->composer->factory());

    d->closing = false;

    // -- Actions

    d->composition_open_action = new QAction("Open", this);
    d->composition_open_action->setShortcut(QKeySequence::Open);

    d->composition_save_action = new QAction("Save", this);
    d->composition_save_action->setShortcut(QKeySequence::Save);

    d->composition_saveas_action = new QAction("Save As...", this);
    d->composition_saveas_action->setShortcut(QKeySequence::SaveAs);

    d->composition_insert_action = new QAction("Insert", this);
    d->composition_insert_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_I);

    d->composition_quit_action = new QAction("Quit", this);
    d->composition_quit_action->setShortcut(QKeySequence::Quit);

    d->undo_action = d->composer->stack()->createUndoAction(this);
    d->undo_action->setShortcut(QKeySequence::Undo);

    d->redo_action = d->composer->stack()->createRedoAction(this);
    d->redo_action->setShortcut(QKeySequence::Redo);
    
    QAction *run_action = new QAction("Run", this);
    run_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_R);

    QAction *step_action = new QAction("Step", this);
    step_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_N);

    QAction *continue_action = new QAction("Cont", this);
    continue_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_C);

    QAction *next_action = new QAction("Next", this);
    next_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_T);

    QAction *stop_action = new QAction("Stop", this);
    stop_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_Period);

    // -- Menus

    QMenuBar *menu_bar = this->menuBar();

    d->recent_compositions_menu = new dtkRecentFilesMenu("Open recent...", this);

    d->composition_menu = menu_bar->addMenu("Composition");
    d->composition_menu->addAction(d->composition_open_action);
    d->composition_menu->addMenu(d->recent_compositions_menu);
    d->composition_menu->addAction(d->composition_save_action);
    d->composition_menu->addAction(d->composition_saveas_action);
    d->composition_menu->addSeparator();
    d->composition_menu->addAction(d->composition_insert_action);
    d->composition_menu->addSeparator();
    d->composition_menu->addAction(d->composition_quit_action);

    d->edit_menu = menu_bar->addMenu("Edit");
    d->edit_menu->addAction(d->undo_action);
    d->edit_menu->addAction(d->redo_action);

    QMenu *debug_menu = menu_bar->addMenu("Debug");
    debug_menu->addAction(run_action);
    debug_menu->addAction(step_action);
    debug_menu->addAction(continue_action);
    debug_menu->addAction(next_action);
    debug_menu->addAction(stop_action);

    // -- Connections

    connect(run_action, SIGNAL(triggered()), d->composer, SLOT(run()));
    connect(step_action, SIGNAL(triggered()), d->composer, SLOT(step()));
    connect(continue_action, SIGNAL(triggered()), d->composer, SLOT(cont()));
    connect(next_action, SIGNAL(triggered()), d->composer, SLOT(next()));
    connect(stop_action, SIGNAL(triggered()), d->composer, SLOT(stop()));

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(close()));

    //connect(d->composer, SIGNAL(modified(bool)), d, SLOT(setModified(bool)));

    connect(d->composition_open_action, SIGNAL(triggered()), this, SLOT(compositionOpen()));
    connect(d->composition_save_action, SIGNAL(triggered()), this, SLOT(compositionSave()));
    connect(d->composition_saveas_action, SIGNAL(triggered()), this, SLOT(compositionSaveAs()));
    connect(d->composition_insert_action, SIGNAL(triggered()), this, SLOT(compositionInsert()));
    connect(d->composition_quit_action, SIGNAL(triggered()), qApp, SLOT(quit()));

    connect(d->recent_compositions_menu, SIGNAL(recentFileTriggered(const QString&)), this, SLOT(compositionOpen(const QString&)));

    // -- Layout

    dtkSplitter *left = new dtkSplitter(this);
    left->setOrientation(Qt::Vertical);
    left->addWidget(d->nodes);

    dtkSplitter *right = new dtkSplitter(this);
    right->setOrientation(Qt::Vertical);
    right->addWidget(d->scene);
    right->addWidget(d->editor);
    right->addWidget(d->stack);
    right->addWidget(d->composer->compass());
    right->setSizes(QList<int>()
                    << this->size().height()/4
                    << this->size().height()/4
                    << this->size().height()/4
                    << this->size().height()/4);

    QWidget *middle = new QWidget(this);
    
    QVBoxLayout *m_layout = new QVBoxLayout(middle);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->addWidget(d->composer);
    m_layout->addWidget(new dtkViewManager(this));

    d->inner = new dtkSplitter(this);
    d->inner->setOrientation(Qt::Horizontal);
    d->inner->addWidget(left);
    d->inner->addWidget(middle);
    d->inner->addWidget(right);
    d->inner->setSizes(QList<int>()
                    << 200
                    << this->size().width()-200-200
                    << 200);

    dtkSplitter *central = new dtkSplitter(this);
    central->setOrientation(Qt::Vertical);
    central->addWidget(d->inner);

    this->setCentralWidget(central);
}

MainWindow::~MainWindow() {
    delete d;
}

bool MainWindow::compositionOpen() {
    if(!d->maySave())
        return true;

    QFileDialog *dialog = new QFileDialog(this, tr("Open composition"), QDir::homePath(), QString("dtk composition (*.dtk)"));
    dialog->setStyleSheet("background-color: none ; color: none;");
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->open(this, SLOT(compositionOpen(const QString&)));

    return true;
}

bool MainWindow::compositionOpen(const QString& file) {
    if(sender() == d->recent_compositions_menu && !d->maySave())
        return true;

    bool status = d->composer->open(file);

    if(status) {
        d->recent_compositions_menu->addRecentFile(file);
        d->setCurrentFile(file);
    }

    return status;
}

bool MainWindow::compositionSave() {
    bool status;

    if(d->current_composition.isEmpty() || d->current_composition == "untitled.dtk")
        status = this->compositionSaveAs();
    else
        status = d->composer->save();

    if(status)
        this->setWindowModified(false);

    return status;
}

bool MainWindow::compositionSaveAs() {

    bool status = false;

    QStringList nameFilters;
    nameFilters <<  "Ascii composition (*.dtk)";
    nameFilters << "Binary composition (*.dtk)";

    QFileDialog dialog(this, "Save composition as ...", QDir::homePath(), QString("dtk composition (*.dtk)"));
    dialog.setStyleSheet("background-color: none ; color: none;");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setConfirmOverwrite(true);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilters(nameFilters);
    dialog.setDefaultSuffix("dtk");

    if(dialog.exec()) {

        if(dialog.selectedNameFilter() == nameFilters.at(0))
            status = this->compositionSaveAs(dialog.selectedFiles().first(), dtkComposerWriter::Ascii);
        else
            status = this->compositionSaveAs(dialog.selectedFiles().first(), dtkComposerWriter::Binary);
    }

    return status;
}

bool MainWindow::compositionSaveAs(const QString& file, dtkComposerWriter::Type type) {

    bool status = false;

    if(file.isEmpty())
        return status;

    status = d->composer->save(file, type);

    if(status) {
        d->setCurrentFile(file);
        this->setWindowModified(false);
    }
    
    return status;
}

bool MainWindow::compositionInsert() {
    QFileDialog *dialog = new QFileDialog(this, tr("Insert composition"), QDir::homePath(), QString("dtk composition (*.dtk)"));
    dialog->setStyleSheet("background-color: none ; color: none;");
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->open(this, SLOT(compositionInsert(const QString&)));

    return true;
}

bool MainWindow::compositionInsert(const QString& file) {
    const bool status = d->composer->insert(file);

    if(status)
        this->setWindowModified(true);

    return status;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (d->maySave()) {
         d->closing = true;
         event->accept();
     } else {
         event->ignore();
     }
}
