#pragma once
#include <string>
// 用于C++插件的信息结构体
struct HydgPluginInfo
{
  std::string filePath; // 插件文件路径(含文件名)
  std::string id;
  std::string name;
  std::string version;
  std::string author;
  std::string description;
  HydgPluginInfo() = default; // 默认构造函数
  // 防止结构体赋值不全
  HydgPluginInfo(const std::string &filePath, const std::string &id, const std::string &name, const std::string &version,
                 const std::string &author, const std::string &description)
      : filePath(filePath), id(id), name(name), version(version), author(author), description(description) {}
};
