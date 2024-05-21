#include "spdlog/spdlog.h"
//#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
//#include "spdlog/sinks/tee_sink.h"

int main() {
    try {
        // 创建控制台日志 sink
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::warn);
        //console_sink->set_pattern("[%H:%M:%S %z] [%P - %t] [%l]");
        // 设置不同级别的消息颜色
    // console_sink->set_color(spdlog::level::trace, console_sink->bold + console_sink->black);
    // console_sink->set_color(spdlog::level::debug, console_sink->bold + console_sink->green);
    // console_sink->set_color(spdlog::level::info, console_sink->bold + console_sink->white);
    // console_sink->set_color(spdlog::level::warn, console_sink->bold + console_sink->yellow);
    // console_sink->set_color(spdlog::level::err, console_sink->bold + console_sink->red);
    // console_sink->set_color(spdlog::level::critical, console_sink->bold + console_sink->white + console_sink->on_red);

//console_sink->set_color(spdlog::level::err, console_sink->bold);
//console_sink->set_color(spdlog::level::err, console_sink->green);
    // const string_view_t black = "\033[30m";
    // const string_view_t red = "\033[31m";
    // const string_view_t green = "\033[32m";
    // const string_view_t yellow = "\033[33m";
    // const string_view_t blue = "\033[34m";
    // const string_view_t magenta = "\033[35m";
    // const string_view_t cyan = "\033[36m";
    // const string_view_t white = "\033[37m";
    
// 创建轮转文件日志 sink
        size_t max_file_size = 1048576 * 1; // 1MB
        size_t max_files = 100; // 保留最近的 100 个日志文件
        auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/rotating_log.txt", max_file_size, max_files);
        
        // 创建文件日志 sink
        //auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/multisink.txt");
        rotating_sink->set_level(spdlog::level::debug);
        //file_sink->set_pattern("[%H:%M:%S %z] [thread %t] [%l] %v");

        // 创建多目标 sink
        std::vector<spdlog::sink_ptr> sinks {console_sink, rotating_sink};
        auto logger = std::make_shared<spdlog::logger>("multi_sink", begin(sinks), end(sinks));
        //logger->set_level(spdlog::level::debug);
        //logger->set_pattern("[%H:%M:%S %z] [thread %t] [%l] %v");
        logger->set_pattern("[%Y%m%d %H%M%S %e] [%P - %t] [%l] %v");
        spdlog::register_logger(logger);

        // 使用日志器
        logger->error("{}  This message should appear in both console and file", 111111);
        


        // 使用默认日志器（如果需要）
        //spdlog::info("This is another info message");
    }
    catch (const spdlog::spdlog_ex &ex) {
        //std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }

    return 0;
}
