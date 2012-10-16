#!/bin/sh

TARGET="fc"
MODULES="sql"
GMAKE="/usr/local/bin/gmake"
QMAKE="/usr/local/bin/qmake-qt4"
DEFINES="DEBUG"
COMPILER=clang

${GMAKE} distclean

${QMAKE} -project

if [ -e ${TARGET}.pro ]
then
	# modules
	echo "QT += ${MODULES}" >> ${TARGET}.pro;
	echo "modules \"${MODULES}\" was added.";
	# DEFINES
	echo "DEFINES += ${DEFINES}" >> ${TARGET}.pro;
	${QMAKE} -spec freebsd-${COMPILER};
	# echo "Для поддержки shape добавь -DWITH_SHAPE и -lshp в Makefile"
else
	echo "ERROR: file ${TARGET}.pro not found."
fi

