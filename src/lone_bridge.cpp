#include <string>

#ifdef _WIN32
#define LONE_API __declspec(dllexport)
#else
#define LONE_API
#endif

extern "C" LONE_API const char* lone_saluta() {
    static const std::string message =
        "Ciao da C++! L'integrazione con Python funziona tramite DLL.";
    return message.c_str();
}

extern "C" LONE_API int lone_add(int a, int b) {
    return a + b;
}
