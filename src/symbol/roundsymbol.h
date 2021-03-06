#ifndef __ROUNDSYMBOL_H__
#define __ROUNDSYMBOL_H__

#include "symbol.h"

class RoundSymbol: public Symbol {
public:

  RoundSymbol(const QString& def, const Polarity& polarity,
      const AttribData& attrib);

  virtual QPainterPath painterPath(void);

protected:

private:
  QString m_def;
  qreal m_r;
};

#endif /* __ROUNDSYMBOL_H__ */
