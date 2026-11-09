SUMMARY = "Image securisee SecGW"
DESCRIPTION = "Image minimale durcie pour la passerelle SecGW"
LICENSE = "MIT"

inherit image
IMAGE_FEATURES += "read-only-rootfs"

IMAGE_INSTALL = " \
    packagegroup-core-boot \
    os-release \
    dropbear \
    systemd-analyze \
    secgw-gateway \
"
IMAGE_INSTALL = " \
    packagegroup-core-boot \
    os-release \
    dropbear \
    systemd-analyze \
    secgw-gateway \
    can-utils \
    iproute2 \
"
