project 'Glad'
    kind 'StaticLib'
    language 'C'
    staticruntime 'on'

    targetdir ('bin/' .. outputdir)
    objdir ('bin-int/' .. outputdir .. '/%{prj.name}')

    files
    {
        'include/glad/glad.h',
        'include/KHR/khrplatform.h',
        'src/glad.c'
    }

    includedirs
    {
        'include'
    }

    filter 'system:windows'
        links { 'opengl32.lib' }
        systemversion 'latest'

    filter 'system:linux'
        links { 'GL' }

    filter 'configurations:Debug'
        runtime 'Debug'
        symbols 'on'

    filter 'configurations:Release'
        runtime 'Release'
        optimize 'on'
