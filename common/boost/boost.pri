# https://boostorg.jfrog.io/ui/repos/tree/General/main/release
# Visual C++ 14.2 (2019)
# Visual C++ 14.3 (2022)

gcc {
    LIBS += -lboost_atomic-mt -lboost_chrono-mt -lboost_container-mt -lboost_context-mt -lboost_contract-mt -lboost_coroutine-mt -lboost_date_time-mt -lboost_exception-mt -lboost_fiber-mt -lboost_filesystem-mt -lboost_graph-mt -lboost_iostreams-mt -lboost_json-mt -lboost_locale-mt -lboost_log-mt -lboost_log_setup-mt -lboost_math_c99-mt -lboost_math_c99f-mt -lboost_math_c99l-mt -lboost_math_tr1-mt -lboost_math_tr1f-mt -lboost_math_tr1l-mt -lboost_nowide-mt -lboost_numpy39-mt -lboost_prg_exec_monitor-mt -lboost_program_options-mt -lboost_python39-mt -lboost_random-mt -lboost_regex-mt -lboost_serialization-mt -lboost_stacktrace_noop-mt -lboost_stacktrace_windbg-mt -lboost_stacktrace_windbg_cached-mt -lboost_system-mt -lboost_test_exec_monitor-mt -lboost_thread-mt -lboost_timer-mt -lboost_type_erasure-mt -lboost_unit_test_framework-mt -lboost_wave-mt -lboost_wserialization-mt
}
msvc {
    INCLUDEPATH += C:/local/boost_1_79_0
    LIBS += "-LC:/local/boost_1_79_0/lib64-msvc-14.2"
}
