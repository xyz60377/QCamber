#ifndef __HORIZONTALHEXAGONSYMBOL_H__
#define __HORIZONTALHEXAGONSYMBOL_H__

#include "symbol.h"

class HorizontalHexagonSymbol: public Symbol {
public:

  HorizontalHexagonSymbol(const QString& def, const Polarity& polarity,
      const AttribData& attrib);

  virtual QPainterPath painterPath(void);

protected:

private:
  QString m_def;
  qreal m_w;
  qreal m_h;
  qreal m_r;
};

#endif /* __HORIZONTALHEXAGONSYMBOL_H__ */
