/**
 * @file: mainscene.cpp
 */

#include <QResizeEvent>
#include <QSize>
#include <QImage>
#include <QFileDialog>
#include <QString>
#include <QtGui>
#include <QGraphicsView>
#include <QWheelEvent>
#include <Qt>

#include <map>

#include "../Gui/mainscene.h"
#include "../Gui/GNode.h"
#include "../Gui/GEdge.h"
#include "../Gui/LayoutIterationDialog.h"
#include "ui_mainscene.h"
#include "../DumpParser.h"

#define default_frame_width 20

MainScene::MainScene(QWidget *parent, const QString * filename) :
    QMainWindow(parent),
    ui(new Ui::MainScene) {
    ui->setupUi(this);
    m_graph = new LGraph();
    m_scene = new QGraphicsScene(ui->CFGView->sceneRect());
    layout_iterations = 3;
    is_drawed = false;
    ui->CFGView->setRenderHint(QPainter::Antialiasing);
    ui->CFGView->scale(qreal(0.8), qreal(0.8));
}

void MainScene::ScaleView(qreal scale_factor) {
    qreal factor = ui->CFGView->transform().scale(scale_factor, scale_factor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    ui->CFGView->scale(scale_factor, scale_factor);
 }

MainScene::~MainScene() {
    if (m_graph)
        m_graph->Destroy();

    if (m_scene)
        delete m_scene;
}

void MainScene::SetLayoutIteratrions() {
   LayoutIterationDialog * dial = new LayoutIterationDialog((QWidget *)this);
   dial->show();
}

bool MainScene::LoadDump() {
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open a dump"),
                //QDir::currentPath(),
                "/Users",
                tr("Dump files (*.txt);;All files (*.*)") );

    if (filename.isNull()) {
        return false;
     }

    if (is_drawed) {
        m_graph->Destroy();
        m_graph = new LGraph();
    } else
        is_drawed = true;

    // Initialization of DumpProp and CFGView.
    ParseDump(m_graph, filename);

    Draw();
    return true;
}

bool MainScene::Save() {
    if (m_scene == NULL)
        return false;
    QString filename = QFileDialog::getSaveFileName(
                this,
                tr("Save image as"),
                //QDir::currentPath(),
                "/Users",
                tr("All files (*.*)") );

    if (filename.isNull()) {
        return false;
     }

    QImage image(m_scene->width(), m_scene->height(),
                 QImage::Format_ARGB32_Premultiplied);
    image.fill(0xFFFFFF);
    QPainter painter(&image);
    m_scene->render(&painter);
    image.save(filename);
    return true;
}

bool MainScene::Authors() {
    return true;
}

bool MainScene::Version() {
    return true;
}

bool MainScene::Resize(const QSize &iconSize) {
    ui->CFGView->resize(iconSize);
    return true;
}

void MainScene::resizeEvent(QResizeEvent * resize) {
    ui->CFGView->setGeometry(QRect(0, 0,
                                   resize->size().width() - default_frame_width,
                                   resize->size().height() - default_frame_width));
}

bool MainScene::SetGraph(LGraph * graph_to_set) {
    if (m_scene == NULL)
        return false;

    map<pNode, GNode *> nodes_map;
    GNode * buf_node;
    GEdge * buf_edge;
    for (list<pNode>::iterator node_iter = graph_to_set->nodes_list()->begin();
         node_iter != graph_to_set->nodes_list()->end();
         node_iter++)
    {
        buf_node = new GNode(this, ((pLNode)(*node_iter))->IsDummy(),
                             (*node_iter)->id(),
                             ((pLNode)(*node_iter))->Content());
        nodes_map[*node_iter] = buf_node;
        buf_node->setPos(((pLNode)(*node_iter))->getX(), ((pLNode)(*node_iter))->getY());
        if (((pLNode)(*node_iter))->IsDummy())
            buf_node->setVisible(false);
        m_scene->addItem(buf_node);
    }

    map<pEdge, bool> added_edges;
    for (list<pNode>::iterator node_iter = graph_to_set->nodes_list()->begin();
         node_iter != graph_to_set->nodes_list()->end();
         node_iter++)
    {
        for (list<pEdge>::iterator edge_iter = (*node_iter)->in_edges_list()->begin();
             edge_iter != (*node_iter)->in_edges_list()->end();
             edge_iter++)
        {
            if (added_edges.find((*edge_iter)) == added_edges.end())
            {
                if (((pLEdge)(*edge_iter))->Composite()) {
                    if (((pLEdge)(*edge_iter))->composite_edges() == NULL)
                        continue;

                    list<pLEdge> * edge_list = const_cast<list<pLEdge> *>
                            (((pLEdge)(*edge_iter))->composite_edges());
                    list<GEdge *> * composite_edges = new list<GEdge *>;

                    GEdge * buf_comp_edge;
                    for (list<pLEdge>::iterator comp_edge_iter = edge_list->begin();
                         comp_edge_iter != edge_list->end();
                         comp_edge_iter++) {
                        if (((pLEdge)(*comp_edge_iter))->IsReverse() == false)

                            buf_comp_edge = new GEdge(nodes_map[(*comp_edge_iter)->to()],
                                                 nodes_map[(*comp_edge_iter)->from()]);
                        else
                            buf_comp_edge = new GEdge(nodes_map[(*comp_edge_iter)->from()],
                                                     nodes_map[(*comp_edge_iter)->to()]);
                        buf_comp_edge->SetComposite(true);
                        added_edges[(*comp_edge_iter)] = true;
                        (*comp_edge_iter)->SetComposite(false);
                        composite_edges->push_back(buf_comp_edge);
                    }

                    for (list<GEdge *>::iterator comp_edge_iter = composite_edges->begin();
                         comp_edge_iter != composite_edges->end();
                         comp_edge_iter++) {
                        (*comp_edge_iter)->SetCompositeEdges(new list<GEdge *>(*composite_edges));
                        (*comp_edge_iter)->SetComposite(true);
                        m_scene->addItem(*comp_edge_iter);
                    }
                } else {
                    added_edges[*edge_iter] = true;

                    if (((pLEdge)(*edge_iter))->IsReverse() == false)
                        buf_edge = new GEdge(nodes_map[(*edge_iter)->to()],
                                             nodes_map[(*edge_iter)->from()]);
                    else
                        buf_edge = new GEdge(nodes_map[(*edge_iter)->from()],
                                                 nodes_map[(*edge_iter)->to()]);
                    m_scene->addItem(buf_edge);
                }
            }
        }
    }
    return true;
}

bool MainScene::Draw() {
    ui->statusbar->showMessage("Draw started");
    if (ui->CFGView == NULL) {
        ui->statusbar->showMessage("Cannot init QGraphicsView!\n");
        return false;
    }

    if (m_graph == NULL) {
        ui->statusbar->showMessage("Cannot display the dump");
        return false;
    }

    if (is_drawed)
        if (m_scene)
            m_scene->clear();
        else
            ui->statusbar->showMessage("Cannot init QGraphicsScene");

    m_graph->Layout();

    if (SetGraph(m_graph)) {
        ui->CFGView->setScene(m_scene);
        ui->CFGView->show();
    } else {
        return false;
    }

    ui->statusbar->showMessage("Draw finished");
    return true;
}
