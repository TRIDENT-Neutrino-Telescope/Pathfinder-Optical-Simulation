#!/usr/bin/env python3

import numpy as np
import yaml
import matplotlib.pyplot as plt
import matplotlib as mpl
font = {'family': 'serif',
        'weight': 'normal', 'size': 12}
mpl.rc('font', **font)

##############################
#
# calculate the optical property of water
# data is not precise yet
#
# eng for photon energy, in unit of eV
# self.waveLen for photon wave length, in unit of nm


class OpticalProperty:
    def __init__(self, photonEnergy_: np.ndarray) -> None:
        self.energy = photonEnergy_  # in unit of eV
        self.waveLen = 1240 / self.energy  # in unit of nm

    def calculateLens(self) -> None:
        parameterList = ["absLen", "rayScaLen",
                         "mieScaLen", "refracIdx"]
        functionList = ["AbsCoef", "RayScaCoef",
                        "MieScaCoef", "InverseRefracIdx"]
        functionList = [getattr(self, "calculate"+fname) for fname in functionList]
        for [para, func] in zip(parameterList, functionList):
            setattr(self, para, 1 / func())

    def calculateInverseRefracIdx(self):
        return 1 / 1.33 * np.ones(shape=self.energy.shape)

    def setMieParas(self, forward: float):
        self.mieForward = forward

    def writeInl(self) -> None:
        fileName = "{:s}.inl".format(self.name)
        length = len(self.energy)
        self.string = '#pragma once\n\n'
        self.string += '#include "G4SystemOfUnits.hh"\n'
        self.string += '#include "G4Material.hh"\n\n'
        self.string += "namespace {:s} {{\n".format(self.name)

        parameterList = ["energy", "absLen", "rayScaLen",
                         "mieScaLen", "refracIdx"]
        unitList = ["eV", "m", "m", "m", "1."]

        for [para, unit] in zip(parameterList, unitList):
            self.string += "inline G4double {:s}[{:d}] = {{\n".format(para, length)
            for para_ in getattr(self, para):
                self.string += "  {:.5f} * {:s}, \n".format(para_, unit)
            self.string += "};\n"

        if hasattr(self, "mieForward"):
            self.string += "inline G4double mieForward = {:.2f};\n".format(self.mieForward)

        self.string += "}  // namespace config"
        with open(fileName, "w") as file:
            file.write(self.string)

    def writeNPY(self) -> None:
        opticalData = np.array([self.energy, self.refracIdx, self.absLen, self.rayScaLen, self.mieScaLen])
        np.save(self.name+".npy", opticalData)

    def writeCSV(self) -> None:
        opticalData = np.array([self.energy, self.refracIdx, self.absLen, self.rayScaLen, self.mieScaLen])
        np.savetxt(self.name+".csv", opticalData, delimiter=",", fmt="%8.3f")
        with open(self.name+".csv", "a") as f:
            f.write(f"{self.mieForward:8.3f}\n")

    def writeYAML(self) -> None:
        dataOptical = {
            "energy": self.energy.tolist(),
            "refractive_index": self.refracIdx.tolist(),
            "absorption_length": self.absLen.tolist(),
            "scatter_length_rayeigh": self.rayScaLen.tolist(),
            "scatter_length_mie": self.mieScaLen.tolist(),
            "mie_forward_angle": self.mieForward,
        }
        with open(self.name+".yaml", "w") as f:
            yaml.dump(dataOptical, f)

    def calculateEffectiveScatterLen(self) -> None:
        self.effScaLen = 1 / (1 / self.rayScaLen
                              + (1 - self.mieForward) / self.mieScaLen)

    def calculateAttenuationLen(self) -> None:
        self.effAttLen = 1 / (1 / self.effScaLen + 1 / self.absLen)

    def plotImage(self) -> None:
        self.calculateEffectiveScatterLen()
        fig = plt.figure(figsize=(18, 4), dpi=300)
        ax1 = fig.add_subplot(1, 3, 1)
        ax1.plot(self.waveLen, self.absLen, color="#3498DB")
        ax1.set_xlim(300, 600)
        ax1.set_xlabel("Wave length / nm")
        ax1.set_ylabel("Absorption length / m")
        ax2 = fig.add_subplot(1, 3, 2)
        ax2.plot(self.waveLen, self.effScaLen, color="#3498DB")
        ax2.set_xlim(300, 600)
        ax2.set_xlabel("Wave length / nm")
        ax2.set_ylabel("Effective scattering length / m")
        ax2 = fig.add_subplot(1, 3, 3)
        ax2.plot(self.waveLen, self.refracIdx, color="#3498DB")
        ax2.set_xlim(300, 600)
        ax2.set_xlabel("Wave length / nm")
        ax2.set_ylabel("Refractive index")
        fig.savefig(self.name+".png", bbox_inches='tight')


class GlacialIce(OpticalProperty):
    """
    The optical properties of glacial ice measured by IceCube
    See:
    DOI: 10.1016/j.nima.2007.07.143 IceCube 2006
    DOI: 10.1016/j.nima.2013.01.054 IceCube 2013
    for more info. 
    """

    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "GlacialIce"
        self.setMieParas(0.94)

    def calculateInverseRefracIdx(self):
        waveLen_ = self.waveLen / 1000  # in micro meter
        refracIdx = (1.55749 - 1.57988 * (waveLen_)
                     + 3.99993 * np.power(waveLen_, 2)
                     - 4.68271 * np.power(waveLen_, 3)
                     + 2.09354 * np.power(waveLen_, 4))
        return 1 / refracIdx

    def calculateInverseGroupRefracIdx(self):
        waveLen_ = self.waveLen / 1000  # in micro meter
        factor = (1.227106 - 0.954648 * waveLen_
                  + 1.42568 * np.power(waveLen_, 2)
                  - 0.711832 * np.power(waveLen_, 3))
        return self.calculateInverseRefracIdx() / factor

    def calculateAbsCoef(self):
        return (0.01 * np.power(self.waveLen/400, -1.1)
                + 7000 * np.exp(-7000/self.waveLen))

    def calculateRayScaCoef(self):
        # no Raylei scattering in ice
        return (1e-6 * np.power(self.waveLen, 0.0))

    def calculateMieScaCoef(self):
        eff_sca_len_400 = 40  # From Table C1 of IceCube 2013 paper
        # It is a rough average of the depth dependent value
        sca_len_400 = eff_sca_len_400 * (1 - self.mieForward)
        sca_len = sca_len_400 * np.power(self.waveLen/400, 0.9)
        return 1 / sca_len


