#include <boost/filesystem.hpp>
#include <iostream>

//using namespace boost::filesystem;
namespace fs = boost::filesystem;

int main()
{
    //path p{"C:\\Windows\\System"};
    //fs::path p{"~"};

    fs::path p(getenv("HOME"));

    //fs::create_directory(fs::path(getenv("HOME")));

    std::cout << p.root_name() << '\n';
    std::cout << p.root_directory() << '\n';
    std::cout << p.root_path() << '\n';
    std::cout << p.relative_path() << '\n';
    std::cout << p.parent_path() << '\n';
    std::cout << p.filename() << '\n';
}
