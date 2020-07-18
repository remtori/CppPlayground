workspace 'CppPlayground'
	architecture 'x86_64'

	configurations
	{
		'Debug',
		'Release',
	}

	flags
	{
		'MultiProcessorCompile',
	}

outputdir = '%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}'

function make_proj(src_dir, proj_kind)

	project(src_dir)
	location(src_dir)
	warnings 'Extra'

	local istest = false
	if (proj_kind == 'Test') then
		istest = true
		proj_kind = 'ConsoleApp'
	end

	kind(proj_kind or 'SharedLib')

	language 'C++'
	cppdialect 'C++17'
	staticruntime 'off'

	targetdir(_MAIN_SCRIPT_DIR .. '/bin/' .. outputdir)
	objdir(_MAIN_SCRIPT_DIR .. '/bin-int/' .. outputdir .. '/%{prj.name}')

	files
	{
		src_dir .. '/*.h',
		src_dir .. '/*.cpp',
	}

	includedirs
	{
		_MAIN_SCRIPT_DIR .. '/Libraries',
		_MAIN_SCRIPT_DIR .. '/third_party/v8/include',
	}

	libdirs
	{
		_MAIN_SCRIPT_DIR .. '/third_party/v8',
	}

	if (istest) then
		files
		{
			_MAIN_SCRIPT_DIR .. '/third_party/catch2/include/catch2/catch2.hpp',
			_MAIN_SCRIPT_DIR .. '/third_party/catch2/include/catch2/catch2.cpp',
		}

		includedirs
		{
			_MAIN_SCRIPT_DIR .. '/third_party/catch2/include',
		}
	end

	filter { 'kind:SharedLib', 'system:windows' }
		defines 'COMPILING_DLL'

	filter { 'system:linux', 'configurations:Debug' }
		linkoptions { '-rdynamic' }

	filter 'system:linux'
		toolset 'gcc'

	filter 'system:windows'
		toolset 'msc'

	filter 'configurations:Debug'
		defines 'DEBUG'
		runtime 'Debug'
		symbols 'on'

	filter 'configurations:Release'
		defines 'RELEASE'
		runtime 'Release'
		optimize 'on'

	-- Reset filter
	filter {}
end

-- group 'Libraries'
-- 	include './Libraries'

group 'Applications'
	include './Applications'

group ''
