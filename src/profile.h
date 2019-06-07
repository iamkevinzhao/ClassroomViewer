#ifndef PROFILE_H
#define PROFILE_H

#include <QImage>
#include <QHash>
#include <memory>

struct Profile
{
  QString name;
  QImage image;
};

using Facebook = QHash<QString, Profile>;

extern std::shared_ptr<Facebook> gFacebook;

bool LoadFacebook();

#endif // PROFILE_H
