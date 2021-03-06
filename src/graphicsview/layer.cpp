#include "layer.h"

#include <QtGui>

#include "context.h"
#include "odbppgraphicsscene.h"

Layer::Layer(QString step, QString layer):
  GraphicsLayer(NULL), m_step(step), m_layer(layer), m_notes(NULL)
{
  GraphicsLayerScene* scene = new GraphicsLayerScene;
  m_features = new LayerFeatures(step, "steps/%1/layers/" +layer +"/features");
  m_features->addToScene(scene);
  setLayerScene(scene);
}

Layer::~Layer()
{
  if (m_notes) {
    delete m_notes;
  }
  delete m_features;
}

QString Layer::step()
{
  return m_step;
}

QString Layer::layer()
{
  return m_layer;
}

Notes* Layer::notes()
{
  if (!m_notes) {
    m_notes = new Notes(m_step, m_layer);
  }
  return m_notes;
}

QStandardItemModel* Layer::reportModel(void)
{
  return m_features->reportModel();
}

void Layer::setHighlightEnabled(bool status)
{
  m_layerScene->setHighlightEnabled(status);
}

void Layer::setShowStepRepeat(bool status)
{
  m_features->setShowStepRepeat(status);
  forceUpdate();
}

void Layer::setPen(const QPen& pen)
{
  m_features->setPen(pen);
  GraphicsLayer::setPen(pen);
}

void Layer::setBrush(const QBrush& brush)
{
  m_features->setBrush(brush);
  GraphicsLayer::setBrush(brush);
}

void Layer::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  QGraphicsItem::mousePressEvent(event);
}

void Layer::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
  QGraphicsItem::mouseDoubleClickEvent(event);
}
