# CppPlayground

Playing around with C++ but this time embracing open-source / library

## Workspace Settings

- Version: c++17

- Compiler: g++

- Configurable define macro:

```
DISABLE_ASSERT: define this to disable assertion
DISABLE_LOGGING: define this to disable logging
```

- Include Dirs

```
Libraries/
third_party/catch2/include/
third_party/v8/include/
third_party/glm/
third_party/GLFW/include/
third_party/glad/include/
third_party/spdlog/include/
third_party/imgui/
third_party/stb/
```

- Required system lib:

	- `opengl32.lib` on Window or `gl` on Linux

	- `X11` on Linux

	- `DbgHelp` with msvc or `cxxabi` with gcc