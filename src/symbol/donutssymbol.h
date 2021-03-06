#ifndef __DONUTSSYMBOL_H__
#define __DONUTSSYMBOL_H__

#include "symbol.h"

class DonutSSymbol: public Symbol {
public:

  DonutSSymbol(const QString& def, const Polarity& polarity,
      const AttribData& attrib);

  virtual QPainterPath painterPath(void);

protected:

private:
  QString m_def;
  qreal m_od;
  qreal m_id;
};

#endif /* __DONUTSSYMBOL_H__ */
