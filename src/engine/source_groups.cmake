set(source_files)

add_file_folder("Core"
    core/core.h
    core/core.cpp
    core/logging.h
    core/logging.cpp
    core/vmallocator.h
    core/cmdline.h
    core/cmdline.cpp
)

add_file_folder("Spdlog"
    spdlog/async.h
    spdlog/async_logger-inl.h
    spdlog/async_logger.h
    spdlog/cfg
    spdlog/common-inl.h
    spdlog/common.h
    spdlog/details
    spdlog/fmt
    spdlog/formatter.h
    spdlog/fwd.h
    spdlog/logger-inl.h
    spdlog/logger.h
    spdlog/pattern_formatter-inl.h
    spdlog/pattern_formatter.h
    spdlog/sinks
    spdlog/spdlog-inl.h
    spdlog/spdlog.h
    spdlog/stopwatch.h
    spdlog/tweakme.h
    spdlog/version.h
)

