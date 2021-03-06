#include "nullsymbol.h"

#include <QtGui>
#include <QRegExp>

#include "macros.h"


NullSymbol::NullSymbol(const QString& def, const Polarity& polarity,
    const AttribData& attrib):
    Symbol(def, "null([0-9.]+)", polarity, attrib), m_def(def)
{
  QRegExp rx(m_pattern);
  if (!rx.exactMatch(def))
    throw InvalidSymbolException(def.toAscii());

  QStringList caps = rx.capturedTexts();
  m_ext = caps[1].toInt();

  m_bounding = painterPath().boundingRect();
}

QPainterPath NullSymbol::painterPath(void)
{
  QPainterPath path;


  return path;
}
