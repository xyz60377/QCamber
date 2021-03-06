#include "ellipsesymbol.h"

#include <QtGui>
#include <QRegExp>

#include "macros.h"


EllipseSymbol::EllipseSymbol(const QString& def, const Polarity& polarity,
    const AttribData& attrib):
    Symbol(def, "el([0-9.]+)x([0-9.]+)", polarity, attrib), m_def(def)
{
  QRegExp rx(m_pattern);
  if (!rx.exactMatch(def))
    throw InvalidSymbolException(def.toAscii());

  QStringList caps = rx.capturedTexts();
  m_w = caps[1].toDouble() / 1000.0;
  m_h = caps[2].toDouble() / 1000.0;

  m_bounding = painterPath().boundingRect();
}

QPainterPath EllipseSymbol::painterPath(void)
{
  QPainterPath path;

  path.addEllipse(-m_w / 2, -m_h / 2, m_w, m_h);

  return path;
}
