SUMMARY = "Passerelle CAN SecGW : detection d'anomalies sur bus CAN"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://secgw-gateway.c \
           file://ecu-sim.c \
           file://secgw-gateway.service \
           file://vcan0.service"

S = "${WORKDIR}"

inherit systemd

SYSTEMD_SERVICE:${PN} = "vcan0.service secgw-gateway.service"
SYSTEMD_AUTO_ENABLE = "enable"

RDEPENDS:${PN} += "iproute2"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} secgw-gateway.c -o secgw-gateway
    ${CC} ${CFLAGS} ${LDFLAGS} ecu-sim.c -o ecu-sim
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 secgw-gateway ${D}${bindir}/secgw-gateway
    install -m 0755 ecu-sim ${D}${bindir}/ecu-sim

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 secgw-gateway.service ${D}${systemd_system_unitdir}/
    install -m 0644 vcan0.service ${D}${systemd_system_unitdir}/
}

