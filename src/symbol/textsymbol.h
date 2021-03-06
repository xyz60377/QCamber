#ifndef __TEXTSYMBOL_H__
#define __TEXTSYMBOL_H__

#include "symbol.h"

#include <QtGui>

#include "featuresparser.h"
#include "record.h"

class TextSymbol: public Symbol {
public:
  TextSymbol(const TextRecord* rec);

  virtual QString infoText(void);
  virtual QString longInfoText(void);
  virtual QPainterPath painterPath(void);

protected:
  qreal m_x, m_y;
  QString m_font;
  Orient m_orient;
  qreal m_xsize, m_ysize;
  qreal m_width_factor;
  QString m_text;
  int m_version;
};

#endif /* __TEXTSYMBOL_H__ */
