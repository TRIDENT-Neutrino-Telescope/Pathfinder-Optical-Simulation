#pragma once

#include "G4SystemOfUnits.hh"

#include "memory"
#include "string"
#include "vector"
#include "yaml-cpp/yaml.h"

struct OpticalProperty {
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

class Control {
public:
  Control(const Control &) = delete;
  Control &operator=(const Control &) = delete;
  ~Control(){};

  static std::shared_ptr<Control> Instance() {
    static std::shared_ptr<Control> instance{new Control};
    return instance;
  }

  YAML::Node getRootNode() { return rootNode; }
  bool readYAML(const std::string &fileYAML);
  void readOpticalProperties(const std::string &fileProperties);
  void readOpticalProperties();
  void readOutputDataSettings();
  void setNumPhoton(int nPhoton);
  void setRandomByTime();

private:
  Control(){};

public:
  YAML::Node rootNode;

  OpticalProperty geoOptical;
  bool useAbsolute; // use absolute optical properties
  double radiusSource = 0.2 * m;
  double thicknessSourceGlass = 0.015 * m;
  double radiusDetector;
  int nPhotonTotal;
  int nPhotonLeft;
  int nEvent;
  int randomSeed;

  // Output data parameters
  std::string pathDir;
  std::string fileName;
};
