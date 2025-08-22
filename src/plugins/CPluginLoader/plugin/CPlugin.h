#include "HydgPluginInterface.h"
#include "Logger.h"

/**
 * C++插件实现，用于兼容C语言插件
 * @author lyxyz5223
 */
class CPlugin : public HydgPlugin
{
public:
  typedef const char *(*GetInfoFunc)();
  typedef void (*MainFunc)(const PluginContext *);
  typedef bool (*ShutdownFunc)();

private:
  Logger logger{"CPluginLoader CPlugin"};
  GetInfoFunc _getId = nullptr;
  GetInfoFunc _getName = nullptr;
  GetInfoFunc _getVersion = nullptr;
  GetInfoFunc _getAuthor = nullptr;
  GetInfoFunc _getDescription = nullptr;
  MainFunc _main = nullptr;
  ShutdownFunc _shutdown = nullptr;

public:
  CPlugin(/* args */);
  ~CPlugin();

  const char* getId() override;
  const char* getName() override;
  const char* getVersion() override;
  const char* getAuthor() override;
  const char* getDescription() override;
  void main(const PluginContext *context) override;
  bool shutdown() override;

  // C插件函数的setter函数
  void setGetIdFunc(GetInfoFunc func) { _getId = func; }
  void setGetNameFunc(GetInfoFunc func) { _getName = func; }
  void setGetVersionFunc(GetInfoFunc func) { _getVersion = func; }
  void setGetAuthorFunc(GetInfoFunc func) { _getAuthor = func; }
  void setGetDescriptionFunc(GetInfoFunc func) { _getDescription = func; }
  void setMainFunc(MainFunc func) { _main = func; }
  void setShutdownFunc(ShutdownFunc func) { _shutdown = func; }
};


