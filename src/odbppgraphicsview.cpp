#include "odbppgraphicsview.h"

#include "context.h"
#include "graphicslayer.h"
#include "symbolfactory.h"

ODBPPGraphicsView::ODBPPGraphicsView(QWidget* parent): QGraphicsView(parent),
  m_profile(NULL)
{
  m_scene = new ODBPPGraphicsScene(this);
  m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
  m_scene->setBackgroundBrush(ctx.bg_color);
  m_scene->setSceneRect(-800, -600, 1600, 1200);
  setScene(m_scene);

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  //setRenderHint(QPainter::Antialiasing);
  setCacheMode(CacheBackground);
  setOptimizationFlags(DontSavePainterState);
  setTransformationAnchor(AnchorUnderMouse);
  setViewportUpdateMode(BoundingRectViewportUpdate);
  setZoomMode(AreaZoom);

  connect(m_scene, SIGNAL(rectSelected(QRectF)), this,
      SLOT(zoomToRect(QRectF)));
  connect(horizontalScrollBar(), SIGNAL(valueChanged(int)),
      this, SLOT(updateLayerViewport(void)));
  connect(verticalScrollBar(), SIGNAL(valueChanged(int)),
      this, SLOT(updateLayerViewport(void)));
}

ODBPPGraphicsView::~ODBPPGraphicsView()
{
  m_scene->deleteLater();
}

void ODBPPGraphicsView::scaleView(qreal scaleFactor)
{
  for (int i = 0; i < m_scaleInvariantItems.size(); ++i) {
    m_scaleInvariantItems[i]->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
  }

  if (m_zoomMode != MousePan) {
    setTransformationAnchor(AnchorViewCenter);
  }
  scale(scaleFactor, scaleFactor);
  setTransformationAnchor(AnchorUnderMouse);
}

void ODBPPGraphicsView::scrollView(int dx, int dy)
{
  QScrollBar* hsb = horizontalScrollBar();
  hsb->setValue(hsb->value() + dx);

  QScrollBar* vsb = verticalScrollBar();
  vsb->setValue(vsb->value() + dy);
}

void ODBPPGraphicsView::setZoomMode(ZoomMode mode)
{
  m_zoomMode = mode;
  switch (mode) {
  case None:
    m_scene->setAreaZoomEnabled(false);
    setDragMode(NoDrag);
    break;
  case AreaZoom:
    m_scene->setAreaZoomEnabled(true);
    m_scene->setHighlight(false);
    setDragMode(NoDrag);
    break;
  case MousePan:
    m_scene->setAreaZoomEnabled(false);
    m_scene->setHighlight(false);
    setDragMode(ScrollHandDrag);
    break;
  }
}

void ODBPPGraphicsView::clearScene(void)
{
  m_scene->clear();
}

void ODBPPGraphicsView::addLayer(GraphicsLayer* layer)
{
  m_scene->addLayer(layer);
}

void ODBPPGraphicsView::removeLayer(GraphicsLayer* layer)
{
  m_scene->removeLayer(layer);
}

void ODBPPGraphicsView::addItem(QGraphicsItem* item, bool scale_invariant)
{
  if (scale_invariant) {
    m_scaleInvariantItems.append(item);
  }
  m_scene->addItem(item);
}

void ODBPPGraphicsView::removeItem(QGraphicsItem* item)
{
  m_scene->removeItem(item);
}

void ODBPPGraphicsView::loadProfile(QString step)
{
  QString path;
  path = QString("steps/%1/profile").arg(step.toLower());
  m_profile = new Profile(ctx.loader->absPath(path));

  QColor color(255 - ctx.bg_color.red(), 255 - ctx.bg_color.green(),
      255 - ctx.bg_color.blue(), 255);

  m_profile->setPen(QPen(color, 0));
  m_profile->setBrush(Qt::transparent);
  m_scene->addItem(m_profile);

  m_origin = new OriginSymbol();
  m_origin->setPen(QPen(color, 0));
}

void ODBPPGraphicsView::setBackgroundColor(QColor color)
{
  m_scene->setBackgroundBrush(color);

  if (m_profile) {
    QColor color(255 - ctx.bg_color.red(), 255 - ctx.bg_color.green(),
        255 - ctx.bg_color.blue(), 255);
    m_profile->setPen(QPen(color, 0));
    m_profile->setBrush(Qt::transparent);
  }
}

void ODBPPGraphicsView::setHighlight(bool status)
{
  if (status) {
    setZoomMode(ODBPPGraphicsView::None);
  } else {
    setZoomMode(ODBPPGraphicsView::AreaZoom);
  }
}

void ODBPPGraphicsView::clearHighlight(void)
{
  m_scene->clearHighlight();
}

void ODBPPGraphicsView::initialZoom(void)
{
  zoomToAll();
  addItem(m_origin, true);
}

void ODBPPGraphicsView::zoomToAll(void)
{
  QRectF bounding(m_profile->boundingRect());
  QList<GraphicsLayer*> layers = m_scene->layers();
  for (int i = 0; i < layers.size(); ++i) {
    bounding = bounding.united(layers[i]->boundingRect());
  }
  zoomToRect(bounding);
}

void ODBPPGraphicsView::zoomToRect(QRectF rect)
{
  if (rect.isNull()) {
    return;
  }

  QRectF b = rect.normalized();
  QRectF current = transform().mapRect(QRectF(0, 0, 1, 1));
  QRectF vp = viewport()->rect();

  qreal sx = vp.width() / (current.width() * b.width() * 1.1);
  qreal sy = vp.height() / (current.height() * b.height() * 1.1);
  qreal s = qMin(sx, sy);

  scaleView(s);
  centerOn(b.center());
}

void ODBPPGraphicsView::updateLayerViewport(void)
{
  QRect vrect = viewport()->rect();
  QRectF srect = mapToScene(vrect).boundingRect();
  m_scene->updateLayerViewport(vrect, srect);

  emit sceneRectChanged(srect);
}

void ODBPPGraphicsView::wheelEvent(QWheelEvent *event)
{
  scaleView(pow((double)2, -event->delta() / 240.0));
}

void ODBPPGraphicsView::keyPressEvent(QKeyEvent* event)
{
  switch (event->key()) {
  case Qt::Key_Home:
    zoomToAll();
    return;
  case Qt::Key_PageUp:
    scaleView(2);
    return;
  case Qt::Key_PageDown:
    scaleView(0.5);
    return;
  }
  QGraphicsView::keyPressEvent(event);
}

void ODBPPGraphicsView::resizeEvent(QResizeEvent* event)
{
  updateLayerViewport();
  QGraphicsView::resizeEvent(event);
}
