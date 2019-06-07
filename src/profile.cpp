#include "profile.h"
#include <QDir>
#include <QDebug>

std::shared_ptr<Facebook> gFacebook;

bool LoadFacebook() {
  gFacebook.reset(new Facebook);
  QString dir_name = "images";
  QDir dir(dir_name);
  QString suffix = ".jpeg";
  QStringList filter("*" + suffix);
  QStringList files = dir.entryList(filter);
  if (files.empty()) {
    return false;
  }
  for (QString& file : files) {
    QString name  = file.remove(suffix);
    gFacebook->insert(name, {name, QImage(dir_name + "/" + file)});
  }
  return true;
}
