SUMMARY = "Application de mesure SecGW (capteur simule)"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://meteo-app.c \
           file://meteo-app.service"

S = "${WORKDIR}"

inherit systemd

SYSTEMD_SERVICE:${PN} = "meteo-app.service"
SYSTEMD_AUTO_ENABLE = "enable"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} meteo-app.c -o meteo-app
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 meteo-app ${D}${bindir}/meteo-app

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 meteo-app.service ${D}${systemd_system_unitdir}/meteo-app.service
}

