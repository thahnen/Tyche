# Tyche: QC Software for taking images using the picoflexx camera

## Abstract

A software for quality control using the picoflexx camera to gather images from the target.
Has the ability to view the current images taken by the camera aswell as saving them to file.

## Usability

Implemented as a Windows executable for use on the Windows platform by using the WIN32 API.
As such the software is not platform independant nor does the software work with another camera.
There is an Inno Setup install script for creating a portable installer!

## Prerequisites

Needs following software installed:
1. Microsoft Windows 7/8/8.1/10 64-Bit or higher
2. [OpenCV 4+](https://opencv.org/) for working on images
3. [PMD royale library](https://pmdtec.com/mwc/) for access to the camera