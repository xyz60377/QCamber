#ifndef __OVALSYMBOL_H__
#define __OVALSYMBOL_H__

#include "symbol.h"

class OvalSymbol: public Symbol {
public:

  OvalSymbol(const QString& def, const Polarity& polarity,
      const AttribData& attrib);

  virtual QPainterPath painterPath(void);

protected:

private:
  QString m_def;
  qreal m_w;
  qreal m_h;
};

#endif /* __OVALSYMBOL_H__ */
