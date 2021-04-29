#!/usr/bin/env python3

import numpy as np

##############################
#
# calculate the optical property of water
# data is not precise yet
#
# eng for photon energy, in unit of eV
# wl for photon wave length, in unit of nm


class OpticalProperty:
    def __init__(self, photonEnergy_) -> None:
        self.energy = photonEnergy_
        self.waveLen = 1240 / self.energy

    def calculateLens(self) -> None:
        parameterList = ["absLen", "rayScaLen",
                         "mieScaLen", "refracIdx"]
        functionList = ["AbsCoef", "RayScaCoef",
                        "MieScaCoef", "InverseRefracIdx"]
        functionList = [getattr(self, "calculate"+fname) for fname in functionList]
        for [para, func] in zip(parameterList, functionList):
            setattr(self, para, 1 / func(self.waveLen))

    def calculateInverseRefracIdx(self, wl):
        return 1 / (1.55749 - 1.57988 * (wl/1000)
                    + 3.99993 * np.power(wl/1000, 2)
                    - 4.68271 * np.power(wl/1000, 3)
                    + 2.09354 * np.power(wl/1000, 4))

    def setMieParas(self, forward, backward, ratio):
        self.mieForward = forward
        self.mieBackward = backward
        self.mieRatio = ratio

    def writeNPY(self) -> None:
        opticalData = np.array([self.energy, self.refracIdx, self.absLen, self.rayScaLen, self.mieScaLen])
        np.save(self.name+".npy", opticalData)

class PureWater(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "PureWater"
        self.setMieParas(0.0, 0.0, 0.0)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))


