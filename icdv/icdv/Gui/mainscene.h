#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QSize>
#include <QString>
#include <QResizeEvent>

#include "../Layout/Layout.h"

/// Class that's an implementation of gui.
/**
 * LoadDump() calls QFileDialog and Draw()
 * Draw() draws graph into view and displays
 *  it into ui->CFGView
 * Save() saves curent graph as an image file.
 * Authors() shows authors of this program:).
 * Version() shows version of this program.
 * ZoomIn() zooms in curent image.
 * ZoomOut() zooms out curent image.
 */

namespace Ui {
    class MainScene;
}

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainScene(QWidget *parent = 0, const QString * filename = 0);
    ~MainScene();

    void SetLayoutIteratrions(int retries) {
        if (retries < 1)
            return;
        layout_iterations = retries;
    }

public slots:
    bool LoadDump();
    bool Save();
    bool Authors();
    bool Version();
    void ZoomIn() {
        ScaleView(qreal(1.2));
    }

    void ZoomOut() {
        ScaleView(qreal(1./1.2));
    }

    void SetLayoutIteratrions();
    bool Resize(const QSize &iconSize);


protected:
    // Sets graph_to_set to m_graph if it's valid.
    bool SetGraph(LGraph * graph_to_set);

    // Representation of call graph.
    LGraph * m_graph;

    // This is image that will display in
    // CFGView (look at mainscene.ui).
    QGraphicsScene * m_scene;

    //void keyPressEvent(QKeyEvent *event);

protected slots:
    // This function provides resize of main window
    // and CFGView (look at mainscene.ui).
    void resizeEvent(QResizeEvent * resize);

private:
    // This function draws m_graph into
    // m_scene (see below) and puts it into
    // CFGView (look at mainscene.ui).
    bool Draw();
    bool is_drawed;

    void ScaleView(qreal scale_factor);
    int layout_iterations;
    // ui is a main window gui.
    Ui::MainScene *ui;
};

#endif // MAINSCENE_H
