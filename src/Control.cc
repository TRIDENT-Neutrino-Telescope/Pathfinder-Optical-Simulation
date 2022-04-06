#include "Control.hh"

#include "filesystem"
#include "iostream"
#include "chrono"

using std::string;
using std::vector;

bool Control::readYAML(const string &fileYAML) {
  rootNode = YAML::LoadFile(fileYAML);
  try {
    radiusSource = rootNode["radius_source"].as<double>() * m;
    radiusDetector = rootNode["radius_detector"].as<double>() * m;
    thicknessSourceGlass = rootNode["thickness_source_glass"].as<double>() * m;

    string seed_mode = rootNode["random_seed"].as<string>();
    if (seed_mode == "time") {
      setRandomByTime();
    } else {
      randomSeed = rootNode["random_seed"].as<int>();
    }

    string fileProperties;
    try {
      fileProperties = rootNode["file_optical"].as<string>();
      if (fileProperties == "null") {
        useAbsolute = true;
      } else {
        useAbsolute = false;
      }
    } catch (YAML::Exception &e) {
      useAbsolute = true;
    }
    if (useAbsolute) {
      readOpticalProperties();
    } else {
      readOpticalProperties(fileProperties);
    }
    readOutputDataSettings();
    setNumPhoton(rootNode["number_of_photon_emit"].as<double>());

  } catch (YAML::BadConversion &e) {
    std::cerr << "[Read YAML] ==> {:s}" << e.msg << std::endl;
    return false;
  } catch (YAML::InvalidNode &e) {
    std::cerr << "[Read YAML] ==> {:s}" << e.msg << std::endl;
    return false;
  }
  return true;
}

void Control::setRandomByTime() {
  auto now = std::chrono::system_clock::now();
  auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
  auto epoch = now_ms.time_since_epoch();
  auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
  randomSeed = value.count();
}

void Control::readOpticalProperties(const std::string &fileProperties) {
  double absorptionRel = rootNode["relative"]["absorption"].as<double>();
  double mieRel = rootNode["relative"]["mie"].as<double>();
  double rayRel = rootNode["relative"]["ray"].as<double>();

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

void Control::readOpticalProperties() {
  geoOptical.num = 1;
  geoOptical.energy = new double(3.0 * eV);
  geoOptical.refracIdx = new double(rootNode["absolute"]["ref_idx"].as<double>());
  geoOptical.absLen = new double(rootNode["absolute"]["absorption"].as<double>() * m);
  geoOptical.scaLenRay = new double(rootNode["absolute"]["ray"].as<double>() * m);
  geoOptical.scaLenMie = new double(rootNode["absolute"]["mie"].as<double>() * m);
  geoOptical.mieForward = rootNode["absolute"]["mie_forward"].as<double>();
  geoOptical.mieBackward = rootNode["absolute"]["mie_backward"].as<double>();
  geoOptical.mieRatio = rootNode["absolute"]["mie_ratio"].as<double>();
}

void Control::readOutputDataSettings() {
  auto node = rootNode["output"];
  if (auto n_m = node["path_dir"]; n_m.IsDefined()) {
    if (n_m.as<std::string>() == "default")
      pathDir = "data";
    else
      pathDir = n_m.as<std::string>();
    std::filesystem::create_directories(pathDir);
  }

  if (auto n_m = node["file_name"]; n_m.IsDefined())
    fileName = n_m.as<std::string>();
}

void Control::setNumPhoton(int nPhoton) {
  constexpr int batchSize = 100000;
  nPhotonTotal = nPhoton;
  nPhotonLeft = nPhotonTotal;
  nEvent = std::ceil(static_cast<double>(nPhotonTotal) / batchSize);
}