class Rayleigh(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Rayleigh"
        self.setMieParas(0.0, 0.0, 0.0)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return 0.1 * np.power(wl/450, -3.5)

    def calculateMieScaCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

class Rayleigh_Sca2(Rayleigh):
    def __init__(self, photonEnergy_: np.ndarray):
        Rayleigh.__init__(self, photonEnergy_)
        self.name = "Rayleigh_Sca2"

    def calculateRayScaCoef(self, wl):
        return 0.5 * np.power(wl/450, -3.5)

class Rayleigh_Sca3(Rayleigh):
    def __init__(self, photonEnergy_: np.ndarray):
        Rayleigh.__init__(self, photonEnergy_)
        self.name = "Rayleigh_Sca3"

    def calculateRayScaCoef(self, wl):
        return 2.5 * np.power(wl/450, -3.5)


class NatureWater(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "NatureWater"
        self.setMieParas(0.9, 0.3, 0.8)

    def calculateAbsCoef(self, wl):
        return (360. * np.exp(-wl / 45.) + 3e-6 * np.exp(wl / 53.))

    def calculateRayScaCoef(self, wl):
        return 0.005 * np.power(wl/450, -3.5)

    def calculateMieScaCoef(self, wl):
        return 1.1 * np.power(wl/450, -1.)


class NatureWater_Sca2(NatureWater):
    def __init__(self, photonEnergy_: np.ndarray):
        NatureWater.__init__(self, photonEnergy_)
        self.name = "NatureWater_Sca2"

    def calculateRayScaCoef(self, wl):
        return NatureWater.calculateRayScaCoef(self, wl)*2

    def calculateMieScaCoef(self, wl):
        return NatureWater.calculateMieScaCoef(self, wl)*2


class NatureWater_Sca3(NatureWater):
    def __init__(self, photonEnergy_: np.ndarray):
        NatureWater.__init__(self, photonEnergy_)
        self.name = "NatureWater_Sca3"

    def calculateRayScaCoef(self, wl):
        return NatureWater.calculateRayScaCoef(self, wl)*3

    def calculateMieScaCoef(self, wl):
        return NatureWater.calculateMieScaCoef(self, wl)*3


class NatureWater_SD(NatureWater):
    def __init__(self, photonEnergy_: np.ndarray):
        NatureWater.__init__(self, photonEnergy_)
        self.name = "NatureWater_SD"
        self.setMieParas(0.8, 0.3, 0.8)

    def calculateMieScaCoef(self, wl):
        return NatureWater.calculateMieScaCoef(self, wl)/2


class NatureWater_SD_Sca2(NatureWater):
    def __init__(self, photonEnergy_: np.ndarray):
        NatureWater.__init__(self, photonEnergy_)
        self.name = "NatureWater_SD_Sca2"
        self.setMieParas(0.8, 0.3, 0.8)

    def calculateMieScaCoef(self, wl):
        return NatureWater.calculateMieScaCoef(self, wl)/2*2


class NatureWater_SD_Sca3(NatureWater):
    def __init__(self, photonEnergy_: np.ndarray):
        NatureWater.__init__(self, photonEnergy_)
        self.name = "NatureWater_SD_Sca3"
        self.setMieParas(0.8, 0.3, 0.8)

    def calculateMieScaCoef(self, wl):
        return NatureWater.calculateMieScaCoef(self, wl)/2*3


class SeaWaterV0(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "SeaWaterV0"
        self.setMieParas(0.0, 0.0, 0.0)

    def calculateAbsCoef(self, wl):
        return (180. * np.exp(-wl / 45.) + 1.5e-6 * np.exp(wl / 53.))

    def calculateRayScaCoef(self, wl):
        return 0.0045 * np.power(wl/450, -3.4)

    def calculateMieScaCoef(self, wl):
        return np.power(wl, 0.0)


class SeaWaterV0_Sca2(SeaWaterV0):
    def __init__(self, photonEnergy_: np.ndarray):
        SeaWaterV0.__init__(self, photonEnergy_)
        self.name = "SeaWaterV0_Sca2"

    def calculateRayScaCoef(self, wl):
        return SeaWaterV0.calculateRayScaCoef(self, wl) * 2


class SeaWaterV0_Sca3(SeaWaterV0):
    def __init__(self, photonEnergy_: np.ndarray):
        SeaWaterV0.__init__(self, photonEnergy_)
        self.name = "SeaWaterV0_Sca3"

    def calculateRayScaCoef(self, wl):
        return SeaWaterV0.calculateRayScaCoef(self, wl) * 3

class Water_M5R5(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M5R5"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.2 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.5 * np.power(wl, 0.0)/6.25)

class Water_M5R10(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M5R10"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.1 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.5 * np.power(wl, 0.0)/6.25)

class Water_M5R15(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M5R15"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.0667 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.5 * np.power(wl, 0.0)/6.25)

class Water_M5R20(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M5R20"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.05 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.5 * np.power(wl, 0.0)/6.25)

class Water_M5R25(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M5R25"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.04 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.5 * np.power(wl, 0.0)/6.25)

class Water_M5R30(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M5R30"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.0333 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.5 * np.power(wl, 0.0)/6.25)

class Water_M10R5(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M10R5"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.2 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.25 * np.power(wl, 0.0)/6.25)

class Water_M10R10(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M10R10"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.1 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.25 * np.power(wl, 0.0)/6.25)

class Water_M10R15(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M10R15"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.0667 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.25 * np.power(wl, 0.0)/6.25)

class Water_M10R20(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M10R20"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.05 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.25 * np.power(wl, 0.0)/6.25)

class Water_M10R25(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M10R25"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.04 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.25 * np.power(wl, 0.0)/6.25)

class Water_M10R30(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M10R30"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.0333 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.25 * np.power(wl, 0.0)/6.25)


class Water_M15R5(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M15R5"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.2 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.16667 * np.power(wl, 0.0)/6.25)

class Water_M15R10(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M15R10"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.1 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.16667 * np.power(wl, 0.0)/6.25)

class Water_M15R15(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M15R15"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.0667 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.16667 * np.power(wl, 0.0)/6.25)

class Water_M15R20(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M15R20"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.05 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.16667 * np.power(wl, 0.0)/6.25)

class Water_M15R25(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M15R25"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.04 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.16667 * np.power(wl, 0.0)/6.25)

class Water_M15R30(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M15R30"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.0333 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.16667 * np.power(wl, 0.0)/6.25)

class Water_M20R5(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M20R5"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.2 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.125 * np.power(wl, 0.0)/6.25)

class Water_M20R10(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M20R10"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.1 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.125 * np.power(wl, 0.0)/6.25)

class Water_M20R15(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M20R15"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.0667 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.125 * np.power(wl, 0.0)/6.25)

class Water_M20R20(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M20R20"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.05 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.125 * np.power(wl, 0.0)/6.25)

class Water_M20R25(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M20R25"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.04 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.125 * np.power(wl, 0.0)/6.25)

class Water_M20R30(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M20R30"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.0333 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.125 * np.power(wl, 0.0)/6.25)

class Water_M25R5(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M25R5"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.2 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.1 * np.power(wl, 0.0)/6.25)

class Water_M25R10(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M25R10"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.1 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.1 * np.power(wl, 0.0)/6.25)

class Water_M25R15(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M25R15"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.0667 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.1 * np.power(wl, 0.0)/6.25)

class Water_M25R20(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M25R20"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.05 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.1 * np.power(wl, 0.0)/6.25)

class Water_M25R25(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M25R25"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.04 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.1 * np.power(wl, 0.0)/6.25)

class Water_M25R30(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M25R30"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.0333 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.1 * np.power(wl, 0.0)/6.25)

class Water_M30R5(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M30R5"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.2 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.08333 * np.power(wl, 0.0)/6.25)

class Water_M30R10(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M30R10"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.1 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.08333 * np.power(wl, 0.0)/6.25)

class Water_M30R15(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M30R15"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.0667 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.08333 * np.power(wl, 0.0)/6.25)

class Water_M30R20(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M30R20"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.05 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.08333 * np.power(wl, 0.0)/6.25)

class Water_M30R25(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M30R25"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.04 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.08333 * np.power(wl, 0.0)/6.25)

class Water_M30R30(OpticalProperty):
    def __init__(self, photonEnergy_: np.ndarray):
        OpticalProperty.__init__(self, photonEnergy_)
        self.name = "Water_M30R30"
        self.setMieParas(0.6, 0.6, 0.8)

    def calculateAbsCoef(self, wl):
        return (0.001 * np.power(wl, 0.0))

    def calculateRayScaCoef(self, wl):
        return (0.0333 * np.power(wl, 0.0))

    def calculateMieScaCoef(self, wl):
        return (0.0833 * np.power(wl, 0.0)/6.25)

#####################
#
# main function to calculate optical property of water
#
if __name__ == "__main__":
    # 300 nm to 600 nm range
    photonEnergyList = np.linspace(1240./600., 1240./300., 300)
    classList = [Water_M5R5,Water_M5R10,Water_M5R15,Water_M5R20,Water_M5R25,Water_M5R30,
                 Water_M10R5,Water_M10R10,Water_M10R15,Water_M10R20,Water_M10R25,Water_M10R30,
                 Water_M15R5,Water_M15R10,Water_M15R15,Water_M15R20,Water_M15R25,Water_M15R30,
                 Water_M20R5,Water_M20R10,Water_M20R15,Water_M20R20,Water_M20R25,Water_M20R30,
                 Water_M25R5,Water_M25R10,Water_M25R15,Water_M25R20,Water_M25R25,Water_M25R30,
                 Water_M30R5,Water_M30R10,Water_M30R15,Water_M30R20,Water_M30R25,Water_M30R30,
                 PureWater, Rayleigh, Rayleigh_Sca2, Rayleigh_Sca3,
                 NatureWater, NatureWater_Sca2, NatureWater_Sca3,
                 NatureWater_SD, NatureWater_SD_Sca2, NatureWater_SD_Sca3,
                 SeaWaterV0, SeaWaterV0_Sca2, SeaWaterV0_Sca3]
    for className in classList:
        medium = className(photonEnergyList)
        medium.calculateLens()
        medium.writeNPY()

    # nature_water = NatureWater(photonEnergyList)
    # nature_water.calculateLens()
    # nature_water.writeInl()
    # sea_water = SeaWaterV0(photonEnergyList)
    # sea_water.calculateLens()
    # sea_water.writeInl()
    # sea_water = SeaWaterV0_Sca2(photonEnergyList)
    # sea_water.calculateLens()
    # sea_water.writeInl()
    # sea_water = SeaWaterV0_Sca3(photonEnergyList)
    # sea_water.calculateLens()
    # sea_water.writeInl()
