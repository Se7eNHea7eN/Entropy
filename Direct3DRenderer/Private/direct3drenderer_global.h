#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(DIRECT3DRENDERER_LIB)
#  define DIRECT3DRENDERER_EXPORT Q_DECL_EXPORT
# else
#  define DIRECT3DRENDERER_EXPORT Q_DECL_IMPORT
# endif
#else
# define DIRECT3DRENDERER_EXPORT
#endif
