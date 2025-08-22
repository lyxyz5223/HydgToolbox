#include "CPlugin.h"

CPlugin::CPlugin(/* args */)
{
}

CPlugin::~CPlugin()
{
}


void CPlugin::main(const PluginContext *context)
{
  if (_main) {
    _main(context);
  } else {
    // 处理未定义的主函数
    logger.error("C plugin function 'main' not found.");
  }
}

bool CPlugin::shutdown()
{
  if (_shutdown) {
    return _shutdown();
  }
  return true; // 正常关闭
}

const char* CPlugin::getId()
{
  if (_getId) {
    return _getId();
  }
  return "No Id";
}


const char* CPlugin::getName()
{
  if (_getName) {
    return _getName();
  }
  return "No Name";
}

const char* CPlugin::getVersion()
{
  if (_getVersion) {
    return _getVersion();
  }
  return "No Version";
}

const char* CPlugin::getAuthor()
{
  if (_getAuthor) {
    return _getAuthor();
  }
  return "No Author";
}

const char* CPlugin::getDescription()
{
  if (_getDescription) {
    return _getDescription();
  }
  return "No Description";
}

