#include "mainwin.h"
#include <QApplication>
#include <boost/program_options.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    // Check program options
    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()
            ("debug,d",  "Enable debug info.")
            ("help,h" ,  "Print help.");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc),vm); // can throw

        // --help,-h  option: prints help
        if (vm.count("help")) {
            std::cout << "Server command line parameters." << std::endl
                    << desc << std::endl;
            return 0;
        }
        po::notify(vm); // throws on error, so do after help in case
                        // there are any problems

        // --debug,-d  option: enables printing debug info
        if (vm.count("debug")) {
            pd_debug_info = true;
            std::cout << "Debug info enabled." << std::endl;
            DBG("Debug info enabled.");
        }
    }
    catch(po::error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
        std::cerr << desc << std::endl;
        return 1;
    }


    // Run application
    QApplication a(argc, argv);
    MainWin w;
    w.show();

    return a.exec();
}
