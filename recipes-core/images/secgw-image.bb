SUMMARY = "Image securisee SecGW"
DESCRIPTION = "Image minimale durcie pour la passerelle SecGW"
LICENSE = "MIT"

inherit image

IMAGE_FEATURES += "ssh-server-dropbear"

IMAGE_INSTALL = " \
    packagegroup-core-boot \
    os-release \
    systemd-analyze \
    meteo-app \


