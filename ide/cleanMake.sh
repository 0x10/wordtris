#!/bin/sh

LINKNAME=`readlink -f $0`
DIRNAME=`dirname $LINKNAME`
echo "DIR: "$DIRNAME
echo "PWD: "`pwd`

DTR=""
FTR=""

cd ${DIRNAME}
TARCOPY=saved.pro.tar
OLDTARCOPY=old.${TARCOPY}
#find . -name *.pro.user* -exec tar rf ${DIRNAME}/${TARCOPY} {} \;
if [ -e ${OLDTARCOPY} ]; then rm ${OLDTARCOPY}; fi
if [ -e ${TARCOPY} ]; then mv ${TARCOPY} ${OLDTARCOPY}; fi
find . -name *.pro.user* -exec tar rf ${TARCOPY} {} \;

cleanDir() {
	#echo "cleanDir $1 $2"
	#/usr/bin/find "$1" -maxdepth 1 -name "$2" -exec echo "  + "{} \;
	#/usr/bin/find "$1" -maxdepth 1 -name "$2" -exec rm -rf {} \;
	#R=`/usr/bin/find "$1" -maxdepth 1 -name "$2" -exec echo {} \;`
	R=`/usr/bin/find . -maxdepth 1 -name "$2" -exec echo {} \;`
	if [ "x$R" != "x" ]; then
		DTR="$DTR $R"
	fi
}

cleanFile() {
	#echo "cleanFile $1 $2"
	#/usr/bin/find "$1" -maxdepth 1 -name "$2" -exec echo "  + "{} \;
	#/usr/bin/find "$1" -maxdepth 1 -name "$2" -exec rm {} \;
	R=`/usr/bin/find . -maxdepth 1 -name "$2" -exec echo {} \;`
	if [ "x$R" != "x" ]; then
		FTR="$FTR $R"
	fi
}

BD=${DIRNAME}/

for pd in .
do
	loc=${BD}${pd}
	echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	echo "@ $pd"
	cd $loc

	FTR=""

	for f in android-*-deployment-settings.json app_process libc.so linker Makefile .qmake.stash *.o
	do
		#echo " - $f"
		cleanFile $loc $f
	done

	if [ "x$FTR" != "x" ]; then
		echo $FTR
		rm $FTR
	fi

	DTR=""

	for d in build builddir* *builddir android-build
	do
		#echo " = $d"
		cleanDir $loc $d
	done

	if [ "x$DTR" != "x" ]; then
		echo $DTR
		rm -rf $DTR
	fi
done

