
EGIT_REPO_URI="git://repo.or.cz/qbat.git"

inherit eutils flag-o-matic git

DESCRIPTION="QBat is a small tool based on Qt4 to display battery stats in system tray."
HOMEPAGE="http://repo.or.cz/qbat.git"
SRC_URI=""

LICENSE="GPL-2.0"
SLOT="0"
KEYWORDS="~x86 ~amd64"
IUSE="debug"

RDEPEND=">=x11-libs/qt-4.3.2"

DEPEND="${RDEPEND}
	sys-kernel/linux-headers"

S=${WORKDIR}/${EGIT_PROJECT}

pkg_setup() {
	if ! qmake --version | grep -i qt4 ; then
		eerror "qmake does not point to qmake-qt4"
		die "Install qmake-qt4 and set symlinks correctly"
	fi
}


src_unpack() {
	#check version:
	if [ "$PV" != 9999 ]; then
		EGIT_TREE="v${PV}"
	fi
	git_src_unpack
}

src_compile() {
	cd "${S}"
	config_defines=""
	config_release=""
	if use debug; then 
		config_release="debug"
	else
		config_release="release"
		config_defines="$config_defines QT_NO_DEBUG_OUTPUT"
	fi
	
	qmake -recursive -Wall "CONFIG+=$config_release" "DEFINES+=$config_defines"
	emake || die
}

src_install() {
	make INSTALL_ROOT="${D}" install || die
}

pkg_postinst() {
	elog "You need a at least kernel version 2.6.24 to use this software"
}
