#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(GLRENDERER_LIB)
#  define GLRENDERER_EXPORT Q_DECL_EXPORT
# else
#  define GLRENDERER_EXPORT Q_DECL_IMPORT
# endif
#else
# define GLRENDERER_EXPORT
#endif
