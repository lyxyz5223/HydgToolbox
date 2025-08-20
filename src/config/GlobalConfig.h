#include <string>

class GlobalConfig
{
private:
    GlobalConfig() = delete;
    ~GlobalConfig() = delete;
public:
    static std::string ZeroMQBindUrl; // ZeroMQ 绑定地址
    static std::string PluginDirectory; // 插件目录
};

