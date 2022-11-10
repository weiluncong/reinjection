QT.multimedia_private.VERSION = 5.13.0
QT.multimedia_private.name = QtMultimedia
QT.multimedia_private.module =
QT.multimedia_private.libs = $$QT_MODULE_LIB_BASE
QT.multimedia_private.includes = $$QT_MODULE_INCLUDE_BASE/QtMultimedia/5.13.0 $$QT_MODULE_INCLUDE_BASE/QtMultimedia/5.13.0/QtMultimedia
QT.multimedia_private.frameworks =
QT.multimedia_private.depends = core_private gui_private multimedia
QT.multimedia_private.uses = pulseaudio
QT.multimedia_private.module_config = v2 internal_module
QT.multimedia_private.enabled_features = alsa gstreamer_1_0 gstreamer gstreamer_app gstreamer_encodingprofiles linux_v4l pulseaudio
QT.multimedia_private.disabled_features = directshow evr gpu_vivante gstreamer_0_10 gstreamer_photography openal resourcepolicy wasapi wmf wmsdk wshellitem
QMAKE_LIBS_ALSA = -lasound
QMAKE_LIBS_GSTREAMER = -lgstaudio-1.0 -lgstvideo-1.0 -lgstbase-1.0 -lgstpbutils-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
QMAKE_INCDIR_GSTREAMER = /usr/include/gstreamer-1.0 /usr/include/glib-2.0 /usr/lib64/glib-2.0/include
QMAKE_DEPENDS_GSTREAMER_APP_CC = GSTREAMER
QMAKE_DEPENDS_GSTREAMER_APP_LD = GSTREAMER
QMAKE_LIBS_GSTREAMER_APP = -lgstapp-1.0
QMAKE_LIBS_PULSEAUDIO = -lpulse-mainloop-glib -lpulse -lglib-2.0
QMAKE_DEFINES_PULSEAUDIO = _REENTRANT
QMAKE_INCDIR_PULSEAUDIO = /usr/include/glib-2.0 /usr/lib64/glib-2.0/include
