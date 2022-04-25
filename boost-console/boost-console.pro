CONFIG -= qt
#QT += core
CONFIG += c++17 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lboost_atomic-mt -lboost_chrono-mt -lboost_container-mt -lboost_context-mt -lboost_contract-mt -lboost_coroutine-mt -lboost_date_time-mt -lboost_exception-mt -lboost_fiber-mt -lboost_filesystem-mt -lboost_graph-mt -lboost_iostreams-mt -lboost_json-mt -lboost_locale-mt -lboost_log-mt -lboost_log_setup-mt -lboost_math_c99-mt -lboost_math_c99f-mt -lboost_math_c99l-mt -lboost_math_tr1-mt -lboost_math_tr1f-mt -lboost_math_tr1l-mt -lboost_nowide-mt -lboost_numpy39-mt -lboost_prg_exec_monitor-mt -lboost_program_options-mt -lboost_python39-mt -lboost_random-mt -lboost_regex-mt -lboost_serialization-mt -lboost_stacktrace_noop-mt -lboost_stacktrace_windbg-mt -lboost_stacktrace_windbg_cached-mt -lboost_system-mt -lboost_test_exec_monitor-mt -lboost_thread-mt -lboost_timer-mt -lboost_type_erasure-mt -lboost_unit_test_framework-mt -lboost_wave-mt -lboost_wserialization-mt

DEFINES += POCO_STATIC
LIBS += -lPocoData -lPocoDataMySQL -lPocoDataPostgreSQL -lPocoDataSQLite -lPocoEncodings -lPocoJWT -lPocoNet -lPocoNetSSL -lPocoRedis -lPocoUtil -lPocoXML -lPocoZip -lPocoFoundation -lPocoCrypto -lPocoJSON
#LIBS += -liphlpapi
#LIBS += -lexpat -lpcre
#LIBS += -lz
#LIBS += -lwsock32
#LIBS += -lssl -lcrypto
#LIBS +=  -lws2_32

LIBS += -lwsock32
LIBS += -lssl -lcrypto
LIBS += -lws2_32
LIBS += -lcrypt32

SOURCES += \
        main.cpp

DESTDIR = $$PWD/..

TARGET = $${TARGET}-$${QMAKE_HOST.arch}

#message($$QMAKE_QMAKE)
contains(QMAKE_QMAKE, .*\-static\/.*) {
    message( "[STATIC BUILD]" )
    DEFINES += QT_STATIC_BUILD
    TARGET = $${TARGET}-static
} else {
    message( "[SHARED BUILD]" )
}

DISTFILES += \
    ../data.json
