#include "notes.h"

#include <QtGui>

#include "context.h"
#include "notesparser.h"

Notes::Notes(QString step, QString layer): Symbol("symbol"), m_empty(false)
{
  QString filename = ctx.loader->absPath(QString("steps/%1/layers/%2/notes")
      .arg(step).arg(layer));

  NotesParser parser(filename);
  NotesDataStore* ds = parser.parse();
  if (!ds) {
    m_empty = true;
    return;
  }

  QList<NoteRecord*> records = ds->records();
  for (QList<NoteRecord*>::const_iterator it = ds->records().begin();
      it != ds->records().end(); ++it) {
    addChild((*it)->createSymbol());
  }
}

Notes::~Notes()
{
}

bool Notes::empty(void)
{
  return m_empty;
}
