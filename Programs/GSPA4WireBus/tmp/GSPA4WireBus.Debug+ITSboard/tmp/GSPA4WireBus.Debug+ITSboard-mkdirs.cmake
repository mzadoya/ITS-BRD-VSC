# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/maximzadoya/Desktop/gsItsBoard2/ITS-BRD-VSC/Programs/GSPA4WireBus/tmp/GSPA4WireBus.Debug+ITSboard"
  "/Users/maximzadoya/Desktop/gsItsBoard2/ITS-BRD-VSC/Programs/GSPA4WireBus/tmp/1"
  "/Users/maximzadoya/Desktop/gsItsBoard2/ITS-BRD-VSC/Programs/GSPA4WireBus/tmp/GSPA4WireBus.Debug+ITSboard"
  "/Users/maximzadoya/Desktop/gsItsBoard2/ITS-BRD-VSC/Programs/GSPA4WireBus/tmp/GSPA4WireBus.Debug+ITSboard/tmp"
  "/Users/maximzadoya/Desktop/gsItsBoard2/ITS-BRD-VSC/Programs/GSPA4WireBus/tmp/GSPA4WireBus.Debug+ITSboard/src/GSPA4WireBus.Debug+ITSboard-stamp"
  "/Users/maximzadoya/Desktop/gsItsBoard2/ITS-BRD-VSC/Programs/GSPA4WireBus/tmp/GSPA4WireBus.Debug+ITSboard/src"
  "/Users/maximzadoya/Desktop/gsItsBoard2/ITS-BRD-VSC/Programs/GSPA4WireBus/tmp/GSPA4WireBus.Debug+ITSboard/src/GSPA4WireBus.Debug+ITSboard-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/maximzadoya/Desktop/gsItsBoard2/ITS-BRD-VSC/Programs/GSPA4WireBus/tmp/GSPA4WireBus.Debug+ITSboard/src/GSPA4WireBus.Debug+ITSboard-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/maximzadoya/Desktop/gsItsBoard2/ITS-BRD-VSC/Programs/GSPA4WireBus/tmp/GSPA4WireBus.Debug+ITSboard/src/GSPA4WireBus.Debug+ITSboard-stamp${cfgdir}") # cfgdir has leading slash
endif()
