CONFIG( debug, debug|release ) {
  message( No installation available in Debug mode )
} else {
 !win32:error(install_win.pri should only be used when building win32 configuration)
 LIB_EXTENSION = dll

 CONFIG(crosscompil) {
    QTLIBS_INSTALL_PATH = $${TARGET_INSTALL_PATH}
    QTLIBS_PATH = /home/eric/.wine/drive_c/Qt/2010.02/qt/bin
    QTPLUGINS_PATH = $${QTLIBS_PATH}/../plugins
    qtlibs.path  = $${QTLIBS_INSTALL_PATH}
    qtlibs.files = $${QTLIBS_PATH}/*mingw* \
                   $${QTLIBS_PATH}/*QtCore4* \
                   $${QTLIBS_PATH}/*QtGui4* \
                   $${QTLIBS_PATH}/*QtSql4* \
                   $${QTLIBS_PATH}/*QtScript4* \
                   $${QTLIBS_PATH}/*Qt3Support4* \
                   $${QTLIBS_PATH}/*QtXml4* \
                   $${QTLIBS_PATH}/*QtSvg4* \
                   $${QTLIBS_PATH}/*QtNetwork4*
    INSTALLS += qtlibs
    message( Cross-compilation : Qt Libs will be installed from $${QTLIBS_PATH} to $${QTLIBS_INSTALL_PATH})

 } else {
    # define root path and install path
    isEmpty(INSTALL_ROOT_PATH):INSTALL_ROOT_PATH = $${RELEASE_BINARY_PATH}/$${INSTALL_BASENAME_PATH}/$${BINARY_TARGET}
    INSTALL_BINARY_PATH    = $${INSTALL_ROOT_PATH}
    INSTALL_LIBS_PATH      = $${INSTALL_BINARY_PATH}
    INSTALL_PLUGINS_PATH   = $${INSTALL_LIBS_PATH}/plugins

    # install Qt libs and plugs
    INSTALL_QT_INSIDE_BUNDLE = yes
    INSTALL_QT_LIBS_PATH     = $${INSTALL_BINARY_PATH}
    INSTALL_QT_PLUGINS_PATH  = $${INSTALL_BINARY_PATH}/plugins/qt

    message( Win32 Bundle : Qt Libs will be installed from $$[QT_INSTALL_LIBS] to $${INSTALL_QT_LIBS_PATH})
    message( Win32 Bundle : Binary Wrapper will be : $${INSTALL_BINARY_WRAPPER_NAME})
 }

}  #end if release mode


