#include "yaml-cpp/yaml.h"
#include "string"
#include "vector"
#include "memory"

struct OpticalProperty
{
  int num;
  double *energy;
  double *refracIdx;
  double *absLen;
  double *scaLenRay;
  double *scaLenMie;
  double mieForward;
  double mieBackward = 0.0;
  double mieRatio = 1.0;
};

class Control
{
public:
  Control(const Control &) = delete;
  Control &operator=(const Control &) = delete;
  ~Control() {};

  static std::shared_ptr<Control> Instance()
  {
    static std::shared_ptr<Control> instance{new Control};
    return instance;
  }

  YAML::Node getRootNode() { return rootNode; }
  bool readYAML(const std::string &fileYAML);
  void readOpticalProperties(const std::string &fileProperties);

private:
  Control() {};

public:
  YAML::Node rootNode;

  OpticalProperty geoOptical;
  bool useAbsolute;  // use absolute optical properties
  double radiusSource;
  double radiusDetector;


  // Output data parameters
  std::string pathDir;
  std::string fileName;
};
