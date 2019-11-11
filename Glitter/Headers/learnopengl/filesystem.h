#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <cstdlib>

class FileSystem
{
public:
  typedef std::string (*Builder) (const std::string& path);
  static std::string getPath(const std::string& path);

private:
  static const std::string& getRoot();

  //static std::string(*foo (std::string const &)) getPathBuilder()
  static Builder getPathBuilder();

  static std::string getPathRelativeRoot(const std::string& path);

  static std::string getPathRelativeBinary(const std::string& path);
};

// FILESYSTEM_H
#endif
