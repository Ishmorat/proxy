#include "config.h"

Config::Config(int port_, const std::string& pg_addr_, int pg_port_, const std::string& log_filepath_)
    : port        { port_           }
    , pg_addr     { pg_addr_        }
    , pg_port     { pg_port_        }
    , log_filepath{ log_filepath_   }
{}

Config Config::create(int argc, char* argv[]) {
    Config cfg;
    for (size_t i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if      (arg.starts_with("-port="    )) cfg.port = std::atoi(arg.c_str() + 6);
        else if (arg.starts_with("-pg_addr=" )) cfg.pg_addr = arg.c_str() + 9;
        else if (arg.starts_with("-pg_port=" )) cfg.pg_port = std::atoi(arg.c_str() + 9);
        else if (arg.starts_with("-log_file=")) cfg.log_filepath = arg.c_str() + 10;
    }
    return cfg;
}

void Config::print() {
    std::cout << "port     : "  << port
            << "\npg_addr  : " << pg_addr
            << "\npg_port  : " << pg_port
            << "\nlog_file : " << log_filepath << std::endl;
}
