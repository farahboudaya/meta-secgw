SUMMARY = "Image securisee SecGW"
DESCRIPTION = "Image minimale durcie pour la passerelle SecGW"
LICENSE = "MIT"

inherit core-image

IMAGE_INSTALL:append = " os-release"
IMAGE_FEATURES += "ssh-server-dropbear"
