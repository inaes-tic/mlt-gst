# Note: plugindir is set in configure

##############################################################################
# TODO: change libgstmltshmproto.la to something else, e.g. libmysomething.la     #
##############################################################################
plugin_LTLIBRARIES = libgstmltshmproto.la

##############################################################################
# TODO: for the next set of variables, name the prefix if you named the .la, #
#  e.g. libmysomething.la => libmysomething_la_SOURCES                       #
#                            libmysomething_la_CFLAGS                        #
#                            libmysomething_la_LIBADD                        #
#                            libmysomething_la_LDFLAGS                       #
##############################################################################

# sources used to compile this plug-in
libgstmltshmproto_la_SOURCES = gstmltshmproto.c mltcommon.c

# compiler and linker flags used to compile this plugin, set in configure.ac
libgstmltshmproto_la_CFLAGS = $(GST_CFLAGS)
libgstmltshmproto_la_LIBADD = $(GST_LIBS)
libgstmltshmproto_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstmltshmproto_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstmltshmproto.h mltcommon.h
