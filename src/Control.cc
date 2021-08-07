#include "Control.hh"
#include "G4SystemOfUnits.hh"
#include "iostream"
using std::string;
using std::vector;

bool Control::readYAML(const std::string &fileYAML)
{
  rootNode = YAML::LoadFile(fileYAML);
  try {

  radiusSource = rootNode["radius_source"].as<double>() * m;
  radiusDetector = rootNode["radius_detector"].as<double>() * m;

  string fileProperties = rootNode["file_optical"].as<string>();
  if (fileProperties == "null")
  {
    useAbsolute = true;
  
    geoOptical.num = 2;
  
    geoOptical.energy = new double[geoOptical.num];
    geoOptical.energy[0] = 2.0 * eV;
    geoOptical.energy[1] = 4.5 * eV;
  
    geoOptical.refracIdx = new double[geoOptical.num];
    for (int i = 0; i < geoOptical.num; i++) 
      geoOptical.refracIdx[i] = rootNode["absolute"]["ref_idx"].as<double>();

    geoOptical.absLen = new double[geoOptical.num];
    for (int i = 0; i < geoOptical.num; i++)
      geoOptical.absLen[i] = rootNode["absolute"]["absorption"].as<double>() * m;

    geoOptical.scaLenRay = new double[geoOptical.num];
    for (int i = 0; i < geoOptical.num; i++)
      geoOptical.scaLenRay[i] = rootNode["absolute"]["ray"].as<double>() * m;

    geoOptical.scaLenMie = new double[geoOptical.num];
    for (int i = 0; i < geoOptical.num; i++)
      geoOptical.scaLenMie[i] = rootNode["absolute"]["mie"].as<double>() * m;

    geoOptical.mieForward = rootNode["absolute"]["mie_forward"].as<double>();
    geoOptical.mieBackward = rootNode["absolute"]["mie_backward"].as<double>();
    geoOptical.mieRatio = rootNode["absolute"]["mie_ratio"].as<double>();
  }
  else
  {
    useAbsolute = false;
    readOpticalProperties(fileProperties);
  }
  }
  catch (YAML::BadConversion &e)
  {
    std::cerr << "[Read YAML] ==> {:s}" << e.msg << std::endl;
    return false;
  }
  catch (YAML::InvalidNode &e)
  {
    std::cerr << "[Read YAML] ==> {:s}" << e.msg << std::endl;
    return false;
  }
  return true;
}

void Control::readOpticalProperties(const std::string &fileProperties)
{
  double absorptionRel = rootNode["absorption"]["relative"].as<double>();
  double mieRel = rootNode["mie"]["relative"].as<double>();
  double rayRel = rootNode["ray"]["relative"].as<double>();

  auto node = YAML::LoadFile(fileProperties);
  vector<double> energy_;
  vector<double> refracIdx_;
  vector<double> absLen_;
  vector<double> scaLenRay_;
  vector<double> scaLenMie_;

  energy_ = node["energy"].as<vector<double>>();
  geoOptical.num = energy_.size();
  geoOptical.energy = new double[geoOptical.num];
  for (int i = 0; i < geoOptical.num; i++)
    geoOptical.energy[i] = energy_[i] * eV;

  refracIdx_ = node["refractive_index"].as<vector<double>>();
  geoOptical.refracIdx = new double[geoOptical.num];
  for (int i = 0; i < geoOptical.num; i++)
    geoOptical.refracIdx[i] = refracIdx_[i];

  absLen_ = node["absorption_length"].as<vector<double>>();
  geoOptical.absLen = new double[geoOptical.num];
  for (int i = 0; i < geoOptical.num; i++)
    geoOptical.absLen[i] = absorptionRel * absLen_[i] * m;

  scaLenRay_ = node["scatter_length_rayeigh"].as<vector<double>>();
  geoOptical.scaLenRay = new double[geoOptical.num];
  for (int i = 0; i < geoOptical.num; i++)
    geoOptical.scaLenRay[i] = rayRel * scaLenRay_[i] * m;

  scaLenMie_ = node["scatter_length_mie"].as<vector<double>>();
  geoOptical.scaLenMie = new double[geoOptical.num];
  for (int i = 0; i < geoOptical.num; i++)
    geoOptical.scaLenMie[i] = mieRel * scaLenMie_[i] * m;

  geoOptical.mieForward = node["mie_forward_angle"].as<double>();
}
