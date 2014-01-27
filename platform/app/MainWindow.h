#pragma once

#include <dtkComposer/dtkComposerWriter.h>

#include <QtGui>

class MainWindowPrivate;

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
     MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    bool compositionOpen();
    bool compositionOpen(const QString& file);
    bool compositionSave();
    bool compositionSaveAs();
    bool compositionSaveAs(const QString& file, dtkComposerWriter::Type type = dtkComposerWriter::Ascii);
    bool compositionInsert();
    bool compositionInsert(const QString& file);

protected:

    void closeEvent(QCloseEvent *event);

private:
    MainWindowPrivate *d;
};
