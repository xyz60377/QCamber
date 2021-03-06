#include "record.h"

#include <QDateTime>
#include <QPainterPath>
#include <QRegExp>
#include <QString>
#include <QTransform>

#include "featuresparser.h"
#include "symbolfactory.h"

TextRecord::TextRecord(FeaturesDataStore* ds, const QStringList& param,
    const AttribData& attr):
  Record(ds, attr)
{
  if (param.empty()) {
    return;
  }

  int i = 0;
  x = param[++i].toDouble();
  y = param[++i].toDouble();
  font = param[++i];
  polarity = (param[++i] == "P")? P: N;
  orient = (Orient)param[++i].toInt();
  xsize = param[++i].toDouble();
  ysize = param[++i].toDouble();
  width_factor = param[++i].toDouble();
  text = dynamicText(param[++i]);
  version = param[++i].toInt();
}

Symbol* TextRecord::createSymbol(void) const
{
  Symbol* symbol = new TextSymbol(this);
  setTransform(symbol);
  return symbol;
}

void TextRecord::setTransform(Symbol* symbol) const
{
  symbol->setPos(x, -y);

  if (orient >= M_0) {
    QTransform trans;
    trans.scale(-1, 1);
    symbol->setTransform(trans);
  }
  symbol->setRotation((orient % 4) * 90);
}

QString TextRecord::dynamicText(QString text)
{
  QString dynText = text;
  const QDate &Date = QDate::currentDate();
  const QTime &Time = QTime::currentTime();
  dynText.replace("$$date-ddmmyy", QString().sprintf("%02d/%02d/%02d",
        Date.day(), Date.month(), Date.year()%100), Qt::CaseInsensitive);
  dynText.replace("$$date", QString().sprintf("%02d/%02d/%02d",
        Date.month(), Date.day(), Date.year()%100), Qt::CaseInsensitive);
  dynText.replace("$$time", QString().sprintf("%02d:%02d",
        Time.hour(), Time.minute()), Qt::CaseInsensitive);

  FeaturesDataStore* fds = dynamic_cast<FeaturesDataStore*>(ds);
  dynText.replace("$$job", fds->jobName(), Qt::CaseInsensitive);
  dynText.replace("$$step", fds->stepName(), Qt::CaseInsensitive);
  dynText.replace("$$layer", fds->layerName(), Qt::CaseInsensitive);

  dynText.replace("$$x_mm", QString("%1").arg(x*25.4), Qt::CaseInsensitive);
  dynText.replace("$$y_mm", QString("%1").arg(y*25.4), Qt::CaseInsensitive);

  dynText.replace("$$x", QString("%1").arg(x), Qt::CaseInsensitive);
  dynText.replace("$$y", QString("%1").arg(y), Qt::CaseInsensitive);

  QRegExp rx("\\$\\$(\\S+)");
  int pos = 0;
  while ((pos = rx.indexIn(dynText, pos)) != -1) {
    QString replacement = fds->attrlist(rx.cap(1)).toUpper();
    dynText.replace(pos, rx.cap(0).length(), replacement);
    pos += replacement.length();
  }

  return dynText;
}