class CleanMedium(OpticalProperty):
    """
    The clean medium that does not absorbe and scatter.
    """

    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "CleanMedium"
        self.setMieParas(0.0)

    def calculateInverseRefracIdx(self):
        """
        From Bjorn Herold's PhD thesis Equation A 1
        """
        preasure = 250  # in unit of bar
        rWaveLen = 1 / self.waveLen
        refractiveIdx = (1.3201 + 1.4e-5 * preasure
                         + 16.2566 * rWaveLen
                         - 4383.0 * np.power(rWaveLen, 2)
                         + 1.1455e6 * np.power(rWaveLen, 3))
        return 1 / refractiveIdx

    def calculateAbsCoef(self):
        return (1e-6 * np.power(self.waveLen, 0.0))

    def calculateRayScaCoef(self):
        return (1e-6 * np.power(self.waveLen, 0.0))

    def calculateMieScaCoef(self):
        return (1e-6 * np.power(self.waveLen, 0.0))


class NatureWater(OpticalProperty):
    """
    The optical properties at Lake Baikal measured by NT200
    See 10.1016/j.nima.2012.06.035 for more info.
    """

    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "NatureWater"
        # from section 8.3 of NT200 paper
        self.setMieParas(0.9)

    def calculateInverseRefracIdx(self):
        """
        ! This is not true for nature water
        From Bjorn Herold's PhD thesis Equation A 1
        """
        preasure = 250  # in unit of bar
        rWaveLen = 1 / self.waveLen
        refractiveIdx = (1.3201 + 1.4e-5 * preasure
                         + 16.2566 * rWaveLen
                         - 4383.0 * np.power(rWaveLen, 2)
                         + 1.1455e6 * np.power(rWaveLen, 3))
        return 1 / refractiveIdx

    def calculateAbsCoef(self):
        """
        From Fig. 6 of NT200 paper
        """
        return (180. * np.exp(-9.5 * self.waveLen / 500.) +
                7e-7 * np.exp(10 * self.waveLen / 500.))

    def calculateRayScaCoef(self):
        """
        No measurement data
        """
        return (1e-6 * np.power(self.waveLen, 0.0))

    def calculateMieScaCoef(self):
        """
        From Fig. 9 and Fig. 10 of NT200 paper
        """
        scaLenCoef_488 = 0.1
        return scaLenCoef_488 * np.power(self.waveLen/488, -1.)


class SeaWater(OpticalProperty):
    """
    The optical properties at Mid Sea measured by Antares.
    This is mainly based on the partic 0.075 model produced by Antares.
    """

    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "SeaWater"
        self.setMieParas(0.93)  # from DOI:10.3389/fphy.2018.00132

    def calculateInverseRefracIdx(self):
        """
        From Bjorn Herold's PhD thesis Equation A 1
        """
        preasure = 250  # in unit of bar
        rWaveLen = 1 / self.waveLen
        refractiveIdx = (1.3201 + 1.4e-5 * preasure
                         + 16.2566 * rWaveLen
                         - 4383.0 * np.power(rWaveLen, 2)
                         + 1.1455e6 * np.power(rWaveLen, 3))
        return 1 / refractiveIdx

    def calculateAbsCoef(self):
        """
        From Claudio Kopper's PhD thesis Fig. 4.2
        """
        return (180. * np.exp(-10*self.waveLen / 450)
                + 1.5e-6 * np.exp(8.5*self.waveLen / 450))

    def calculateRayScaCoef(self):
        """
        From Claudio Kopper's PhD thesis Fig. 4.2
        """
        wl_ = (self.waveLen - 300) / 300
        # The average length of scattering (Rayleigh + Mie)
        scaLenTot = (8.78 * np.power(wl_, 2)
                     + 55.16 * wl_ + 18.49)
        ratioRay = 0.17  # the percentage of Rayleigh scattering in all scatterings
        scaLenRay = scaLenTot / ratioRay
        return 1 / scaLenRay

    def calculateMieScaCoef(self):
        """
        From Claudio Kopper's PhD thesis Fig. 4.2
        """
        wl_ = (self.waveLen - 300) / 300
        # The average length of scattering (Rayleigh + Mie)
        scaLenTot = (8.78 * np.power(wl_, 2)
                     + 55.16 * wl_ + 18.49)
        ratioRay = 0.17  # the percentage of Rayleigh scattering in all scatterings
        scaLenMie = scaLenTot / (1 - ratioRay)
        return 1 / scaLenMie


#####################
#
# main function to calculate optical property of water
#
if __name__ == "__main__":
    # 300 nm to 600 nm range
    photonEnergyList = np.linspace(1240./600., 1240./300., 300)
    listClassMedium = [CleanMedium, GlacialIce, SeaWater, NatureWater]
    for classMedium in listClassMedium:
        medium = classMedium(photonEnergyList)
        medium.calculateLens()
        medium.writeYAML()
        medium.writeNPY()
        medium.plotImage()
