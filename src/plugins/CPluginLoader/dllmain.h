#include "../../PublicHeaders/HydgPluginInterface.h"

extern "C"
{
  __declspec(dllexport) HydgPlugin *create();
  __declspec(dllexport) void destroy(HydgPlugin *plugin);
}
