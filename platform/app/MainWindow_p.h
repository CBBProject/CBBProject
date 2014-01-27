#pragma once

#include <QtCore>
#include <QtGui>

class dtkComposer;
class dtkComposerFactoryView;
class dtkComposerGraphView;
class dtkComposerSceneModel;
class dtkComposerSceneNodeEditor;
class dtkComposerSceneView;
class dtkComposerStackView;

class dtkRecentFilesMenu;

class dtkSplitter;

class MainWindow;

class MainWindowPrivate : public QObject {

    Q_OBJECT

public slots:
    void setModified(bool modified);

public:

    bool maySave();
    void setCurrentFile(const QString& file);

    dtkComposer *composer;
    dtkComposerFactoryView *nodes;
    dtkComposerGraphView *graph;
    dtkComposerSceneModel *model;
    dtkComposerSceneNodeEditor *editor;
    dtkComposerSceneView *scene;
    dtkComposerStackView *stack;

    dtkSplitter *inner;

    QMenu*   composition_menu;
    QAction* composition_open_action;
    QAction* composition_save_action;
    QAction* composition_saveas_action;
    QAction* composition_insert_action;
    QAction* composition_quit_action;

    QMenu*   edit_menu;
    QAction* undo_action;
    QAction* redo_action;

    dtkRecentFilesMenu* recent_compositions_menu;

    QString current_composition;

    bool closing;

    MainWindow* q;
};
