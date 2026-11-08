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
    meteo-app \
"
secgw_manifest() {
    cd ${IMAGE_ROOTFS}
    sha256sum usr/bin/meteo-app > ${IMAGE_ROOTFS}${sysconfdir}/secgw-manifest.sha256
}
ROOTFS_POSTPROCESS_COMMAND += "secgw_manifest;"
