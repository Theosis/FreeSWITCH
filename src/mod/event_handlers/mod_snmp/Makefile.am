include $(top_srcdir)/build/modmake.rulesam
MODNAME=mod_snmp

mod_LTLIBRARIES = mod_snmp.la
mod_snmp_la_SOURCES  = mod_snmp.c subagent.c
mod_snmp_la_CFLAGS   = $(AM_CFLAGS) `net-snmp-config --cflags`
mod_snmp_la_LIBADD   = $(switch_builddir)/libfreeswitch.la
mod_snmp_la_LDFLAGS  = -avoid-version -module -no-undefined -shared `net-snmp-config --agent-libs`
