#include "logger.h"

voxey::core::Logger::Logger()
{
}

voxey::core::Logger::~Logger()
{
}

void voxey::core::Logger::run()
{
    std::cout << "running logger" << std::endl;
    //TODO: just so i can remember, i changed from mingw to msvc lib to fix it
	glfwSetErrorCallback(glfwErrorCallback); 
}

void voxey::core::Logger::glfwErrorCallback(int err, const char* desc)
{
	std::cerr << "error glfw '" << err << "': " << desc << std::endl;
}

void voxey::core::Logger::dPrint(const char* name, const char* desc, [[maybe_unused]] const std::vector<std::string>& args)
{
    std::string formattedDesc = desc;

    if (!formattedDesc.empty() && !std::isupper(formattedDesc[0]))
    {
        formattedDesc[0] = std::toupper(formattedDesc[0]);
    }

    if (!args.empty())
    {
        std::stringstream ss;
        size_t start = 0;
        size_t end = formattedDesc.find('%', start);
        size_t argIdx = 0;

        while (end != std::string::npos && argIdx < args.size())
        {
            ss << formattedDesc.substr(start, end - start);
            ss << args[argIdx++];
            start = end + 1;
            end = formattedDesc.find('%', start);
        }

        ss << formattedDesc.substr(start);
        formattedDesc = ss.str();
    }

    std::cout << name << ": " << formattedDesc << std::endl;
}
