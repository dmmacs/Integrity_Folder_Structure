#ifndef VERSION_H
#define VERSION_H

#ifdef QT_DEBUG
namespace Version
{
 static const int MAJOR = 0;
 static const int MINOR = 3;
 static const int BUILD = 318;
}
#else
namespace Version
{
 static const int MAJOR = 1;
 static const int MINOR = 0;
 static const int BUILD = -1;
}

#endif


#endif // VERSION_H
